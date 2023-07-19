#pragma once
#include <gamearea.h>
#include <player.h>

class GameWorld {
private:
	Player*						player;

	std::shared_ptr<Graphics>	graphicsPtr;

	std::vector<GameArea*>*		areas;
	GameArea*					currentArea;

public:
	GameWorld();

	~GameWorld();

	const std::shared_ptr<Graphics>& graphicsetGraphics() { return graphicsPtr; }

	void InitPlayerPhysics(b2Vec2 spawn);

	void GameLoop(float & accumulator);
};