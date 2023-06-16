#pragma once
#include <unordered_map>
#include <utility>

#include <box2d/box2d.h>
#include <animation.h>
#include <perlinnoise.h>


const int VERTICES_PER_EDGE = 2;
const int MAX_EDGES = 4;


enum Direction : unsigned short{
	North = 0,
	East = 1,
	South = 2,
	West = 4,
	None = 8
};

enum TileLayer : unsigned short{
	Ground = 0,
	Hill = 1,
	Wall = 2,
	Platform = 4,
	Empty = 8
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

	b2Body*													physicsBody;
	Direction												capDirection;

	std::shared_ptr<Graphics>								graphicsRef;

	std::shared_ptr<Sprite>									spriteSheet;

	SDL_Rect												sourceRect;

	std::vector<b2Vec2>										topChain;
	std::vector<b2Vec2>										bottomChain;
	std::vector<b2Vec2>										eastCap;
	std::vector<b2Vec2>										westCap;

	b2Fixture*												topFix;
	b2Fixture*												bottomFix;
	b2Fixture*												eastFix;
	b2Fixture*												westFix;

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

	b2Body*													GetBodyReference() { return physicsBody; }

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
};

class TileManager {
private:
	//Capping direction
	std::unordered_map<Direction, std::vector<Tile*>>*		groundTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		hillTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		platformTiles;
	std::unordered_map<Direction, std::vector<Tile*>>*		wallTiles;
	std::shared_ptr<Sprite>									spriteSheet;
	std::vector<std::vector<Tile*>>							tileMap;

	int														tileWidth;
	int														tileHeight;

	int														worldRows;
	int														worldCols;

	std::vector<std::vector<SDL_Color>>						spriteSheetPixels;

	std::shared_ptr<Graphics>								graphicsRef;
		
	b2World*												physics;
	
	Vector2													playerDimensions;
	Vector2													worldSize;
								
	void													TileParseTypesFromJSON(std::string json);
					
public:
	TileManager(const char* filepath, const std::shared_ptr<Graphics>& graphics, b2World* world, Vector2 playerDimensions);
	
	~TileManager();

	void UpdateMap();

	void DrawMap(Vector2 cameraOffset, SDL_Rect& cameraBounds);

	bool IsInCameraBounds(Tile* t, SDL_Rect cameraBounds);

	std::vector<std::vector<Tile*>>* GenerateTileMap(PerlinNoise* perlin, b2World* physicsWorld, Vector2 pDim);
	
	void GenerateTileMapRectArea(SDL_Rect& r);

	void LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap);

	Vector2 GetTileDimensions() { return Vector2(tileWidth, tileHeight); }

	std::vector<std::vector<Tile*>>* GetTileMap() { return &tileMap; }
};
