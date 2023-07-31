#pragma once
#include "drunkard.h"
#include "animation.h"

#include <unordered_map>
#include <utility>

#include <box2d/box2d.h>


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;
const int MAX_TUNNELS = 6;


enum Direction : uint16_t {
	North = 1 << 0,
	East = 1 << 2,
	South = 1 << 3,
	West = 1 << 4,
	None = 1 << 5,
};

enum TileLayer : uint16_t {
	Ground = 1 << 6,
	Hill = 1 << 7,
	Wall = 1 << 8,
	Platform = 1 << 9,
	Empty = 1 << 10
};

typedef struct TileConnection_S {
	int			xConnect;
	int			yConnect;
	TileLayer	layer;
	Direction	hillDirection;
}TileConnection;

typedef struct TileSpriteSheet_S {
	const char* name;
	const char* filepath;
	Sprite* sheet;
	uint8_t margin;
	uint8_t spacing;
	uint8_t columns;
	uint8_t tileCount;
	uint32_t tileWidth;
	uint32_t tileHeight;
	uint32_t imageWidth;
	uint32_t imageHeight;
	Direction direction;
}TileSpriteSheet;

class Tile {
private:
	int														id;
	Direction												direction;

	SDL_Color												debugColor;

	SDL_RendererFlip										flipFlags;

	Vector2													pixelDimensions;
	b2Vec2													worldDimensions;

	Vector2													pixelPosition;
	b2Vec2													worldPosition;

	b2Vec2													worldCenter;
	Vector2													pixelCenter;

	b2Body* physicsBody;
	Direction												capDirection;

	std::shared_ptr<Graphics>								graphicsRef;

	std::shared_ptr<Sprite>									spriteSheet;

	SDL_Rect												sourceRect;

	std::vector<b2Vec2>										topChain;
	std::vector<b2Vec2>										bottomChain;
	std::vector<b2Vec2>										eastCap;
	std::vector<b2Vec2>										westCap;

	b2Fixture* topFix;
	b2Fixture* bottomFix;
	b2Fixture* eastFix;
	b2Fixture* westFix;

	std::vector<TileConnection>								possibleConnections;
	TileLayer												tileLayers;
	Direction												hillOrientation;


	//Rotation in degrees for SDL2
	float													zRot;


public:
	Tile();
	Tile(int id, const std::shared_ptr<Sprite>& s, Vector2 gridPosition, Vector2 pDim, Direction dir, const std::shared_ptr<Graphics>& graphics, float zRotation, SDL_Rect srcRect);
	Tile(const Tile& oldTile);
	Tile& operator= (const Tile& other);

	~Tile();

	void													AddPossibleConnection(Vector2 v, TileLayer layer, Direction hillDir);
	void													RotateChain(std::vector<b2Vec2>& chain, float angle);
	void													Draw(Vector2 cameraOffset);
	void													CreatePhysicsEdges(std::vector<std::vector<SDL_Color>>& pixels, Vector2 playerDim);
	void													CreateTileBody(b2World* world, b2Vec2 tpg, b2Vec2 tng, b2Vec2 bpg, b2Vec2 bng);
	void													SetCappingDirection(Direction capping);
	void													DecideCapping(std::vector<std::vector<SDL_Color>>& pixels);
	void													TilePhysicsInit(b2World* world, Vector2 playerDim, std::vector<std::vector<SDL_Color>>& pixels);
	void													SetSpriteDirection(Direction dir) { direction = dir; }
	void													SetSDL_RendererFlipFlags(SDL_RendererFlip flip);
	void													SetTileLayer(TileLayer layers) { tileLayers = layers; }
	void													SetGridPosition(int col, int row);

	Vector2													GetPixelDimensions() { return pixelDimensions; }

	b2Body* GetBodyReference() { return physicsBody; }

	SDL_Color												GetDebugColor() { return debugColor; }

	b2Vec2													GetTopChainFirstVertex() { return topChain.front(); }
	b2Vec2													GetTopChainLastVertex() { return topChain.back(); }
	b2Vec2													GetBottomChainFirstVertex() { return bottomChain.front(); }
	b2Vec2													GetBottomChainLastVertex() { return bottomChain.back(); }

	b2Vec2													GetWorldPosition() { return worldPosition; }
	Vector2													GetPixelPosition() { return pixelPosition; }

	TileLayer												GetTileLayer() { return tileLayers; }

	void													SetHillOrientation(Direction hillOrient) { hillOrientation = hillOrient; }


	void													TestDraw();

	SDL_Rect												GetSourceRect() { return sourceRect; }
	Direction												GetHillDirection() { return hillOrientation; }
	std::vector<std::vector<SDL_Color>>						GetTilePixels();
};

class TileManager {
private:
	//Capping direction
	std::unordered_map<Direction, std::vector<Tile*>>*		groundTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		northEastHillTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		southEastHillTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		platformTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		wallTiles;
	std::shared_ptr<Sprite>									spriteSheet;

	std::shared_ptr<SDL_Texture>							tileMapTexture;
	Vector2													tileMapTextureDrawPosition;

	Vector2													spawn;

	Vector4													bounds;

	std::vector<std::vector<Tile*>>							tileMap;

	int														tileWidth;
	int														tileHeight;

	int														worldRows;
	int														worldCols;

	std::vector<std::vector<SDL_Color>>						spriteSheetPixels;

	std::shared_ptr<Graphics>								graphicsRef;

	b2World* physics;

	Vector2													playerDimensions;
	Vector2													worldSize;

	SDL_Rect												cameraBounds;

	void													TileParseTypesFromJSON(std::string json);

	void													CarvePath();

	void													FillHills(std::vector<std::pair<int, int>>& caveWalk);
	void													CarveCaves();

	void													CreateMapRenderTarget();

public:
	TileManager(const char* filepath, const std::shared_ptr<Graphics>& graphics, b2World* world, Vector2 playerDimensions);

	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset, SDL_Rect& cameraBounds);

	bool IsInCameraBounds(Tile* t, SDL_Rect cameraBounds);

	std::vector<std::vector<Tile*>>* GenerateTileMap(b2World* physicsWorld, Vector2 pDim);

	void GenerateTileMapRectArea(SDL_Rect& r);

	void LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap);

	Vector2 GetTileDimensions() { return Vector2(tileWidth, tileHeight); }

	std::vector<std::vector<Tile*>>* GetTileMap() { return &tileMap; }

	Vector4 GetBounds() { return bounds; }

	inline bool InBounds(int x, int y) { return x > 1 && x < worldCols-1 && y > 1 && y < worldRows-1; }

	b2Vec2 FindSpawnPointFromLeft();

	std::vector<std::vector<SDL_Color>> CopyRectOfTilePixelsFromTexture(SDL_Rect* sR);
	std::unordered_set<std::pair<int,int>, PairHash> GetWalkPerimeter(std::vector<std::pair<int, int>>& caveWalk);
};