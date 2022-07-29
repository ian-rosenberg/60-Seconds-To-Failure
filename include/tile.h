#pragma once
#include <unordered_map>
#include <utility>

#include "vectortypes.h"
#include <box2d/box2d.h>
#include "animation.h"
#include "debugdraw.h"


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;

class Tile {
private:
	int							id;
	int							direction;

	Vector2						pixelDimensions;
	b2Vec2						worldDimensions;

	Vector2						pixelPosition;
	b2Vec2						worldPosition;

	b2Body*						physicsBody;

	Animation*					animSprite;

	std::shared_ptr<Graphics>	graphicsRef;

	//Debug Drawing, null if not enabled
	DebugDraw*					debugDraw;


	std::vector<std::vector<SDL_Color>> GetPixelDataFromFile(const char* file);
	Uint32								GetPixel(SDL_Surface* surface, int x, int y);

public:
	Tile();
	Tile(Sprite* s, DebugDraw* debugDraw, Vector2 gridPosition, Vector2 pDim, int dir, std::shared_ptr<Graphics> g);
	Tile(const Tile& oldTile);
	
	~Tile();

	inline void							SetBody(b2Body* b) { physicsBody = b; }
	void								Draw();
	std::vector<std::vector<b2Vec2>>	CreatePhysicsEdges();
	void								TilePhysicsInit(b2World* world, Vector2 p);
};

class TileManager {
private:
	std::unordered_map<std::string, std::vector<Tile*>>	tileTypes;
	std::vector<std::vector<Tile*>>						tileMap;

	std::shared_ptr<Graphics>							graphicsRef;

	b2World*											physics;
	
	Vector2												playerDimensions;


	std::pair<std::string, std::vector<Tile*>>			TileParseTypesFromJSON(std::string json);
	
public:
	TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDimensions);
	
	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset);
};
