#include "gameworld.h"

GameWorld::GameWorld() {
	areas = nullptr;
	player = nullptr;
	currentArea = nullptr;
	graphicsPtr = std::shared_ptr<Graphics>(new Graphics());
	graphicsPtr->SetOldTime();

	if (!graphicsPtr->GetRenderer()) {
		std::cerr << "Renderer is nullptr!" << std::endl;
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

	delete areas;

	graphicsPtr.reset();
	graphicsPtr = nullptr;
}

void GameWorld::InitTestArea() {
	
	areas->push_back(
		new GameArea(areas->size(),
			b2Vec2(0.0f,
				10.f * PIX_IN_MET),
			graphicsPtr,
			player->GetAvgPixelDimensions()));

	currentArea = areas->at(0);

	// Actor
	{
		b2Vec2 pos = currentArea->FindSpawnPointFromLeft();
		b2Vec2 d = player->GetWorldDimensions();
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position.Set(pos.x,pos.y);
		player->SetBody(currentArea->GetWorldPtr()->CreateBody(&bd));

		b2FixtureDef fd;
		b2CircleShape shape;

		shape.m_radius = d.x / 4;
		shape.m_p = b2Vec2(0, d.y / 4);

		fd.shape = &shape;
		fd.density = 10.f;
		fd.friction = .9f;

		player->GetBody()->CreateFixture(&fd);

		b2PolygonShape jumpBox;
		b2Vec2 verts[] = {
			b2Vec2(-d.x * 0.25, d.y * 0.25),
			b2Vec2(d.x * 0.25, d.y * 0.25),
			b2Vec2(d.x * 0.25, d.y * 0.55),
			b2Vec2(-d.x * 0.25, d.y * 0.55)
		};

		jumpBox.Set(verts, 4);

		b2FixtureDef jd;
		jd.shape = &jumpBox;
		jd.isSensor = true;
		player->SetJumpTrigger(player->GetBody()->CreateFixture(&jd));
	}

	player->SetGravityEnabled(currentArea->GetGravityScale().y != 0 ? 1 : 0);

	currentArea->SetActive(1);

	currentArea->SetPlayer(player);
	currentArea->AddEntity(player);
}

void GameWorld::GameLoop(float & accumulator) {
	SDL_Event currentEvent;
	float frameTime;

	SDL_PollEvent(&currentEvent);

	while (currentEvent.type != SDL_QUIT) {
		SDL_RenderClear(graphicsPtr->GetRenderer());
		graphicsPtr->SetOldTime();
		graphicsPtr->SetNewTime(SDL_GetTicks64());
		frameTime = graphicsPtr->GetFrameDeltaTime() / MS;

		accumulator += (frameTime > 0.25f ? 0.25f : frameTime);

		if (currentArea->CaptureInputEvents(&currentEvent) < 1)
			break;

		currentArea->AreaThink();
		
		while (accumulator >= DELTA_TIME) {
			currentArea->AreaUpdate();

			currentArea->PhysicsSteps(DELTA_TIME);

			accumulator -= DELTA_TIME;
		}
		graphicsPtr->SetAccumulatorTime(accumulator / DELTA_TIME);
		currentArea->AreaDraw();

		SDL_RenderPresent(graphicsPtr->GetRenderer());
		
		//if (graphicsPtr->GetFrameDeltaTime() < DELTA_TIME * MS)
			//SDL_Delay(DELTA_TIME * MS - graphicsPtr->GetFrameDeltaTime());
	}

	return;
}