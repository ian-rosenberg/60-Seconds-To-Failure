#include "gameworld.h"

void GameWorld::PlayerPhysicsInit(b2World* physicsArea)
{
	// Actor
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position.Set(0, -SCALED_HEIGHT);
		player->SetBody(physicsArea->CreateBody(&bd));

		float radius = 0.5f;
		b2CircleShape shape;
		shape.m_radius = radius;
		player->GetBody()->CreateFixture(&shape, 20.0f);
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
	player = new Player(ren);
	renderer = ren;

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

void GameWorld::Update() {
	currentArea->Update();
}

void GameWorld::InitTestArea() {
	areas->push_back(new GameArea(areas->size(), b2Vec2(0.0f, 9.8f)));

	currentArea = areas->at(0);

	currentArea->SetPlayer(player);

	PlayerPhysicsInit(currentArea->GetWorldPtr());
}