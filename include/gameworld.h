#pragma once
#include <gamearea.h>
#include <player.h>

class GameWorld {
private:
	Player*						player;

	SDL_Renderer*				renderer;

	std::vector<GameArea*>*		areas;
	GameArea*					currentArea;
	
	void						PlayerPhysicsInit(b2World* physicsArea);

public:
	GameWorld(SDL_Renderer* ren);

	~GameWorld();
		
	void Update();

	void InitTestArea();

	void Draw();
};