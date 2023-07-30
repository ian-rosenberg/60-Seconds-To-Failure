#include "gamearea.h"
#include "staticentity.h"

GameArea::GameArea(int ID, b2Vec2 grav, const std::shared_ptr<Graphics>& graphics, Vector2 playerDim) {
	Vector2 screenDim;
	id = ID;
	player = nullptr;
	entityManager = new EntityManager(1, graphics);//enabling debug draw with parameter, renderer
	gravityScale = grav;
	gravityEnabled = gravityScale.y != 0 || gravityScale.x != 0;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	this->graphics = graphics;

	cameraFollowStrength = 0.1f;
	
	screenDim = graphics->GetScreenDimensions();

	camera = new Camera(
		SDL_Rect(0,
			0,
			screenDim.x,
			screenDim.y),
		Vector4(0,0,0,0));

	debugDraw = new DebugDraw(graphics, camera);

	InitPhysicsWorld();

	tileManager = new TileManager("SideViewTest", 
		graphics, 
		areaPhysics, 
		playerDim);

	std::vector<std::vector<Tile*>>* tilemap = tileManager->GenerateTileMap(areaPhysics, playerDim);

	camera->SetBounds(tileManager->GetBounds());

	tileManager->LinkTilemapGhostVertices(tilemap);

	debugDraw->AddTileMapRef(tilemap);

	playerPixelDimensions = playerDim;
}

GameArea::~GameArea() {
	delete entityManager;
	delete debugDraw;
	delete listener;
    delete camera;
	delete tileManager;

	delete areaPhysics;
	player = nullptr;

	graphics.reset();
}

void GameArea::AreaThink() {
	if (!active)
		return;

	entityManager->EntityThinkAll();
}

void GameArea::AreaUpdate() {
	if (!active)
		return;

	tileManager->UpdateMap();
	entityManager->EntityUpdateAll();
	camera->Move(player->GetDrawPosition(), cameraFollowStrength);
	entityManager->InputUpdate();
}

//https://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
void GameArea::PhysicsSteps(float deltaTime) {
	const int MAX_STEPS = 5;

	fixedTimestepAccum += deltaTime;
	const int nSteps = static_cast<int> (
		std::floor(fixedTimestepAccum / timeStep)
		);
	// To avoid rounding errors, touches fixedTimestepAccumulator_ only
	// if needed.
	if (nSteps > 0)
	{
		fixedTimestepAccum -= nSteps * timeStep;
	}

	assert(
		"Accumulator must have a value lesser than the fixed time step" &&
		fixedTimestepAccum < timeStep + FLT_EPSILON
	);

	fixedTimestepAccumRatio = fixedTimestepAccum / timeStep;

	// This is similar to clamp "dt":
	//deltaTime = std::min(deltaTime, (float)(MAX_STEPS * timeStep));
	// but it allows above calculations of fixedTimestepAccumulator_ and
	// fixedTimestepAccumulatorRatio_ to remain unchanged.
	const int nStepsClamped = std::min(nSteps, MAX_STEPS);
	//std::cout << nStepsClamped << std::endl;
	for (int i = 0; i < nStepsClamped; ++i)
	{
		// In singleStep_() the CollisionManager could fire custom
		// callbacks that uses the smoothed states. So we must be sure
		// to reset them correctly before firing the callbacks.
		ResetSmoothStates();
		SinglePhysicsStep(timeStep);
	}

	areaPhysics->ClearForces();

	// We "smooth" positions and orientations using
	// fixedTimestepAccumulatorRatio_ (alpha).
	SmoothPhysicsStates();
}

void GameArea::SinglePhysicsStep(float deltaTime)
{	
	areaPhysics->Step(timeStep, velocityIterations, positionIterations);
	
	player->ToggleGrounded(false);
	
	for (auto c = listener->_contacts.begin(); c != listener->_contacts.end(); c++) {
		Contact contact = *c;

		if (contact.fixtureA == player->GetJumpTrigger()
			|| contact.fixtureB == player->GetJumpTrigger()) {
			if (!player->IsGrounded()) {
				player->ToggleGrounded(true);
			}
			break;
		}
	}
}

void GameArea::SmoothPhysicsStates()
{
	const float oneMinusRatio = 1.f - fixedTimestepAccumRatio;

	for (b2Body* b = areaPhysics->GetBodyList(); b != nullptr; b = b->GetNext())
	{
		if (b->GetType() == b2_staticBody)
		{
			continue;
		}

		PhysicsComponent* c = (PhysicsComponent*)(b->GetUserData().pointer);
		c->smoothedPosition =
			fixedTimestepAccumRatio * b->GetPosition() +
			oneMinusRatio * c->prevPosition;
		c->smoothedAngle =
			fixedTimestepAccumRatio * b->GetAngle() +
			oneMinusRatio * c->prevAngle;
	}
}

void GameArea::ResetSmoothStates()
{
	for (b2Body* b = areaPhysics->GetBodyList(); b != nullptr; b = b->GetNext())
	{
		if (b->GetType() == b2_staticBody)
		{
			continue;
		}

		PhysicsComponent* c = (PhysicsComponent*)(b->GetUserData().pointer);
		c->smoothedPosition = c->prevPosition = b->GetPosition();
		c->smoothedAngle = c->prevAngle = b->GetAngle();
		//b->SetTransform(c->smoothedPosition, c->smoothedAngle);
	}
}

void GameArea::AreaDraw(float accum) {
	SDL_Rect camRect = camera->GetRect();

	if (!active)
		return;


	//interpolate all ze positions
	tileManager->DrawMap(Vector2(camera->GetRect().x, camera->GetRect().y), camRect);
	entityManager->EntityDrawAll(camera->GetRect(), accum);
	debugDraw->DrawAll(accum, camRect);
}


b2Vec2 GameArea::FindSpawnPointFromLeft()
{
	std::vector<std::vector<Tile*>>* tilemap = tileManager->GetTileMap();
	Vector2 pDim = playerPixelDimensions;
	int col = 1;

	for (int row = tilemap->size() - 2; col < tilemap[row].size()-2; row--) {
		if (row < 1) {
			row = tilemap->size()-2;
			col++;
		}
		
		if (tilemap->at(row).at(col) != nullptr && tilemap->at(row-1).at(col) == nullptr) {
			Vector2 worldSpawn(col * tileManager->GetTileDimensions().x, (row - 1) * tileManager->GetTileDimensions().y);
			graphics->Vector2PixelsToMeters(worldSpawn);

			return b2Vec2(worldSpawn.x + pDim.x / 2, worldSpawn.y);
		}
	}

	return b2Vec2(0,0);
}

void GameArea::InitPhysicsWorld()
{
	areaPhysics = new b2World(gravityScale);
	areaPhysics->SetAllowSleeping(true);
	areaPhysics->SetAutoClearForces(false);
	listener = new ContactListener();
	areaPhysics->SetContactListener(listener);

	fixedTimestepAccum = 0;
	fixedTimestepAccumRatio = 0;
}

void GameArea::AddEntity(Entity* e) {
	entityManager->AddEntity(e);
	debugDraw->AddEntityRef(e);
}

void GameArea::SetPlayer(Player* p) {
	Vector2 dim = p->GetAvgPixelDimensions();
	Vector2 sD = graphics->GetScreenDimensions();

	player = p;
	player->SetInputQueuePtr(entityManager->GetInputQueue());
	player->SetEventsToFirePtr(entityManager->GetEventsToFire());
	player->UpdateScreenPosition(0);
}

Uint8 GameArea::CaptureInputEvents(SDL_Event* e){
	const Uint8* keyboardState;
	int keyCount = 0;
	Entity::InputEvent* newEvent;
	Entity::InputEvent* prevEvent = entityManager->GetNextInputEvent();
	Uint8 started = prevEvent != nullptr && prevEvent->inputType == JUMP ? 1 : 0;
	Uint64 elapsedInputTime = 0;

	while (SDL_PollEvent(e)) {
		if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_CLOSE)
			return 0;
	}

	keyboardState = SDL_GetKeyboardState(nullptr);
	
	if (keyboardState[SDL_SCANCODE_ESCAPE])
		keyCount++;
	if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
		if (!gravityEnabled)
			keyCount++;
	if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
		if (!gravityEnabled)
			keyCount++;
	if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
		keyCount++;
	if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
		keyCount++;
	if (keyboardState[SDL_SCANCODE_E])
		keyCount++;
	if (keyboardState[SDL_SCANCODE_BACKSPACE])
		keyCount++;
	if (keyboardState[SDL_SCANCODE_SPACE])
		if (gravityEnabled)
			keyCount++;
	if (keyboardState[SDL_SCANCODE_Q])
		if (gravityEnabled)
			keyCount++;
	if (keyboardState[SDL_SCANCODE_R])
		if (gravityEnabled)
			keyCount++;
	if (keyboardState[SDL_SCANCODE_F])
		if (gravityEnabled)
			keyCount++;

	if (keyCount == 0) {
		if (prevEvent)
			newEvent = prevEvent;
	}

	while (keyCount > 0){
		newEvent = new Entity::InputEvent();

		if (keyboardState[SDL_SCANCODE_ESCAPE])
			newEvent->inputType = MENU;
		if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
			if (!gravityEnabled)
				newEvent->inputType = WALK_UP;
		if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
			if (!gravityEnabled)
				newEvent->inputType = WALK_DOWN;
		if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
			newEvent->inputType = WALK_RIGHT;
		if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
			newEvent->inputType = WALK_LEFT;
		if (keyboardState[SDL_SCANCODE_E])
			newEvent->inputType = INTERACT;
		if (keyboardState[SDL_SCANCODE_BACKSPACE])
			newEvent->inputType = DECLINE;
		if (keyboardState[SDL_SCANCODE_SPACE] && !started) {
			if (gravityEnabled){
				newEvent->inputType = JUMP;
				started = 1;
			}
		}
		if (keyboardState[SDL_SCANCODE_Q])
			if (gravityEnabled)
				newEvent->inputType = PUNCH;
		if (keyboardState[SDL_SCANCODE_R])
			if (gravityEnabled)
				newEvent->inputType = SHOOT;
		if (keyboardState[SDL_SCANCODE_F])
			if (gravityEnabled)
				newEvent->inputType = KICK;

		newEvent->keyDown = 1;
		if (e->key.state == SDL_RELEASED)
			newEvent->keyDown = 0;
		newEvent->keyCount = keyCount;
		newEvent->prevEvent = prevEvent;
		newEvent->gravity = gravityEnabled;
		newEvent->e = e;
		newEvent->msSinceLastInput = (prevEvent != nullptr) ? SDL_GetTicks() - prevEvent->msSinceLastInput : SDL_GetTicks();
		newEvent->repeat = e->key.repeat ? 1 : 0;

		entityManager->PushBackInputEvent(newEvent);

		prevEvent = newEvent;

		keyCount--;
	}

	return 1;
}
