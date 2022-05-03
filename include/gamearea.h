#pragma once

#include <map>
#include <entity.h>
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <player.h>

class GameArea {
private:
	int							id;

	SDL_Renderer*				ren;

	Player*						player;

	EntityManager*				entityManager;

	b2Vec2*						gravityScale;
	b2World*					areaPhysics;

	void						CreateTestArea();
	//Test ground vars
	b2Body*						ground;
	b2BodyDef					groundBD;
	b2PolygonShape				groundBox;

	b2Body*						testPlatform;
	float						testPlatformBottom;
	float						testPlatformTop;


	const float					timeStep = 1.0f / 60.0f;
	const int32					velocityIterations = 6;
	const int32					positionIterations = 2;

public:
	GameArea(int ID, b2Vec2 grav, SDL_Renderer* r);

	~GameArea();

	void Update();

	inline b2World* GetWorldPtr() { return areaPhysics; }

	void AddEntity(Entity* e);

	void SetPlayer(Player* p);

	inline EntityManager* GetEntityManager() { return entityManager; }
};