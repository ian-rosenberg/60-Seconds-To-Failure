#include "gameworld.h"

void GameWorld::PlayerPhysicsInit(b2World* physicsArea)
{
	// Actor
	{
		b2Vec2 d = player->GetWorldDimensions();
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position.Set(0, -graphicsPtr->GetScaledHeight() / 2);
		player->SetBody(physicsArea->CreateBody(&bd));

		b2FixtureDef fd;
		b2CircleShape shape;

		shape.m_radius = d.x / 4;
		shape.m_p = b2Vec2(0, d.y/4);

		fd.shape = &shape;
		fd.friction = 1.f;
		fd.density = 10;
		
		player->GetBody()->CreateFixture(&fd);

		b2PolygonShape jumpBox;
		b2Vec2 verts[] = {
			b2Vec2(-d.x * 0.25, d.y / 4),
			b2Vec2(d.x * 0.25, d.y / 4),
			b2Vec2(d.x * 0.25, d.y * 0.55),
			b2Vec2(-d.x * 0.25, d.y * 0.55)
		};
		
		jumpBox.Set(verts, 4);

		b2FixtureDef jd;
		jd.shape = &jumpBox;
		jd.isSensor = true;
		player->SetJumpTrigger(player->GetBody()->CreateFixture(&jd));
	}

	player->SetGravityEnabled(currentArea->GetGravityScale()->y != 0 ? 1 : 0);

	currentArea->AddEntity(player);
}

GameWorld::GameWorld() {
	areas = NULL;
	player = NULL;
	currentArea = NULL;
	graphicsPtr = std::make_shared<Graphics>();
	graphicsPtr->SetOldTime();

	if (!graphicsPtr->GetRenderer()) {
		std::cerr << "Renderer is NULL!" << std::endl;
		graphicsPtr = nullptr;
		return;
	}
	
	areas = new std::vector<GameArea*>();
	player = new Player(graphicsPtr);

	InitTestArea();
}

GameWorld::~GameWorld() {
	if (areas) {
		while (!areas->empty()) {
			GameArea* a = areas->back();
			areas->pop_back();
			delete a;
		}
	}

	graphicsPtr = nullptr;
}

void GameWorld::EnableDebugDraw() {
	for (auto itArea = areas->begin(); itArea != areas->end(); itArea++) {
		EntityManager* em = (*itArea)->GetEntityManager();
	}
}

void GameWorld::InitTestArea() {
	areas->push_back(new GameArea(areas->size(), b2Vec2(0.0f, 9.81 * MET_TO_PIX), graphicsPtr));

	currentArea = areas->at(0);
	currentArea->SetActive(1);

	currentArea->SetPlayer(player);

	PlayerPhysicsInit(currentArea->GetWorldPtr());
	
	EnableDebugDraw();
}

bool GameWorld::GameLoop(float & accumulator) {
	SDL_Event currentEvent;
	SDL_RenderClear(graphicsPtr->GetRenderer());
	graphicsPtr->SetOldTime();
	graphicsPtr->SetNewTime(SDL_GetTicks64());
	float frameTime = graphicsPtr->GetFrameDeltaTime() / MS;

	accumulator += (frameTime > 0.25f ? 0.25f : frameTime);

	if (currentArea->CaptureInputEvents(&currentEvent) < 1)
		return true;

	currentArea->AreaThink();

	while (accumulator >= DELTA_TIME) {	
		currentArea->AreaUpdate();
		
		currentArea->PhysicsSteps(DELTA_TIME);

		accumulator -= DELTA_TIME;
	}

	currentArea->AreaDraw(accumulator / DELTA_TIME);

	SDL_RenderPresent(graphicsPtr->GetRenderer());
	
	//if (graphicsPtr->GetFrameDeltaTime() < DELTA_TIME * MS)
		//SDL_Delay(DELTA_TIME * MS - graphicsPtr->GetFrameDeltaTime());

	return false;
}