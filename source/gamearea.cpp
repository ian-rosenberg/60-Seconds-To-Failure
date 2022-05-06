#include "gamearea.h"
#include "staticentity.h"

void GameArea::CreateTestArea() {
	float hDim = player->GetWorldDimensions().y;
	float fVal = 0.0f;
	areaPhysics = new b2World(*gravityScale);

	// Ground
	{
		StaticEntity *se = new StaticEntity(ren, SCALED_WIDTH, hDim);
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
		StaticEntity* se = new StaticEntity(ren, SCALED_WIDTH/4.0f, 1.0f);
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

GameArea::GameArea(int ID, b2Vec2 grav, SDL_Renderer* r) {
	id = ID;
	player = NULL;
	entityManager = new EntityManager(1);//enabling debug draw with parameter, renderer
	gravityScale = new b2Vec2(grav);
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	ren = r;
}

GameArea::~GameArea() {
	delete entityManager;
	delete areaPhysics;
}

void GameArea::Update() {
	areaPhysics->Step(timeStep, velocityIterations, positionIterations);

	entityManager->EntityThinkAll();
	entityManager->EntityUpdateAll();
	entityManager->EntityDrawAll();	
}

void GameArea::AddEntity(Entity* e) {
	entityManager->AddEntity(e);
}

void GameArea::SetPlayer(Player* p) {
	player = p;

	CreateTestArea();
}