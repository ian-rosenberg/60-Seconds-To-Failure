#pragma once

#include <map>
#include <entity.h>
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <player.h>
#include <contactlistener.h>
#include <memory>
#include <inputdriver.h>

class GameArea {
private:
	int								id;

	Uint8							active;
	Uint8							gravityEnabled;

	std::shared_ptr<Graphics>		graphics;
	std::shared_ptr<InputDriver>	inputDriver;

	Player*							player;

	EntityManager*					entityManager;

	b2Vec2*							gravityScale;
	b2World*						areaPhysics;

	void							CreateTestArea();
	//Test ground vars
	b2Body*							ground;
	b2BodyDef						groundBD;
	b2PolygonShape					groundBox;

	b2Body*							testPlatform;
	float							testPlatformBottom;
	float							testPlatformTop;

	ContactListener					*listener;

	const double					timeStep = 1 / 60.0f;
	const int32						velocityIterations = 6;
	const int32						positionIterations = 2;

public:
	GameArea(int ID, b2Vec2 grav, std::shared_ptr<Graphics> g);

	~GameArea();

	inline b2World* GetWorldPtr() { return areaPhysics; }

	void AddEntity(Entity* e);

	void SetPlayer(Player* p);

	void CreateInputEvent(SDL_Event* e, InputEvent* prevInput);

	void AreaThink(SDL_Event* e);

	void AreaUpdate();

	void PhysicsStep();

	void AreaDraw(double accumulator);

	inline EntityManager* GetEntityManager() { return entityManager; }

	inline Uint8 IsActive() { return active; }

	inline void SetActive(Uint8 flag) { active = flag; }

	inline b2Vec2* GetGravityScale() { return gravityScale; }
};