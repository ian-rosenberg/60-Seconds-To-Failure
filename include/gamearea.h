#pragma once

#include <map>
#include <memory>

#include <entity.h>
#include <player.h>
#include <tile.h>
#include <contactlistener.h>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>

class GameArea {
private:
	int								id;

	Uint8							active;
	Uint8							gravityEnabled;

	std::shared_ptr<Graphics>		graphics;

	Player*							player;

	EntityManager*					entityManager;

	b2Vec2*							gravityScale;
	b2World*						areaPhysics;

	TileManager*					tileManager;

	void							CreateTestArea();
	//Test ground vars
	b2Body*							ground;
	b2BodyDef						groundBD;
	b2PolygonShape					groundBox;

	b2Body*							testPlatform;
	float							testPlatformBottom;
	float							testPlatformTop;

	ContactListener					*listener;

	double							fixedTimestepAccum;
	double							fixedTimestepAccumRatio;
	const double					timeStep = 1 / 30.0f;
	const int32						velocityIterations = 6;
	const int32						positionIterations = 2;

public:
	GameArea(int ID, b2Vec2 grav, std::shared_ptr<Graphics> g);

	~GameArea();

	inline b2World* GetWorldPtr() { return areaPhysics; }

	void AddEntity(Entity* e);

	void SetPlayer(Player* p);

	Uint8 CaptureInputEvents(SDL_Event* e);

	void AreaThink();

	void AreaUpdate();

	void PhysicsSteps(float deltaTime);

	void SinglePhysicsStep(float deltaTime);

	void SmoothPhysicsStates();

	void ResetSmoothStates();

	void AreaDraw(double accumulator);

	inline EntityManager* GetEntityManager() { return entityManager; }

	inline Uint8 IsActive() { return active; }

	inline void SetActive(Uint8 flag) { active = flag; }

	inline b2Vec2* GetGravityScale() { return gravityScale; }
};