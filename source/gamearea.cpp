#include "gamearea.h"

void GameArea::CreateTestArea() {
	float hDim = player->GetWorldDimensions().y / 2;
	float fVal = 0.0f;
	areaPhysics = new b2World(*gravityScale);

	// Ground
	{
		b2BodyDef bd;
		b2Body* ground = areaPhysics->CreateBody(&bd);

		b2EdgeShape shape;
		shape.SetTwoSided(b2Vec2(-SCALED_WIDTH, SCALED_HEIGHT/2 - hDim), b2Vec2(SCALED_WIDTH, SCALED_HEIGHT / 2 - hDim));

		b2FixtureDef tpd = {};
		tpd.friction = fVal;
		tpd.shape = new b2EdgeShape(shape);

		b2Fixture* f = ground->CreateFixture(&tpd);	
	}

	

	// Platform
	{
		b2BodyDef bd;
		bd.position.Set(0.0f, -SCALED_HEIGHT/4);
		b2Body* body = areaPhysics->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(SCALED_WIDTH / 8, 0.5f);
		
		b2FixtureDef tpd = {};
		tpd.friction = fVal;
		tpd.shape =  new b2PolygonShape(shape);

		b2Fixture* f = body->CreateFixture(&tpd);
	}
}

GameArea::GameArea(int ID, b2Vec2 grav, SDL_Renderer* r) {
	id = ID;
	player = NULL;
	entityManager = new EntityManager();
	gravityScale = new b2Vec2(grav);
	areaPhysics = NULL;
	ground = {};
	testPlatform = {};
	testPlatformBottom = 0.0f;
	testPlatformTop = 0.0f;
	ren = r;
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

	//areaPhysics->DebugDraw();

	/*if (debugDraw) {
		Vector2 d = player->GetAverageActorDimensions();
		SDL_Rect r = {0,HEIGHT-d.y/2,WIDTH,25};
		SDL_Rect p = { 0.0f, -HEIGHT / 4, WIDTH / 4, 1.0f };
		SDL_Rect pr = { player->GetDrawPosition().x - player->GetAverageActorDimensions().x/2, player->GetDrawPosition().y - player->GetAverageActorDimensions().y/2, player->GetAverageActorDimensions().x, player->GetAverageActorDimensions().y };
		SDL_SetRenderDrawColor(ren, 0,255,0,255);
		SDL_RenderDrawRect(ren, &r);
		SDL_RenderDrawRect(ren, &pr);
		SDL_RenderDrawRect(ren, &p);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	}*/
		
}

void GameArea::AddEntity(Entity* e) {
	entityManager->AddEntity(e);
}

void GameArea::SetPlayer(Player* p) {
	player = p;

	CreateTestArea();
}