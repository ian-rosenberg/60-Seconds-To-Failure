#pragma once
#include "vectortypes.h"
#include <box2d/box2d.h>
#include "animation.h"
#include "debugdraw.h"


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;

class Tile {
private:
	int							id;

	Vector2						pixelDimensions;
	b2Vec2						worldDimensions;

	Vector2						pixelPosition;
	b2Vec2						worldPosition;

	b2ChainShape*				chainShape;

	b2Body*						physicsBody;
	b2FixtureDef*				fixtureDef;

	Animation*					animSprite;

	std::shared_ptr<Graphics>	graphicsRef;

	//Debug Drawing, null if not enabled
	DebugDraw*					debugDraw;


	std::vector<std::vector<SDL_Color>> GetPixelDataFromFile(const char* file);
	Uint32								GetPixel(SDL_Surface* surface, int x, int y);

public:
	Tile(std::shared_ptr<Graphics> graphics, Vector2 pDim, DebugDraw* debugDraw);
	~Tile();

	inline void							SetBody(b2Body* b) { physicsBody = b; }
	void								Draw();
	std::vector<b2Vec2>					CreatePhysicsEdges();
};

class TileManager {
private:
	std::vector<std::vector<Tile*>>     tileMap;

	std::shared_ptr<Graphics>			graphicsRef;

	b2World*							physics;
	
public:
	TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDimensions, DebugDraw* ddRef);
	
	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset);
};
