#pragma once
#include <gamearea.h>
#include <player.h>
#include <debugdraw.h>

class GameWorld {
private:
	Player*						player;

	std::shared_ptr<Graphics>	graphicsPtr;

	std::vector<GameArea*>*		areas;
	GameArea*					currentArea;
	
	void						PlayerPhysicsInit(b2World* physicsArea);

public:
	GameWorld();

	~GameWorld();

	inline std::shared_ptr<Graphics> GetGraphics() { return graphicsPtr; }

	void EnableDebugDraw();

	void InitTestArea();

	bool GameLoop(float & accumulator);
};