#pragma once
#include <unordered_map>
#include <utility>

#include "vectortypes.h"
#include <box2d/box2d.h>
#include "animation.h"
#include "debugdraw.h"


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;

enum Direction : unsigned short{
	None = 0,
	North = 1,
	East = 2,
	South = 4,
	West = 8
};

typedef struct TileSpriteSheet_S {
	std::string name;
	std::string filepath;
	Sprite* sheet;
	Uint8 margin;
	Uint8 spacing;
	Uint8 columns;
	Uint8 tileCount;
	Uint16 tileWidth;
	Uint16 tileHeight;
	Uint32 imageWidth;
	Uint32 imageHeight;
	Direction direction;
}TileSpriteSheet;

class Tile {
private:
	int														id;
	Direction												direction;

	SDL_RendererFlip										flipFlags;

	Vector2													pixelDimensions;
	b2Vec2													worldDimensions;

	Vector2													pixelPosition;
	b2Vec2													worldPosition;

	b2Vec2													worldCenter;
	Vector2													pixelCenter;

	b2Body*													physicsBody;
	Direction												capDirection;

	Animation*												animSprite;

	std::shared_ptr<Graphics>								graphicsRef;

	//Debug Drawing, null if not enabled					
	DebugDraw*												debugDraw;

	//Rotation in degrees for SDL2
	float													zRot;

	//Deferring chain and body creation til post rotation and flipping
	std::vector<std::pair<std::vector<b2Vec2>, bool>>		chainsAndCaps;

public:
	Tile();
	Tile(int id, Sprite* s, DebugDraw* debugDraw, Vector2 gridPosition, Vector2 pDim, Direction dir, std::shared_ptr<Graphics> g, float zRotation);
	Tile(const Tile &oldTile);
	
	~Tile();

	void													FlipChain(std::vector<b2Vec2>& chain);
	void													RotateChain(std::vector<b2Vec2>& chain, float angle);
	void													Draw(Vector2 cameraOffset);
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>>		CreatePhysicsEdges();
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>>		DecideCapping(std::vector<b2Vec2>& c1, std::vector<b2Vec2>& c2, std::vector<std::vector<SDL_Color>>& pixels, SDL_Rect r);
	void													TilePhysicsInit(b2World* world, Vector2 p);
	void													TileCreateBody(b2World* world);
	void													SetCappingDirection(Direction capping);
	void													SetSpriteDirection(Direction dir) { direction = dir; }
	void													SetSDL_RendererFlipFlags(SDL_RendererFlip flip) { flipFlags = flip; }
	void													FlipTileSprite();
};

class TileManager {
private:
	std::unordered_map<std::string, std::vector<Tile*>>	tileTypes;
	std::unordered_map<std::string, Sprite*>			typeSheets;

	std::vector<std::vector<Tile*>>						tileMap;

	std::shared_ptr<Graphics>							graphicsRef;

	b2World*											physics;
	
	Vector2												playerDimensions;
								
	void												TileParseTypesFromJSON(std::string json);
	
public:
	TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDimensions);
	
	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset);
};
