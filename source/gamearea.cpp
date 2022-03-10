#include "gamearea.h"

void GameArea::CreateTestArea() {
	areaPhysics = new b2World(*gravityScale);

	// Ground
	{
		b2BodyDef bd;
		b2Body* ground = areaPhysics->CreateBody(&bd);

		b2EdgeShape shape;
		shape.SetTwoSided(b2Vec2(-WIDTH, 0.0f), b2Vec2(WIDTH, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
	}

	// Platform
	{
		b2BodyDef bd;
		bd.position.Set(0.0f, 10.0f);
		b2Body* body = areaPhysics->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(3.0f, 0.5f);
		b2Fixture* testPlatform = body->CreateFixture(&shape, 0.0f);

		testPlatformBottom = HEIGHT;
		testPlatformTop = HEIGHT + 1.0f;
	}
}

GameArea::GameArea(int ID, b2Vec2 grav) {
	id = ID;
	player = NULL;
	entityManager = new EntityManager();
	gravityScale = new b2Vec2(grav);
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
}

GameArea::~GameArea() {
	entityManager->DeleteEntity(player);
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