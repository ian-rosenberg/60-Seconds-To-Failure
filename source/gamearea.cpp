#include "gamearea.h"
#include "staticentity.h"

void GameArea::CreateTestArea() {
	float hDim = player->GetWorldDimensions().y;
	float fVal = 0.01f;
	areaPhysics = new b2World(*gravityScale);

	listener = new ContactListener();
	areaPhysics->SetContactListener(listener);

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
	inputDriver = std::make_shared<InputDriver>();
	entityManager = new EntityManager(inputDriver, 1);//enabling debug draw with parameter, renderer
	gravityScale = new b2Vec2(grav);
	gravityEnabled = gravityScale->y != 0 || gravityScale->x != 0;
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	graphics = g;
}

GameArea::~GameArea() {
	delete gravityScale;
	delete entityManager;
	delete areaPhysics;
	delete listener;
}

void GameArea::AreaThink(SDL_Event *e) {
	InputEvent* pEvent = nullptr;

	if (!active)
		return;


	pEvent = inputDriver->GetNextInputEvent();
	if (e->key.keysym.scancode != SDL_SCANCODE_UNKNOWN)
		CreateInputEvent(e, pEvent);
	//else
		//std::cout << "NO INPUT" << std::endl;

	entityManager->EntityThinkAll();
}

void GameArea::AreaUpdate() {
	if (!active)
		return;

	entityManager->EntityUpdateAll(graphics->GetFrameDeltaTime());
}

void GameArea::PhysicsStep() {
	areaPhysics->Step(timeStep, velocityIterations, positionIterations);

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

	CreateTestArea();
}

void GameArea::CreateInputEvent(SDL_Event* e, InputEvent* prevInput)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	InputEvent* newEvent = new InputEvent();
	InputState s = IDLE;
	InputType t = NONE;


	if(state[SDL_SCANCODE_ESCAPE])
		t = MENU;

	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
		if (!gravityEnabled)
			t = WALK_UP;
		else
			t = NONE;
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
		if (!gravityEnabled)
			t = WALK_DOWN;
		else
			t = NONE;

	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
		t = WALK_RIGHT;
	else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
		t = WALK_LEFT;
	else
		t = NONE;

	if (state[SDL_SCANCODE_E])
		t = INTERACT;

	if (state[SDL_SCANCODE_BACKSPACE])
		t = DECLINE;

	if (state[SDL_SCANCODE_SPACE])
		if (gravityEnabled)
			t = JUMP;
		else
			t = NONE;

	if (state[SDL_SCANCODE_Q])
		if (gravityEnabled)
			t = PUNCH;
		else
			t = NONE;

	if (state[SDL_SCANCODE_R])
		if (gravityEnabled)
			t = SHOOT;
		else
			t = NONE;

	if (state[SDL_SCANCODE_F])
		if (gravityEnabled)
			t = KICK;
		else
			t = NONE;

	if (t == WALK_UP || t == WALK_RIGHT || t == WALK_DOWN || t == WALK_LEFT) {
		if (prevInput && prevInput->state == PRESSED && prevInput->inputType == t)
			s = HOLD;
		else
			s = PRESSED;
	}
	else if (t == JUMP)
		s = PRESSED;
	else
		s = RELEASED;

	std::cout << s << "\t" << t << std::endl;

	newEvent->e = e;
	newEvent->inputType = t;
	newEvent->state = s;
	newEvent->msSinceLastInput = (prevInput != nullptr) ? SDL_GetTicks() - prevInput->msSinceLastInput : SDL_GetTicks();

	inputDriver->PushBackInputEvent(newEvent);

	if (s == HOLD)
		inputDriver->PushBackHeldEvent(newEvent);
}