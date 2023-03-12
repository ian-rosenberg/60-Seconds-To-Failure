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

	std::shared_ptr<Graphics> GetGraphics() { return graphicsPtr; }

	void InitTestArea();

	void GameLoop(float & accumulator);
};