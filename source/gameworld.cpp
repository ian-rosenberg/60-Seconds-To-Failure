#include "gameworld.h"

void GameWorld::PlayerPhysicsInit(b2World* physicsArea)
{
	// Actor
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.linearDamping = 0.3f;
		bd.position.Set(0, -SCALED_HEIGHT/2);
		player->SetBody(physicsArea->CreateBody(&bd));

		b2PolygonShape shape;
		shape.SetAsBox(player->GetWorldDimensions().x / 2, player->GetWorldDimensions().y / 2);
		player->GetBody()->CreateFixture(&shape, 100.0f);

		b2EdgeShape edge;
		b2Vec2 v1, v2, v3, v4;
		v1.Set(player->GetWorldDimensions().x / -2, -player->GetWorldDimensions().y);
		v2.Set(player->GetWorldDimensions().x / -2, player->GetWorldDimensions().y/2);
		v3.Set(player->GetWorldDimensions().x / 2, player->GetWorldDimensions().y/2);
		v4.Set(player->GetWorldDimensions().x / 2, -player->GetWorldDimensions().y);
		edge.SetOneSided(v1, v2, v3, v4);

		b2FixtureDef jd;
		jd.shape = &edge;
		jd.isSensor = true;
		player->SetJumpTrigger(player->GetBody()->CreateFixture(&jd));
	}

	currentArea->AddEntity(player);
}

GameWorld::GameWorld(SDL_Renderer* ren) {
	areas = NULL;
	player = NULL;
	currentArea = NULL;
	renderer = ren;

	if (!renderer) {
		std::cerr << "Renderer is NULL!" << std::endl;

		return;
	}
	
	areas = new std::vector<GameArea*>();
	player = new Player(renderer);

	InitTestArea();
}

GameWorld::~GameWorld() {
	//loop through and delete areas
	GameArea* area = NULL; 

	if (areas) {
		while (!areas->empty()) {
			GameArea* a = areas->back();
			areas->pop_back();
			delete a;
		}
	}
}

void GameWorld::EnableDebugDraw() {
	for (auto itArea = areas->begin(); itArea != areas->end(); itArea++) {
		EntityManager* em = (*itArea)->GetEntityManager();
	}
}

void GameWorld::Update() {
	currentArea->Update();
}

void GameWorld::InitTestArea() {
	areas->push_back(new GameArea(areas->size(), b2Vec2(0.0f, 9.8f), renderer));

	currentArea = areas->at(0);

	currentArea->SetPlayer(player);

	PlayerPhysicsInit(currentArea->GetWorldPtr());
	
	EnableDebugDraw();
}