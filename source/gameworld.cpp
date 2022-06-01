#include "gameworld.h"

void GameWorld::PlayerPhysicsInit(b2World* physicsArea)
{
	// Actor
	{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.position.Set(0, -SCALED_HEIGHT/2);
		player->SetBody(physicsArea->CreateBody(&bd));

		b2PolygonShape shape;
		shape.SetAsBox(player->GetWorldDimensions().x / 2, player->GetWorldDimensions().y / 2);
		player->GetBody()->CreateFixture(&shape, 5.0f);

		b2PolygonShape jumpBox;
		b2Vec2 d = player->GetWorldDimensions();
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
	graphicsPtr->SetCurrentTime(SDL_GetTicks());

	if (!graphicsPtr->GetRenderer()) {
		std::cerr << "Renderer is NULL!" << std::endl;

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
}

void GameWorld::EnableDebugDraw() {
	for (auto itArea = areas->begin(); itArea != areas->end(); itArea++) {
		EntityManager* em = (*itArea)->GetEntityManager();
	}
}

void GameWorld::InitTestArea() {
	areas->push_back(new GameArea(areas->size(), b2Vec2(0.0f, .98f), graphicsPtr));

	currentArea = areas->at(0);
	currentArea->SetActive(1);

	currentArea->SetPlayer(player);

	PlayerPhysicsInit(currentArea->GetWorldPtr());
	
	EnableDebugDraw();
}

bool GameWorld::GameLoop(double &accumulator) {
	double newTime = graphicsPtr->GetGameTime();
	double frameTime = newTime - graphicsPtr->GetCurrentTime();

	if (frameTime > 0.25)
		frameTime = 0.25;

	graphicsPtr->SetCurrentTime(newTime);

	SDL_RenderClear(graphicsPtr->GetRenderer());
			
	SDL_PumpEvents();
	SDL_PollEvent(&currentEvent);

	if (currentEvent.type == SDL_QUIT)
		return true;

	accumulator += frameTime;

	currentArea->AreaThink(&currentEvent);

	while (accumulator >= DELTA_TIME) {
		currentArea->AreaUpdate();
		
		currentArea->PhysicsStep();

		accumulator -= DELTA_TIME;
	}

	if (accumulator < FRAME_DELAY)
	{
		SDL_Delay(FRAME_DELAY - DELTA_TIME);
	}

	currentArea->AreaUpdate();

	currentArea->AreaDraw(accumulator / DELTA_TIME);

	SDL_RenderPresent(graphicsPtr->GetRenderer());

	return false;
}