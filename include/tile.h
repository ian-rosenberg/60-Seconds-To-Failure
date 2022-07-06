#pragma once
#include "vectortypes.h"
#include <box2d/box2d.h>
#include "animation.h"
#include "debugdraw.h"


const int VERTICES_PER_EDGE = 4;
const int MAX_EDGES = 4;

class Tile {
private:
	int							id;

	Vector2						pixelDimensions;
	b2Vec2						worldDimensions;

	Vector2						pixelPosition;
	b2Vec2						worldPosition;

	std::vector<b2EdgeShape*>	edges;

	b2Body*						physicsBody;
	b2FixtureDef*				fixtureDef;

	Animation*					animSprite;

	std::shared_ptr<Graphics>	graphicsRef;


	//Debug Drawing, null if not enabled
	DebugDraw*					debugDraw;
	


	int CreatePhysicsEdges(SDL_Surface* readSurface);

public:
	Tile(std::shared_ptr<Graphics> graphics);
	~Tile();

	void Draw();
};

class TileManager {
private:
	std::vector<std::vector<Tile*>*>*	tileMap;


	std::shared_ptr<Graphics>			graphicsRef;

public:
	TileManager(const char* filepath, std::shared_ptr<Graphics> graphics);
	~TileManager();
};
