#pragma once
#include <unordered_map>
#include <utility>

#include "vectortypes.h"
#include <box2d/box2d.h>
#include "animation.h"
#include "debugdraw.h"


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;

enum TileCapping : unsigned long{
	None = 0,
	North = 1,
	East = 2,
	South = 4,
	West = 8
};

class Tile {
private:
	int							id;
	int							direction;

	SDL_RendererFlip			flipFlags;

	Vector2						pixelDimensions;
	b2Vec2						worldDimensions;

	Vector2						pixelPosition;
	b2Vec2						worldPosition;

	b2Body*						physicsBody;
	TileCapping					capDirection;

	Animation*					animSprite;

	std::shared_ptr<Graphics>	graphicsRef;

	//Debug Drawing, null if not enabled
	DebugDraw*					debugDraw;


	std::vector<std::vector<SDL_Color>> GetPixelDataFromFile(const char* file);
	Uint32								GetPixel(SDL_Surface* surface, int x, int y);

public:
	Tile();
	Tile(Sprite* s, DebugDraw* debugDraw, Vector2 gridPosition, Vector2 pDim, int dir, std::shared_ptr<Graphics> g);
	Tile(Tile* oldTile);
	
	~Tile();

	void							SetBody(b2Body* b) { physicsBody = b; }
	void								Draw();
	std::vector<std::vector<b2Vec2>>	CreatePhysicsEdges();
	void								TilePhysicsInit(b2World* world, Vector2 p, SDL_RendererFlip flip);
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
