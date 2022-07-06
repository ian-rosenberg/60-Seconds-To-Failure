#include "gamearea.h"
#include "staticentity.h"

void GameArea::CreateTestArea() {
	float hDim = player->GetWorldDimensions().y;
	float fVal = 0.9f;
	areaPhysics = new b2World(*gravityScale);

	listener = new ContactListener();
	areaPhysics->SetContactListener(listener);
	areaPhysics->SetAllowSleeping(false);

	// Ground
	{
		StaticEntity *se = new StaticEntity(graphics, SCALED_WIDTH, hDim);
		se->SetActorName("Ground");
		
		b2BodyDef bd;
		b2Body* ground = areaPhysics->CreateBody(&bd);
		se->SetBody(ground);

		b2EdgeShape shape;
		shape.SetTwoSided(b2Vec2(-SCALED_WIDTH/2, SCALED_HEIGHT/2 - hDim), b2Vec2(SCALED_WIDTH/2, SCALED_HEIGHT / 2 - hDim));

		b2FixtureDef tpd = {};
		tpd.friction = fVal;
		tpd.shape = new b2EdgeShape(shape);

		b2Fixture* f = ground->CreateFixture(&tpd);
		se->SetStaticTriggerFixture(f);

		entityManager->AddEntity(se);
	}

	// Platform
	{
		StaticEntity* se = new StaticEntity(graphics, SCALED_WIDTH / 4.0f, 1.0f);
		se->SetActorName("Platform");
		b2BodyDef bd;
		bd.position.Set(-SCALED_WIDTH / 8.0f, -SCALED_HEIGHT/4);
		b2Body* body = areaPhysics->CreateBody(&bd);
		se->SetBody(body);

		b2PolygonShape shape;
		shape.SetAsBox(SCALED_WIDTH / 8, 0.5f);

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
	entityManager = new EntityManager(1);//enabling debug draw with parameter, renderer
	gravityScale = new b2Vec2(grav);
	gravityEnabled = gravityScale->y != 0 || gravityScale->x != 0;
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	graphics = g;
	tileManager = new TileManager("", graphics);
}

GameArea::~GameArea() {
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

	entityManager->InputUpdate();
	entityManager->EntityUpdateAll(graphics->GetFrameDeltaTime());

	for (auto c = listener->_contacts.begin(); c != listener->_contacts.end(); c++) {
		Contact contact = *c;

		if (contact.fixtureA == player->GetJumpTrigger()
			|| contact.fixtureB == player->GetJumpTrigger()) {
			if (!player->IsGrounded()) {
				player->ToggleGrounded(true);
			}
		}
	}
}

void GameArea::PhysicsStep() {
	areaPhysics->Step(timeStep, velocityIterations, positionIterations);
}

void GameArea::AreaDraw(double accumulator) {
	if (!active)
		return;

	entityManager->EntityDrawAll(accumulator);
}

void GameArea::AddEntity(Entity* e) {
	entityManager->AddEntity(e);
}

void GameArea::SetPlayer(Player* p) {
	player = p;
	player->SetInputQueuePtr(entityManager->GetInputQueue());
	player->SetEventsToFirePtr(entityManager->GetEventsToFire());

	CreateTestArea();
}

Uint8 GameArea::CaptureInputEvents(SDL_Event* e){
	const Uint8* keyboardState;
	int keyCount = 0;
	Entity::InputEvent* newEvent;
	Entity::InputEvent* prevEvent = entityManager->GetNextInputEvent();
	Uint8 started = prevEvent != nullptr ? 1 : 0;

	keyboardState = SDL_GetKeyboardState(&keyCount);

	while (SDL_PollEvent(e)) {
		if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_CLOSE)
			return 0;
	}
	
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
		if (keyboardState[SDL_SCANCODE_SPACE])
			if (gravityEnabled)
				newEvent->inputType = JUMP;
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

	if (keyCount == 0) {
		newEvent = new Entity::InputEvent();
		newEvent->keyDown = 0;
		newEvent->keyCount = keyCount;
		newEvent->prevEvent = prevEvent;
		newEvent->gravity = gravityEnabled;
		newEvent->e = e;
		newEvent->msSinceLastInput = (prevEvent != nullptr) ? SDL_GetTicks() - prevEvent->msSinceLastInput : SDL_GetTicks();
		newEvent->repeat = 0;

		entityManager->PushBackInputEvent(newEvent);
	}

	return 1;
}