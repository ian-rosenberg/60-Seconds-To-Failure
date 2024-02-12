#pragma once
#include "drunkard.h"
#include "animation.h"

#include <unordered_map>
#include <utility>

#include <box2d/box2d.h>


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;
const int MAX_TUNNELS = 6;

enum Direction{
	North = 1 << 2,
	East = 1 << 3,
	South = 1 << 4,
	West = 1 << 5,
	None = 1 << 6
};

inline Direction operator|(Direction a, Direction b) {
	return (Direction)(int(a) | int(b));
}

inline Direction& operator|=(Direction a, Direction b) {
	a = a | b;
	return a;
}

inline Direction operator&(Direction a, Direction b) {
	return Direction(int(a) & int(b));
}

inline Direction operator&=(Direction a, Direction b) {
	a = a & b;
	return a;
}

inline bool operator==(Direction a, Direction b) {
	return (int)a == (int)b;
}

inline bool operator!=(Direction a, Direction b) {
	return (int)a != (int)b;
}

enum TileLayer{
	Ground = 1 << 7,
	Hill = 1 << 8,
	Wall = 1 << 9,
	Platform = 1 << 10,
	Empty = 1 << 11
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

	int														gridX;
	int														gridY;
	Vector2													pixelPosition;
	b2Vec2													worldPosition;

	b2Vec2													worldCenter;
	Vector2													pixelCenter;

	b2Body*													physicsBody;
	b2Fixture*												fixture;

	Direction												capDirection;

	std::shared_ptr<Graphics>								graphicsRef;

	std::shared_ptr<Sprite>									sprite;

	SDL_Rect												sourceRect;

	std::vector<b2Vec2>										topChain;
	std::vector<b2Vec2>										bottomChain;
	std::vector<b2Vec2>										eastCap;
	std::vector<b2Vec2>										westCap;


	std::vector<TileConnection>								possibleConnections;
	TileLayer												tileLayers;
	Direction												hillOrientation;



	//Rotation in degrees for SDL2
	float													zRot;
	float*													slopes;


public:
	Tile();
	Tile(int id, Sprite* srcSheet, Vector2 gridPosition, Vector2 pDim, Direction dir, const std::shared_ptr<Graphics>& graphics, float zRotation, SDL_Rect srcRect, float* slopes);
	Tile(const Tile& oldTile);
	Tile& operator= (const Tile& other);

	~Tile();

	void													AddPossibleConnection(Vector2 v, TileLayer layer, Direction hillDir);
	void													RotateChain(std::vector<b2Vec2>& chain, float angle);
	void													Draw(Vector2 cameraOffset);
	void													CreatePhysicsEdges(Vector2 playerDim);
	void													CreateTileBody(b2World* world);
	void													SetCappingDirection(Direction capping);
	void													DecideCapping();
	void													TilePhysicsInit(b2World* world, Vector2 playerDim);
	void													SetSpriteDirection(Direction dir) { direction = dir; }
	void													SetSDL_RendererFlipFlags(SDL_RendererFlip flip);
	void													SetTileLayer(TileLayer layers) { tileLayers = layers; }
	void													SetGridPosition(int col, int row);

	Vector2													GetPixelDimensions() { return pixelDimensions; }

	b2Body*													GetBodyReference() { return physicsBody; }

	SDL_Color												GetDebugColor() { return debugColor; }

	//b2Vec2													GetTopChainFirstVertex() { return (flipFlags == SDL_FLIP_NONE || flipFlags == (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL)) ? topChain.front() : topChain.back(); }
	//b2Vec2													GetTopChainLastVertex() { return (flipFlags == SDL_FLIP_NONE || flipFlags == (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL)) ? topChain.back() : topChain.front(); }
	//b2Vec2													GetBottomChainFirstVertex() { return (flipFlags == SDL_FLIP_NONE || flipFlags == (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL)) ? bottomChain.front() : bottomChain.back(); }
	//b2Vec2													GetBottomChainLastVertex() { return (flipFlags == SDL_FLIP_NONE || flipFlags == (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL)) ? bottomChain.back() : bottomChain.front(); }

		
	b2Vec2													GetTopChainFirstVertex() { return topChain.front(); }
	b2Vec2													GetTopChainLastVertex() { return topChain.back(); }
	b2Vec2													GetBottomChainFirstVertex() { return bottomChain.front(); }
	b2Vec2													GetBottomChainLastVertex() { return bottomChain.back(); }
																																					 
	b2Vec2													GetWorldPosition() { return worldPosition; }											 
	Vector2													GetPixelPosition() { return pixelPosition; }

	TileLayer												GetTileLayer() { return tileLayers; }

	void													SetHillOrientation(Direction hillOrient) { hillOrientation = hillOrient; }


	void													TestDraw(const SDL_Rect& cameraBounds);

	SDL_Rect												GetSourceRect() { return sourceRect; }
	Direction												GetHillDirection() { return hillOrientation; }
	Direction												GetCappingDirection() { return capDirection; }
	std::vector<std::vector<SDL_Color>>						GetTilePixels();

	float*													GetSlopes() { return slopes; }
	void													FlipChain(std::vector<b2Vec2>& chain);

	SDL_RendererFlip										GetFlipFlags() { return flipFlags; }

	float													GetZRotation() { return zRot; }
	b2Fixture*												GetShapeFixture(){ return fixture; }
	Coord										GetGridPosition() { return Coord(gridX,gridY); }
};

typedef struct TileNode {
	Direction direction;

	std::vector<Tile*> tiles;
	TileNode* child;
}TileNode_S;

class TileCollection {
private:
	TileNode* root;

public:
	TileCollection();
	~TileCollection();

	//Creates an entry for the capping direction of the tilewhen it returns false, 
	void AddTile(Tile* newTile);
	std::vector<Tile*>* FindTilesOfDirection(Direction dir);
	Tile* FindNthTileOfDirection(Direction dir, uint16_t index);
};

class TileManager {
private:
	//Capping direction
	TileCollection*		groundTiles;
	TileCollection*		northEastHillTiles;
	TileCollection*		southEastHillTiles;
	TileCollection*		platformTiles;

	Sprite*													spriteSheet;

	SDL_Texture*											tileMapTexture;
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

	void													CarveCaves(std::vector<std::vector<TileLayer>>& pseudoMap, std::vector<Coord>& carveWalk);
	void													CreatePlatforms(std::vector<std::vector<TileLayer>>& pseudoMap, std::vector<SDL_Rect>& platformStarts);
	void													FillHills(std::vector<std::vector<TileLayer>>& pseudoMap, std::vector<SDL_Rect>& platformTops, std::vector<Coord>& caveWalkPerimeter);
	//void													FillCeiling(std::vector<std::vector<int>>& pseudoMap);
	void													CreateTileMapBodies(std::vector<std::vector<int>>& pseudoMap);
	void													CreateMapRenderTarget();
	void													ConvertLocalMapToTileLayer(std::vector<std::vector<int>>& localMap, std::vector<std::vector<TileLayer>>& pseudoMap);
	void													CreateLocalMap(std::vector<std::vector<TileLayer>>& pseudoMap, std::vector<std::vector<int>>& localMap);
	void													PrunePseudoMap(std::vector<std::vector<TileLayer>>& map);
	std::vector<Coord>										PlatformDFS(int x, int y, int & platformFlag, std::vector<std::vector<int>>& pmap);
	bool													IslandDFS(int x, int y, TileLayer target, std::vector<std::vector<TileLayer>>& pmap);
	void													GetMapPerimeter(std::vector<Coord>& perimeter, std::vector<std::vector<int>>& localMap);

public:
	TileManager(const char* filepath, const std::shared_ptr<Graphics>& graphics, b2World* world, Vector2 playerDimensions);

	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset, SDL_Rect& cameraBounds);

	bool IsInCameraBounds(Tile* t, SDL_Rect cameraBounds);

	std::vector<std::vector<Tile*>>* GenerateTileMap(b2World* physicsWorld, Vector2 pDim);

	Vector2 GetTileDimensions() { return Vector2(tileWidth, tileHeight); }

	std::vector<std::vector<Tile*>>* GetTileMap() { return &tileMap; }

	Vector4 GetBounds() { return bounds; }

	bool IsOfPlatform(int x, int y);
	
	bool IsInBounds(int x, int y);

	std::vector<std::vector<SDL_Color>> CopyRectOfTilePixelsFromTexture(SDL_Rect* sR);
	std::unordered_set<Coord, PairHash> GetWalkPerimeter(std::vector<Coord>& caveWalk, std::vector<std::vector<int>>& localMap);
	void PrintMapToConsole(std::vector<std::vector<int>> const & pmap);
};