#pragma once
#include <unordered_map>
#include <utility>

#include <vectortypes.h>
#include <box2d/box2d.h>
#include <animation.h>


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


	//Rotation in degrees for SDL2
	float													zRot;


public:
	Tile();
	Tile(int id, std::shared_ptr<Sprite> s, Vector2 gridPosition, Vector2 pDim, Direction dir, std::shared_ptr<Graphics> g, float zRotation, SDL_Rect srcRect);
	Tile(const Tile &oldTile);
	
	~Tile();

	void													FlipChain(std::vector<b2Vec2>& chain);
	void													RotateChain(std::vector<b2Vec2>& chain, float angle);
	void													Draw(Vector2 cameraOffset);
	void													CreatePhysicsEdges(Vector2 playerDim);
	void													CreateTileBody(b2World* world, b2Vec2 tpg, b2Vec2 tng, b2Vec2 bpg, b2Vec2 bng);
	void													SetCappingDirection(Direction capping);
	void													DecideCapping(std::vector<std::vector<SDL_Color>>& pixels, SDL_Rect r);
	void													TilePhysicsInit(b2World* world, Vector2 p, Vector2 playerDim);
	void													SetSpriteDirection(Direction dir) { direction = dir; }
	void													SetSDL_RendererFlipFlags(SDL_RendererFlip flip) { flipFlags = flip; }
	Vector2													GetPixelDimensions() { return pixelDimensions; }

	b2Body*													GetBodyReference() { return physicsBody; }

	SDL_Color												GetDebugColor() { return debugColor; }

	b2Vec2													GetTopChainFirstVertex() { return topChain.front(); }
	b2Vec2													GetTopChainLastVertex() { return topChain.back(); }
	b2Vec2													GetBottomChainFirstVertex() { return bottomChain.front(); }
	b2Vec2													GetBottomChainLastVertex() { return bottomChain.back(); }

	b2Vec2													GetWorldPosition() { return worldPosition; }
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

	std::vector<std::vector<Tile*>>* CreateTileMap();

	void LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap);
};
