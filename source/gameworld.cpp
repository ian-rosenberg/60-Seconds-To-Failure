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
		fd.friction = 0.5f;
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

	graphicsPtr.reset();
}

void GameWorld::EnableDebugDraw() {
	for (auto itArea = areas->begin(); itArea != areas->end(); itArea++) {
		EntityManager* em = (*itArea)->GetEntityManager();
	}
}

void GameWorld::InitTestArea() {
	areas->push_back(new GameArea(areas->size(), b2Vec2(0.0f, 0.0025), graphicsPtr));

	currentArea = areas->at(0);
	currentArea->SetActive(1);

	currentArea->SetPlayer(player);

	PlayerPhysicsInit(currentArea->GetWorldPtr());
	
	EnableDebugDraw();
}

bool GameWorld::GameLoop(float & accumulator) {
	SDL_Event currentEvent;
	float newTime = graphicsPtr->GetGameTime();
	float frameTime = newTime - graphicsPtr->GetCurrentTime();
	float aStart = SDL_GetTicks64(), aEnd;

	SDL_RenderClear(graphicsPtr->GetRenderer());

	if (frameTime > 0.25f)
		frameTime = 0.25f;
	graphicsPtr->SetCurrentTime(newTime);

	accumulator += frameTime;

	SDL_PumpEvents();

	if (currentArea->CaptureInputEvents(&currentEvent) < 1)
		return true;

	currentArea->AreaThink();

	while (accumulator >= DELTA_TIME) {	
		
		currentArea->PhysicsSteps(DELTA_TIME);

		accumulator -= DELTA_TIME;
	}

	if (accumulator < FRAME_DELAY)
	{
		SDL_Delay(FRAME_DELAY - DELTA_TIME);
	}

	currentArea->AreaDraw(accumulator / DELTA_TIME);

	SDL_RenderPresent(graphicsPtr->GetRenderer());

	aEnd = SDL_GetTicks64();

	std::cout << "frame time " << (aEnd - aStart) << std::endl;

	return false;
}