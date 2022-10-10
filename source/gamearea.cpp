#include "gamearea.h"
#include "staticentity.h"

void GameArea::CreateTestArea() {
	float hDim = player->GetWorldDimensions().y;
	float fVal = 1.f;
	areaPhysics = new b2World(*gravityScale);
	areaPhysics->SetAllowSleeping(true);
	areaPhysics->SetAutoClearForces(false);

	listener = new ContactListener();
	areaPhysics->SetContactListener(listener);

	fixedTimestepAccum = 0;
	fixedTimestepAccumRatio = 0;

	// Ground
	{
		StaticEntity *se = new StaticEntity(graphics, graphics->GetScaledWidth(), hDim);
		se->SetActorName("Ground");
		se->CalculateAverageActorDimensions();
		
		b2BodyDef bd;
		b2Body* ground = areaPhysics->CreateBody(&bd);
		se->SetBody(ground);

		b2EdgeShape shape;
		shape.SetTwoSided(b2Vec2(-graphics->GetScaledWidth(), graphics->GetScaledHeight()/2 - hDim), b2Vec2(graphics->GetScaledWidth(), graphics->GetScaledHeight() / 2 - hDim));

		b2FixtureDef tpd = {};
		tpd.friction = fVal;
		tpd.shape = new b2EdgeShape(shape);

		b2Fixture* f = ground->CreateFixture(&tpd);
		se->SetStaticTriggerFixture(f);
		se->CalculateAverageActorDimensions();

		entityManager->AddEntity(se);
	}

	// Platform
	{
		StaticEntity* se = new StaticEntity(graphics, graphics->GetScaledWidth() / 4.0f, 1.0f);
		se->SetActorName("Platform");
		b2BodyDef bd;
		bd.position.Set(-graphics->GetScaledWidth() / 8.0f, -graphics->GetScaledHeight()/4);
		b2Body* body = areaPhysics->CreateBody(&bd);
		se->SetBody(body);

		b2PolygonShape shape;
		shape.SetAsBox(graphics->GetScaledWidth() / 8, 0.5f);

		b2FixtureDef tpd = {};
		tpd.friction = fVal;
		tpd.shape =  new b2PolygonShape(shape);

		b2Fixture* f = body->CreateFixture(&tpd);
		se->SetStaticTriggerFixture(f);

		entityManager->AddEntity(se);
	}
}

GameArea::GameArea(int ID, b2Vec2 grav, std::shared_ptr<Graphics> g) {
	id = ID;
	player = NULL;
	entityManager = new EntityManager(1, g);//enabling debug draw with parameter, renderer
	gravityScale = new b2Vec2(grav);
	gravityEnabled = gravityScale->y != 0 || gravityScale->x != 0;
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	graphics = std::shared_ptr<Graphics>(g);
	//perlinNoiseMap = new PerlinNoise(g);
	//perlinNoiseMap->PerlinNoise2D();
	tileManager = nullptr;
}

GameArea::~GameArea() {
	graphics = nullptr;
	delete gravityScale;
	delete entityManager;
	delete areaPhysics;
	delete listener;
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
	entityManager->EntityUpdateAll(graphics->GetFrameDeltaTime());
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

	for (b2Body* b = areaPhysics->GetBodyList(); b != NULL; b = b->GetNext())
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
	for (b2Body* b = areaPhysics->GetBodyList(); b != NULL; b = b->GetNext())
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

void GameArea::AreaDraw(double accumulator) {
	if (!active)
		return;
	tileManager->DrawMap(Vector2(0,0));
	entityManager->EntityDrawAll(accumulator);
}

void GameArea::AddEntity(Entity* e) {
	entityManager->AddEntity(e);
}

void GameArea::SetPlayer(Player* p) {
	Vector2 dim = p->GetAvgPixelDimensions();
	player = p;
	player->SetInputQueuePtr(entityManager->GetInputQueue());
	player->SetEventsToFirePtr(entityManager->GetEventsToFire());

	CreateTestArea();

	tileManager = new TileManager("", graphics, areaPhysics, dim);
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