#include "tile.h"
#include "drunkard.h"
#include "gaussianblur.h"

#include <algorithm>
#include <random>
#include <complex>
#include <stack>


#include <windows.h>

extern "C" {
#include "simple_json.h"
}

#define EDGE_COUNT 2
#define MAX_CAPS 2
#define GAUSSIAN_FILTER_SIZE 3
#define DRUNK_WALK_MAX 4
#define DRUNK_WALK_MIN 1
#define PLATFORM_TOP_EDGE_MIN 4
#define PLATFORM_TOP_EDGE_MAX 9
#define WORLD_ROW_MIN 30
#define WORLD_ROW_MAX 50
#define WORLD_COL_MIN 60
#define WORLD_COL_MAX 100
				  
void Tile::CreatePhysicsEdges(Vector2 playerDimensions)
{
	std::vector<std::vector<SDL_Color>> pixels;
	SDL_Rect boundingRect = { INT_MAX, INT_MAX, -1, -1 };
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (topChain.size() > 0 && bottomChain.size() > 0) {
		return;
	}

	pixels = Sprite::GetPixelData(graphicsRef, sprite.get());


	if (direction == Direction::North) {
		//scan north -> south
		std::cout << "Scanning north to south" << std::endl;
		for (col = 0; col < pixels[row].size(); col++) {
			for (; row < pixels.size(); row++)
			{
				if (pixels[row][col].a > 0) {
					vert = b2Vec2((col)*MET_IN_PIX, (row)*MET_IN_PIX);
					topChain.push_back(vert);
					col += (playerDimensions.x / 2);
					break;
				}
			}
			row = 0;
		}

		for (col = pixels[row].size() - 1, row = 0; row < pixels.size(); row++) {
			if (pixels[row][col].a > 0) {
				vert = b2Vec2((col)*MET_IN_PIX, (row)*MET_IN_PIX);
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning south to north" << std::endl;

		row = pixels.size() - 1;

		//scan from south -> north
		for (col = 0; col < pixels[row].size(); col++) {
			for (; row >= 0; row--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2((col)*MET_IN_PIX, (row)*MET_IN_PIX);
					bottomChain.push_back(vert);
					col += (playerDimensions.x / 2);
					break;
				}
			}
			row = pixels.size() - 1;
		}


		for (row = pixels.size()-1, col = pixels[row].size() - 1; row >= 0; row--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2((col)*MET_IN_PIX, (row)*MET_IN_PIX);
				bottomChain.push_back(vert);
				break;
			}
		}
	}
	else if (direction == Direction::East) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = 0;
		for (; row < pixels.size(); row++) {
			for (col = 0; col < pixels[row].size(); col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row));
					topChain.push_back(vert);
					row += (playerDimensions.x / 2);
					break;
				}
			}
		}

		for (col = 0, row = pixels.size() - 1; col < pixels[row].size(); col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row));
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		for (row = 0; row < pixels.size() - 1; row++) {
			col = pixels[row].size() - 1;
			for (; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row));
					bottomChain.push_back(vert);
					row += (playerDimensions.x / 2);
					break;
				}
			}
		}

		for (col = pixels[row].size() - 1, row = pixels.size() - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row));
				bottomChain.push_back(vert);
				break;
			}
		}
	}
	/*else if (direction == Direction::South) {
		while (col < pixels[row].size()) {
			for (int row = pixels.size() - 1; row >= 0; row--)
			{
				if (pixels[row][col].a == 0) {
					pixel = { col * 1.0f, row * 1.0f };
					std::cout << "column: " << pixel.x << " row: " << pixel.y << std::endl;
					graphicsRef->Vector2PixelsToMeters(pixel);
					vert = b2Vec2(pixel.x, pixel.y);
					chain.push_back(vert);
					col += (playerDimensions.x / 2);
					break;
				}
			}
		}
	}*/
	else if (direction == Direction::West) {
		//scan from east -> west
		std::cout << "Scanning west to east" << std::endl;

		for (row = 0; row < pixels.size() - 1; row++) {
			for (col = 0; col < pixels[row].size(); col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row));
					topChain.push_back(vert);
					row += (playerDimensions.x / 2);
					break;
				}
			}
		}

		for (col = 0, row = pixels.size() - 1; col < pixels[row].size(); col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row));
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from west -> east
		for (row = 0; row < pixels.size() - 1; row++) {
			for (col = pixels[row].size() - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row));
					bottomChain.push_back(vert);
					row += (playerDimensions.x / 2);
					break;
				}
			}
		}

		for (col = pixels[row].size() - 1, row = pixels.size() - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row));
				bottomChain.push_back(vert);
				break;
			}
		}
	}

	pixels.clear();
}

void Tile::DecideCapping()
{
	SDL_Rect r = sourceRect;

	//Decide to cap cardinal direction(s)
	if (capDirection != Direction::None) {
		if ((capDirection & Direction::East) == Direction::East && topChain.back() != bottomChain.back()){
			eastCap.push_back(topChain.back());
			eastCap.push_back(bottomChain.front());
		}

		if ((capDirection & Direction::West) == Direction::West && bottomChain.front() != topChain.front()) {
			westCap.push_back(bottomChain.back());
			westCap.push_back(topChain.front());
		}
	}
}

void Tile::TilePhysicsInit(b2World* world, Vector2 playerDim)
{
	Vector2 tmp;
	tmp = pixelPosition;

	graphicsRef->Vector2PixelsToMeters(tmp);

	worldPosition = {
		(float)tmp.x,
		(float)tmp.y
	};

	CreatePhysicsEdges(playerDim);

	DecideCapping();
}

void Tile::SetSDL_RendererFlipFlags(SDL_RendererFlip flip)
{
	if (flipFlags == flip)
		return;

	flipFlags = flip;

	std::shared_ptr<Sprite> og = sprite;

	sprite = std::make_shared<Sprite>(*sprite->MakeFlippedTexture(flipFlags));
	
	og.reset();
}

void Tile::SetGridPosition(int col, int row)
{

	gridX = col;
	gridY = row;

	pixelPosition = {
		col * 1.0 * sourceRect.w,
		row * 1.0 * sourceRect.h
	};

	if (physicsBody) {
		Vector2 pos = pixelPosition;
		graphicsRef->Vector2PixelsToMeters(pos);
		b2Vec2 bPos(pos.x, pos.y);
		physicsBody->SetTransform(bPos, 0.f);
	}

}

void Tile::CreateTileBody(b2World* world)
{

	//Since Box2D supports 8 ( EIGHT ) vertices MAX per polygon, we chop up the tiles into smaller, 4 vertex fixtures 
	b2BodyDef bd;
	b2FixtureDef fd;
	Vector2 tV;
	std::vector<b2PolygonShape> polyShapes;
	b2Vec2 center;
	SDL_Renderer* r = graphicsRef->GetRenderer();

	bd.type = b2_staticBody;

	bd.position.Set(pixelPosition.x * MET_IN_PIX, pixelPosition.y * MET_IN_PIX);

	worldPosition = bd.position;

	center = { (float)(worldPosition.x + pixelDimensions.x * MET_IN_PIX / 2),
		(float)(worldPosition.y + pixelDimensions.y * MET_IN_PIX / 2)};
	
	switch (direction) {
	case North:
		bd.angle = 0.f;
		break;

	case East:
		bd.angle = M_PI * 1.5f;
		break;

	case South:
		bd.angle = M_PI;
		break;

	case West:
		bd.angle = M_PI * 0.5f;
		break;

	default:
		break;
	}

	physicsBody = world->CreateBody(&bd);

	if(flipFlags == SDL_FLIP_HORIZONTAL)
		for (int i = 0; i < MAX_EDGES; i++)
			slopes[i] *= -1;

	if(flipFlags == SDL_FLIP_VERTICAL)
		for (int i = 0; i < MAX_EDGES; i++)
			slopes[i] *= -1;

	//NEEDS WORKS
	for (int i = 0; i + 1 < topChain.size() && i + 1 < bottomChain.size(); i++) {
		b2PolygonShape fix;
		std::vector<b2Vec2> verts;
		
		verts.push_back(topChain[i]);
		verts.push_back(topChain[i + 1]);

		//logic for tunnels?


		verts.push_back(bottomChain[i]);
		verts.push_back(bottomChain[i + 1]);

		/*if (i%2==1 && (topChain.size() - i == 1) || (bottomChain.size() - i == 1))
		{
			if((topChain.size() - i == 1))
				verts.push_back(topChain[i + 2]);
			if (bottomChain.size() - i == 1)
				verts.push_back(bottomChain[i + 2]);

			fix.Set(verts.data(), verts.size());
			fd.friction = 0.7f;
			fd.shape = &fix;
			fixture = physicsBody->CreateFixture(&fd);

			break;
		}*/

		fix.Set(verts.data(), verts.size());
		fd.shape = &fix;
		fd.friction = 0.7f;
		fixture = physicsBody->CreateFixture(&fd);

	}
}

void Tile::SetCappingDirection(Direction capping)
{
	if (capping == capDirection)
		return;

	this->capDirection = capping;

	topChain.clear();
	bottomChain.clear();
	eastCap.clear();
	westCap.clear();
}

Tile::Tile()
{
	id = 0;
	direction = Direction::North;
	pixelDimensions = { 0,0 };
	worldDimensions = { 0,0 };

	pixelPosition = { 0,0 };
	worldPosition = { 0,0 };

	physicsBody = nullptr;

	sprite = nullptr;
			   
	graphicsRef = nullptr;

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	tileLayers = TileLayer::Ground;

	zRot = 0.f;

	debugColor = SDL_Color(0, 255, 0, 255);

	hillOrientation = Direction::None;
	slopes = nullptr;

	SetGridPosition(INT_MIN, INT_MIN);
}

Tile::Tile(int id, Sprite* srcSheet, Vector2 gridPosition, Vector2 dim, Direction dir, const std::shared_ptr<Graphics>& graphics, float zRotation, SDL_Rect srcRect, float* slopes)
{
	this->id = id;

	sourceRect = srcRect;

	graphicsRef = graphics;
	sprite = std::shared_ptr<Sprite>(new Sprite(*srcSheet));
	sprite->UpdateSrcRect(sourceRect);

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	pixelDimensions = dim;

	worldDimensions.Set(dim.x * MET_IN_PIX, dim.y * MET_IN_PIX);

	pixelPosition = { gridPosition.x * pixelDimensions.x, gridPosition.y * pixelDimensions.y };
	direction = dir;
	zRot = zRotation;

	debugColor = SDL_Color(0, 255, 0, 255);
	this->slopes = slopes;

	SetGridPosition(INT_MIN, INT_MIN);

	tileLayers = TileLayer::Ground;
}

Tile::Tile(const Tile &oldTile)
{
	this->id = oldTile.id;
	this->direction = oldTile.direction;
	this->pixelDimensions = oldTile.pixelDimensions;
	this->worldDimensions = oldTile.worldDimensions;
	this->flipFlags = oldTile.flipFlags;

	this->pixelPosition = { 0,0 };

	this->worldPosition = { 0,0 };

	this->physicsBody = nullptr;

    this->graphicsRef = oldTile.graphicsRef;
	this->sprite = oldTile.sprite;

	this->possibleConnections = oldTile.possibleConnections;

	this->capDirection = oldTile.capDirection;

	this->zRot = oldTile.zRot;

	this->debugColor = oldTile.debugColor;

	this->sourceRect = oldTile.sourceRect;

	this->topChain =    std::vector<b2Vec2>(oldTile.topChain);
	this->bottomChain = std::vector<b2Vec2>(oldTile.bottomChain);
	this->eastCap =     std::vector<b2Vec2>(oldTile.eastCap);
	this->westCap =     std::vector<b2Vec2>(oldTile.westCap);


	this->slopes = oldTile.slopes;

	SetGridPosition(INT_MIN, INT_MIN);
	tileLayers = oldTile.tileLayers;
}

Tile& Tile::operator=(const Tile& rhs)
{
	this->id = rhs.id;
	this->direction = rhs.direction;
	this->pixelDimensions = rhs.pixelDimensions;
	this->worldDimensions = rhs.worldDimensions;

	this->pixelPosition = { 0,0 };

	this->worldPosition = { 0,0 };

	this->physicsBody = nullptr;

	this->graphicsRef = rhs.graphicsRef;
	this->sprite = rhs.sprite;

	this->possibleConnections = rhs.possibleConnections;

	this->capDirection = rhs.capDirection;

	this->zRot = rhs.zRot;

	this->debugColor = rhs.debugColor;

	this->sourceRect = rhs.sourceRect;

	this->topChain = std::vector<b2Vec2>(rhs.topChain);
	this->bottomChain = std::vector<b2Vec2>(rhs.bottomChain);
	this->eastCap = std::vector<b2Vec2>(rhs.eastCap);
	this->westCap = std::vector<b2Vec2>(rhs.westCap);


	this->slopes = rhs.slopes;

	this->flipFlags = rhs.flipFlags;

	SetGridPosition(INT_MIN, INT_MIN);

	tileLayers = rhs.tileLayers;

	return *this;
}

Tile::~Tile()
{
	possibleConnections.clear();

	sprite.reset();
	graphicsRef.reset();
	
	fixture = nullptr;
	physicsBody = nullptr;

	topChain.clear();
	bottomChain.clear();
	eastCap.clear();
	westCap.clear();
}

void Tile::AddPossibleConnection(Vector2 v, TileLayer layer, Direction hillDir)
{
	possibleConnections.push_back(TileConnection{
		(int)v.x,
		(int)v.y,
		layer,
		hillDir
		});
}

void Tile::RotateChain(std::vector<b2Vec2>& chain, float theta)
{
	float c = 0, s = 0, cx = 0, cy = 0;
	float xNew, yNew;
	Vector2 ctr;

	worldPosition = physicsBody->GetPosition();
	
	ctr = { (float)worldPosition.x + worldDimensions.x / 2,
		(float)worldPosition.y + worldDimensions.y / 2 };

	c = cos(-theta);
	s = sin(-theta);

	cx = worldPosition.x;
	cy = worldPosition.y;

	for (b2Vec2 p : chain) {

		// translate point back to origin:
		p.x -= cx;
		p.y -= cy;
		// rotate point
		double Xnew = ctr.x * c - ctr.y * s;
		double Ynew = ctr.x * s + ctr.y * c;
		// translate point back:
		p.Set(Xnew + cx,Ynew + cy);
	}
}

void Tile::Draw(Vector2 cameraOffset)
{
	Vector2 scale;
	Vector2 scaleCenter;
	Vector3 rot{ 0,0,0 };
	Vector4 color = { 255,255,255,255 };
	SDL_Rect dstRect;

	scale = { 1,1 };

	scaleCenter = {
		sourceRect.w / 2.0f,
		sourceRect.h / 2.0f
	};

	dstRect = { 
		(int)(pixelPosition.x - cameraOffset.x),
		(int)(pixelPosition.y - cameraOffset.y),
		(int)pixelDimensions.x, 
		(int)pixelDimensions.y 
	};

	sprite->Draw(Vector2(dstRect.x, dstRect.y), sourceRect, flipFlags);
}

void Tile::TestDraw(const SDL_Rect & cameraBounds)
{
	Vector2 scale;
	Vector2 scaleCenter;
	Vector3 rot{ 0,0,0 };
	Vector4 color = { 255,255,255,255 };
	SDL_Rect dstRect;
	Vector2 sDim = graphicsRef->GetScreenDimensions();
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, camX = 0, camY = 0, ite = 0;

	std::vector<std::vector<b2Vec2>> chains;
	chains.push_back(bottomChain);
	if (!eastCap.empty()) {
		chains.push_back(eastCap);
		std::reverse(chains.back().begin(), chains.back().end());
	}
	
	chains.push_back(topChain);
	
	if (!westCap.empty()) {
		chains.push_back(westCap);
		std::reverse(chains.back().begin(), chains.back().end());
	}
	scale = { 1,1 };

	scaleCenter = {
		sourceRect.w / 2.0f,
		sourceRect.h / 2.0f
	};

	dstRect = { (int)0,
		(int)0,
		(int)pixelDimensions.x, 
		(int)pixelDimensions.y 
	};

	SDL_RenderClear(graphicsRef->GetRenderer());
	sprite->DrawSpriteImage(sprite.get(), Vector2(0,0), 1280, 720);
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);
	for (std::vector<b2Vec2> chain : chains) {
		const b2Vec2* vertices = chain.data();
		int32 vertexCount = chain.size();
		
		for (int i = 0, j = 1; j < vertexCount; i++, j++) {
			t = *(vertices + i) - worldPosition;
			a = Vector2(t.x, t.y);
			graphicsRef->Vector2MetersToPixels(a);

			a.x -= camX;
			a.y -= camY;

			t = *(vertices + j) - worldPosition;
			b = Vector2(t.x, t.y);
			graphicsRef->Vector2MetersToPixels(b);

			b.x -= camX;
			b.y -= camY;

			SDL_RenderDrawLine(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);
		}

		a = b;

		t = *(vertices) - worldPosition;
		b = Vector2(t.x, t.y);
		graphicsRef->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;
	}

	SDL_RenderDrawLine(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
	SDL_RenderPresent(graphicsRef->GetRenderer());
}

std::vector<std::vector<SDL_Color>> Tile::GetTilePixels()
{
	SDL_Rect sR = GetSourceRect();
	return Sprite::GetPixelData(sprite->GetFilePath().c_str(), &sR, graphicsRef);	
}

void Tile::FlipChain(std::vector<b2Vec2> &chain)
{
	if (flipFlags == SDL_FLIP_NONE)
		return;
	if (flipFlags == SDL_FLIP_HORIZONTAL) {
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			std::swap(chain[i].x, chain[j].x);

		std::reverse(chain.begin(), chain.end());
	}
	if (flipFlags == SDL_FLIP_VERTICAL) {
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			std::swap(chain[i], chain[j]);

		std::reverse(chain.begin(), chain.end());
	}
}

void TileManager::TileParseTypesFromJSON(std::string json)
{
	const char* dirStr;
	std::vector<Tile*>* tiles = new std::vector<Tile*>();
	float zRot;
	Direction direction;
	SDL_Rect srcRect;
	SDL_Rect imageRect;
	SJson* genDesc = sj_array_new();
	SJson* jsonResource = sj_load(json.c_str());
	SJson* genDescList = nullptr;
	int xLocation = 0, yLocation = 0; 


	TileSpriteSheet* tss = new TileSpriteSheet();

	sj_get_uint8_value(sj_object_get_value(jsonResource, "tilecount"), &tss->tileCount);
	sj_get_uint8_value(sj_object_get_value(jsonResource, "columns"), &tss->columns);
	sj_get_uint32_value(sj_object_get_value(jsonResource, "tilewidth"), &tss->tileWidth);
	sj_get_uint32_value(sj_object_get_value(jsonResource, "tileheight"), &tss->tileHeight);
	sj_get_uint32_value(sj_object_get_value(jsonResource, "imagewidth"), &tss->imageWidth);
	sj_get_uint32_value(sj_object_get_value(jsonResource, "imageheight"), &tss->imageHeight);
	sj_get_uint8_value(sj_object_get_value(jsonResource, "spacing"), &tss->spacing);
	sj_get_uint8_value(sj_object_get_value(jsonResource, "margin"), &tss->margin);
	
	dirStr = sj_get_string_value(sj_object_get_value(jsonResource, "direction"));
	tss->name = sj_get_string_value(sj_object_get_value(jsonResource, "name"));
	tss->filepath = sj_get_string_value(sj_object_get_value(jsonResource, "spritesheet"));

	genDesc = sj_load(
		sj_get_string_value(
			sj_object_get_value(jsonResource, "generationDescription")
		));
	genDescList = sj_object_get_value(genDesc, sj_get_string_value(sj_object_get_value(jsonResource, "name")));

	if (strcmp(dirStr, "north") == 0)
		direction = Direction::North;
	else if (strcmp(dirStr, "east") == 0)
		direction = Direction::East;
	else if (strcmp(dirStr, "west") == 0)
		direction = Direction::South;
	else if (strcmp(dirStr, "south") == 0)
		direction = Direction::West;
	else
		direction = Direction::None;

	spriteSheet = new Sprite(tss->filepath, 
		tss->imageWidth, 
		tss->imageHeight, 
		graphicsRef);

	tileWidth = tss->tileWidth;
	tileHeight = tss->tileHeight;

	imageRect = { 0,0,(int)tss->imageWidth, (int)tss->imageHeight };

	spriteSheetPixels = spriteSheet->GetPixelData(tss->filepath, &imageRect, graphicsRef);

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < sj_array_get_count(genDescList); i++) {
		Tile* t = nullptr;
		Tile* tE = nullptr;
		Tile* tW = nullptr;
		Tile* tF = nullptr;

		Tile* xMirrorHillTile = nullptr;
		Tile* xMirrorHillTileE = nullptr;
		Tile* xMirrorHillTileW = nullptr;
		Tile* xMirrorHillTileF = nullptr;

		//Tile* yMirrorHillTile = nullptr;
		//Tile* yMirrorHillTileE = nullptr;
		//Tile* yMirrorHillTileW = nullptr;
		//Tile* yMirrorHillTileF = nullptr;
		int layerCount = 0;
		float* slopes = new float[MAX_EDGES];


		layerCount = sj_array_get_count(sj_object_get_value(sj_array_get_nth(genDescList, i), "layers"));
		sj_get_integer_value(sj_object_get_value(sj_array_get_nth(genDescList, i), "xLocation"), &xLocation);
		sj_get_integer_value(sj_object_get_value(sj_array_get_nth(genDescList, i), "yLocation"), &yLocation);

		const char* o = sj_get_string_value(sj_object_get_value(sj_array_get_nth(genDescList, i), "hillOrientation"));
		SJson* tileIndex = sj_object_get_value(sj_array_get_nth(genDescList, i), "possibleConnects");
		int possibleConnects = sj_array_get_count(tileIndex);
		SDL_Rect sR = {
			xLocation,
			yLocation,
			tss->tileWidth,
			tss->tileHeight
		};

		std::vector<std::vector<SDL_Color>> tilePixels = CopyRectOfTilePixelsFromTexture(&sR);


		SJson* tileSlopes = sj_object_get_value(sj_array_get_nth(genDescList, i), "slopes");
		int slopeCount = sj_array_get_count(tileSlopes);

		for (int f = 0; f < slopeCount; f++)
			sj_get_float_value(sj_array_get_nth(sj_object_get_value(sj_array_get_nth(genDescList, i), "slopes"), f), &slopes[f]);

		if (!spriteSheet->CheckIfViableTexture(sR))
			continue;

		switch (direction) {
		case North:
			zRot = 0.f;
			break;

		case East:
			zRot = 270.f;
			break;

		case South:
			zRot = 180.f;
			break;

		case West:
			zRot = 90.f;
			break;

		default:
			zRot = 0.f;
			break;
		}

		zRot = (int)(-1 * zRot + 180) % 360;

		//s->RotateTextureZ(zRot);

		t = new Tile(tiles->size() + 1,
			spriteSheet,
			{ INT32_MIN,INT32_MIN },
			Vector2(tss->tileWidth, tss->tileHeight),
			direction,
			graphicsRef,
			zRot,
			sR,
			slopes);
		t->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
		
		tE = new Tile(*t);
		tE->SetCappingDirection(Direction::East);
		
		tW = new Tile(*t);
		tW->SetCappingDirection(Direction::West);

		tF = new Tile(*t);
		tF->SetCappingDirection((Direction)(Direction::East | Direction::West));

		xMirrorHillTile = new Tile(*t);
		xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

		xMirrorHillTileE = new Tile(*xMirrorHillTile);
		xMirrorHillTileE->SetCappingDirection(Direction::East);

		xMirrorHillTileW = new Tile(*xMirrorHillTile);
		xMirrorHillTileW->SetCappingDirection(Direction::West);

		xMirrorHillTileF = new Tile(*xMirrorHillTile);
		xMirrorHillTileF->SetCappingDirection((Direction)(Direction::East | Direction::West));


		t->TilePhysicsInit(physics, playerDimensions);
		tE->TilePhysicsInit(physics, playerDimensions);
		tW->TilePhysicsInit(physics, playerDimensions);
		tF->TilePhysicsInit(physics, playerDimensions);
		xMirrorHillTile->TilePhysicsInit(physics, playerDimensions);
		xMirrorHillTileE->TilePhysicsInit(physics, playerDimensions);
		xMirrorHillTileW->TilePhysicsInit(physics, playerDimensions);
		xMirrorHillTileF->TilePhysicsInit(physics, playerDimensions);
		
		/*yMirrorHillTile = new Tile(*t);
		yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
		yMirrorHillTileE = new Tile(*t);
		yMirrorHillTileW = new Tile(*t);
		yMirrorHillTileF = new Tile(*t);*/



		for (int l = 0; l < layerCount; l++) {
			const char* layerName = sj_get_string_value(sj_array_get_nth(sj_object_get_value(sj_array_get_nth(genDescList, i), "layers"), l));
			if (strcmp(layerName, "ground") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Ground));
			}
			if (strcmp(layerName, "wall") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Wall));
			}
			if (strcmp(layerName, "platform") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Platform));
			}
			if (strcmp(layerName, "hill") == 0) {
				t->SetTileLayer(TileLayer::Hill);
				break;
			}
		}
		for (int j = 0; j < possibleConnects; j++) {
			TileLayer layers = TileLayer::Empty;
			Direction hillOrient;
			SJson* pConnect = sj_array_get_nth(tileIndex, j);
			int allowedLayerCount = sj_array_get_count(sj_object_get_value(pConnect, "allowedLayers"));
			int x, y;

			sj_get_integer_value(sj_object_get_value(pConnect, "x"), &x);
			sj_get_integer_value(sj_object_get_value(pConnect, "y"), &y);

			for (int l = 0; l < allowedLayerCount; l++) {
				const char* layerName(sj_get_string_value(sj_array_get_nth(sj_object_get_value(pConnect, "allowedLayers"), l)));
				if (strcmp(layerName, "ground") == 0) {
					if (layers & TileLayer::Empty)
						layers = TileLayer::Ground;
					else
						layers = (TileLayer)(layers | TileLayer::Ground);
				}
				if (strcmp(layerName, "wall") == 0) {
					if (layers & TileLayer::Empty)
						layers = TileLayer::Wall;
					else
						layers = (TileLayer)(layers | TileLayer::Wall);
				}
				if (strcmp(layerName, "hill") == 0) {
					layers = TileLayer::Hill;
				}
				if (strcmp(layerName, "platform") == 0) {
					if (layers & TileLayer::Empty)
						layers = TileLayer::Platform;
					else
						layers = (TileLayer)(layers | TileLayer::Platform);
				}
			}
			if (strcmp(o, "Northeast") == 0) {
				xMirrorHillTile->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::South | Direction::East));

				xMirrorHillTile->SetHillOrientation((Direction)(Direction::South | Direction::East));

				/*yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::South | Direction::East));

				yMirrorHillTile->SetHillOrientation((Direction)(Direction::South | Direction::East));*/

				t->AddPossibleConnection(
					Vector2(x, y),
					layers,
					(Direction)(Direction::North | Direction::East));

				t->SetHillOrientation((Direction)(Direction::North | Direction::East));
			}
			else if (strcmp(o, "Southeast") == 0) {
				xMirrorHillTile->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::North | Direction::East));

				xMirrorHillTile->SetHillOrientation((Direction)(Direction::North | Direction::East));

				/*yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::North | Direction::East));

				yMirrorHillTile->SetHillOrientation((Direction)(Direction::North | Direction::East));*/

				t->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::South | Direction::East));

				t->SetHillOrientation((Direction)(Direction::South | Direction::East));
			}
			else {
				t->AddPossibleConnection(
					Vector2(x, y),
					layers,
					Direction::None);
			}
		}

		if ((t->GetTileLayer() & TileLayer::Hill) == TileLayer::Hill) {		

			//yMirrorHillTile->TilePhysicsInit(physics, playerDimensions, tilePixels);
			//yMirrorHillTileE->TilePhysicsInit(physics, playerDimensions, tilePixels);
			//yMirrorHillTileW->TilePhysicsInit(physics, playerDimensions, tilePixels);
			//yMirrorHillTileF->TilePhysicsInit(physics, playerDimensions, tilePixels);
		
			if (t->GetHillDirection() == Direction::North) {
				southEastHillTiles->AddTile(xMirrorHillTileE);
				southEastHillTiles->AddTile(xMirrorHillTileW);
				southEastHillTiles->AddTile(xMirrorHillTileF);
				southEastHillTiles->AddTile(xMirrorHillTile);


				/*
					this section sets up upside-down hills

				northEastHillTiles->at(Direction::East).push_back(yMirrorHillTileE);
				northEastHillTiles->at(Direction::West).push_back(yMirrorHillTileW);
				northEastHillTiles->at((Direction)(Direction::East | Direction::West)).push_back(yMirrorHillTileF);
				northEastHillTiles->at(Direction::None).push_back(yMirrorHillTile);
				*/
				
				northEastHillTiles->AddTile(tE);
				northEastHillTiles->AddTile(tW);
				northEastHillTiles->AddTile(tF);
				northEastHillTiles->AddTile(t);
			}
			else {
				northEastHillTiles->AddTile(xMirrorHillTileE);
				northEastHillTiles->AddTile(xMirrorHillTileW);
				northEastHillTiles->AddTile(xMirrorHillTileF);
				northEastHillTiles->AddTile(xMirrorHillTile);


				/*
					this section sets up upside-down hills

				northEastHillTiles->at(Direction::East).push_back(yMirrorHillTileE);
				northEastHillTiles->at(Direction::West).push_back(yMirrorHillTileW);
				northEastHillTiles->at((Direction)(Direction::East | Direction::West)).push_back(yMirrorHillTileF);
				northEastHillTiles->at(Direction::None).push_back(yMirrorHillTile);
				*/

				southEastHillTiles->AddTile(tE);
				southEastHillTiles->AddTile(tW);
				southEastHillTiles->AddTile(tF);
				southEastHillTiles->AddTile(t);
			}
		}
		else {
			delete xMirrorHillTileE;
			delete xMirrorHillTileW;
			delete xMirrorHillTileF;
			delete xMirrorHillTile;

			//delete yMirrorHillTileE;
			//delete yMirrorHillTileW;
			//delete yMirrorHillTileF;
			//delete yMirrorHillTile;

			//t->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
			//tE->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
			//tW->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
			//tF->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);

			groundTiles->AddTile(tE);
			groundTiles->AddTile(tW);
			groundTiles->AddTile(tF);
			groundTiles->AddTile(t);
		}
	}

	sj_free(jsonResource);
	sj_free(genDesc);
}

//void TileManager::FillHills(std::vector<std::vector<int>>& pseudoMap, std::vector<Coord>& platformTops)
//{
//	std::vector<std::vector<Tile*>> platformsFound;
//	std::vector<std::vector<Tile*>> platforms;
//	std::unordered_set<Tile*> platformTopSingles;
//	std::vector<Tile*> allocatedSpots;
//	std::vector<Tile*> viableBodies;
//	std::vector<Tile*> neTiles = *northEastHillTiles->FindTilesOfDirection((Direction)(East | West));
//	std::vector<Tile*> seTiles = *southEastHillTiles->FindTilesOfDirection((Direction)(East | West));
//
//	for (int y = 0; y < pseudoMap.size(); y++) {
//		for (int x = 0; x < pseudoMap[y].size(); x++) {
//			std::vector<Tile*> visitedPlatform;
//			if (pseudoMap[y][x] == 1)
//			{
//				int platform = 1;
//				//PrintMapToConsole(pseudoMap);
//
//				PlatformDFS(pseudoMap, x, y, platform);
//
//				for (int y = 0; y < pseudoMap.size(); y++) {
//					for (int x = 0; x < pseudoMap[y].size(); x++) {
//						if (pseudoMap[y][x] == 2) {
//							visitedPlatform.push_back(tileMap[y][x]);
//							pseudoMap[y][x] = 0;
//							SDL_Rect r = { x * 5,y * 5,5,5 };
//							SDL_RenderDrawRect(graphicsRef->GetRenderer(), &r);
//						}
//					}
//				}
//
//				if (platform == 1) {
//					std::cout << "Platform found starting at " << x << "," << y << std::endl;
//
//					platformsFound.push_back(visitedPlatform);
//
//					SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 255, 255);
//				}
//				else {
//					std::cout << " NO platform found at " << x << "," << y << std::endl;
//				}
//
//
//
//				SDL_RenderPresent(graphicsRef->GetRenderer());
//
//				//PrintMapToConsole(pseudoMap);
//
//				if (platform == 1)
//					platformsFound.push_back(visitedPlatform);
//				//PrintTileMapToConsole(1);
//			}
//		}
//	}
//
//	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255, 255, 255, 255);
//
//
//	for (auto coord : perimeter) {
//		for (int x = coord.X; x < tileMap[coord.Y].size()
//			&& tileMap[coord.Y][x] != nullptr
//			&& IsInBounds(coord.X, coord.Y - 1)
//			&& tileMap[coord.Y - 1][x] == nullptr
//			&& IsInBounds(coord.X, coord.Y - 2)
//			&& tileMap[coord.Y - 2][x] == nullptr; x++)
//		{
//			platformTopSingles.insert(tileMap[coord.Y][x]);
//
//			for (auto tile : platformTopSingles) {
//				Coord c = tile->GetGridPosition();
//				pseudoMap[c.Y][c.X] = 4;
//			}
//
//			SDL_Rect r = { x * 5, coord.Y * 5,5,5 };
//			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
//
//		}
//
//		SDL_RenderPresent(graphicsRef->GetRenderer());
//	}
//
//
//
//	for (auto platform : platformsFound) {
//		for (auto tile : platform) {
//			Coord pos = tile->GetGridPosition();
//			for (int x = pos.X; x < tileMap[pos.Y].size() && tileMap[pos.Y][x] != nullptr; x++) {
//				if (IsInBounds(pos.X, pos.Y - 1)
//					&& IsInBounds(pos.X, pos.Y - 2)
//					&& tileMap[pos.Y - 1][x] == nullptr
//					&& tileMap[pos.Y - 2][x] == nullptr)
//				{
//					platformTopSingles.insert(tileMap[pos.Y][x]);
//
//					for (auto tile : platformTopSingles) {
//						Coord c = tile->GetGridPosition();
//						pseudoMap[c.Y][c.X] = 4;
//					}
//
//					SDL_Rect r = { x * 5, pos.Y * 5,5,5 };
//					SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
//				}
//
//			}
//
//			SDL_RenderPresent(graphicsRef->GetRenderer());
//		}
//	}
//
//
//	for (int y = 0; y < pseudoMap.size(); y++){
//		for (int x = 0; x < pseudoMap[y].size(); x++){
//			std::cout << pseudoMap[y][x];
//	}
//		std::cout << std::endl;
//	}
//
//	for (auto tile : platformTopSingles) {
//		std::vector<Tile*>rowTiles;
//		Coord start = tile->GetGridPosition();
//		bool foundTile = false;
//		for (auto foundRow : platforms) {
//			if (std::find(foundRow.begin(), foundRow.end(), tile) != foundRow.end()) {
//				foundTile = true;
//				break;
//			}
//		}
//
//		if (!foundTile) {
//			for (int xL = start.X - 1, xLOffset = start.X - 2; IsInBounds(xL, start.Y) && tileMap[start.Y][xL]; xL--, xLOffset--) {
//				rowTiles.emplace(rowTiles.begin(), tileMap[start.Y][xL]);
//
//				if (!IsInBounds(start.Y, xLOffset) || !tileMap[start.Y][xLOffset] || pseudoMap[start.Y][xLOffset] != 4) {
//					break;
//				}
//			}
//
//			for (int xR = start.X, xROffset = start.X + 1; IsInBounds(xR, start.Y) && tileMap[start.Y][xR]; xR++, xROffset++) {
//				rowTiles.push_back(tileMap[start.Y][xR]);
//				
//				if (!IsInBounds(start.Y, xROffset) || !tileMap[start.Y][xROffset] || pseudoMap[start.Y][xROffset] != 4) {
//					break;
//				}
//			}
//
//			auto sortRuleLambda = [](Tile* const& s1, Tile* const& s2) -> bool
//				{
//					return s1->GetGridPosition().X < s2->GetGridPosition().X;
//				};
//
//
//			std::sort(rowTiles.begin(), rowTiles.end(), sortRuleLambda);
//
//			platforms.push_back(rowTiles);
//		}
//	}
//
//	auto sortRuleLambda = [](std::vector<Tile*> const& s1, std::vector<Tile*> const& s2) -> bool
//		{
//			return s1[0]->GetGridPosition().Y < s2[0]->GetGridPosition().Y;
//		};
//
//
//	std::sort(platforms.begin(), platforms.end(), sortRuleLambda);
//
//
//	for (auto platform : platforms) {
//		SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), rand() % 256, rand() % 256, rand() % 256, 255);
//
//		for (Tile* t : platform) {
//			Coord coord = t->GetGridPosition();
//			SDL_Rect r = { coord.X * 5, coord.Y * 5,5,5 };
//			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
//		}
//		SDL_RenderPresent(graphicsRef->GetRenderer());
//	}
//
//
//	for (auto platform : platforms) {
//		Coord fNW, fNE, bNW, bNE, front, back;
//		Coord lCoord, rCoord;
//		Tile* leftCorner = nullptr;
//		Tile* rightCorner = nullptr;
//		int slopeIterator;
//		int rint = (rand() % platform.size()) + 1;
//		int freq = platform.size() / rint;
//		int fi = 0;
//		int i = 0;
//		bool up = false;
//
//		front = platform.front()->GetGridPosition();
//		back = platform.back()->GetGridPosition();
//
//		fNW = { front.X - 1, front.Y - 1 };
//		fNE = { front.X + 1, front.Y - 1 };
//		bNW = { back.X - 1, back.Y - 1 };
//		bNE = { back.X + 1, back.Y - 1 };
//
//		slopeIterator = rint;
//
//		lCoord = front;
//		rCoord = back;
//
//		i = lCoord.X;
//
//		if (IsInBounds(fNW.X, fNW.Y) && IsInBounds(fNE.X, fNE.Y) && !tileMap[fNW.Y][fNW.X] != !tileMap[fNE.Y][fNE.X]) {
//			leftCorner = (tileMap[fNW.Y][fNW.X]) ? tileMap[fNW.Y][fNW.X] : tileMap[fNE.Y][fNE.X];
//		}
//
//		if (IsInBounds(bNW.X, bNW.Y) && IsInBounds(bNE.X, bNE.Y) && !tileMap[bNW.Y][bNW.X] != !tileMap[bNE.Y][bNE.X]) {
//			rightCorner = (tileMap[bNW.Y][bNW.X]) ? tileMap[bNW.Y][bNW.X] : tileMap[bNE.Y][bNE.X];
//		}
//
//		if (leftCorner)
//			up = true;
//
//
//		for (; fi <= freq; fi++) {
//			for (; i < lCoord.X + rint/freq && slopeIterator >= 0 && i < rCoord.X && tileMap[lCoord.Y][i] && !tileMap[lCoord.Y - 1][i]; i++, slopeIterator--) {
//				tileMap[lCoord.Y - 1][i] = up ? new Tile(*neTiles[slopeIterator]) : new Tile(*seTiles[slopeIterator]);
//				tileMap[lCoord.Y - 1][i]->SetGridPosition(i, lCoord.Y - 1);
//				tileMap[lCoord.Y - 1][i]->TestDraw(cameraBounds);
//				allocatedSpots.push_back(tileMap[lCoord.Y - 1][i]);
//			}
//
//			if (i >= rCoord.X)
//				break;
//
//			lCoord.X += rint/2 - 1;
//			up = !up;
//			for (; i < lCoord.X + rint/freq && slopeIterator < neTiles.size() && i < rCoord.X && tileMap[lCoord.Y][i] && !tileMap[lCoord.Y - 1][i]; i++, slopeIterator++) {
//				tileMap[lCoord.Y - 1][i] = up ? new Tile(*neTiles[slopeIterator]) : new Tile(*seTiles[slopeIterator]);
//				tileMap[lCoord.Y - 1][i]->SetGridPosition(i, lCoord.Y - 1);
//				tileMap[lCoord.Y - 1][i]->TestDraw(cameraBounds);
//				allocatedSpots.push_back(tileMap[lCoord.Y - 1][i]);
//			}
//
//			up = !up;
//		}
//
//		if (platform.size() % rint > 0) {
//			if (!up) {
//				for (;i <= rCoord.X && slopeIterator < neTiles.size() && tileMap[lCoord.Y][i] && !tileMap[lCoord.Y - 1][i]; i++, slopeIterator++) {
//					tileMap[lCoord.Y - 1][i] = up ? new Tile(*neTiles[slopeIterator]) : new Tile(*seTiles[slopeIterator]);
//					tileMap[lCoord.Y - 1][i]->SetGridPosition(i, lCoord.Y - 1);
//					tileMap[lCoord.Y - 1][i]->TestDraw(cameraBounds);
//					allocatedSpots.push_back(tileMap[lCoord.Y - 1][i]);
//				}
//			}
//			else {
//				for (;i <= rCoord.X && slopeIterator >= 0 && tileMap[lCoord.Y][i] && !tileMap[lCoord.Y - 1][i]; i++, slopeIterator--) {
//					tileMap[lCoord.Y - 1][i] = up ? new Tile(*neTiles[slopeIterator]) : new Tile(*seTiles[slopeIterator]);
//					tileMap[lCoord.Y - 1][i]->SetGridPosition(i, lCoord.Y - 1);
//					tileMap[lCoord.Y - 1][i]->TestDraw(cameraBounds);
//					allocatedSpots.push_back(tileMap[lCoord.Y - 1][i]);
//				}
//			}
//		}
//			
//	}
//
//	std::cout << "Hills filled, and bodies created!" << std::endl;
//}

void TileManager::CreatePlatforms(std::vector<std::vector<int>>& pseudoMap, std::vector<Coord>& platformStarts)
{
	GaussianBlur* blur = new GaussianBlur(GAUSSIAN_FILTER_SIZE);
	DrunkardsWalk* miniWalk;
	std::vector<Coord> walk;

	for (int y = 0; y < pseudoMap.size(); y++) {
		for (int x = 0; x < pseudoMap[y].size(); x++) {
			int randWidth = std::clamp(rand(), PLATFORM_TOP_EDGE_MIN, PLATFORM_TOP_EDGE_MAX);
			int randHeight = 1;
			Coord platformStart(x,y);

			bool leftOpen, rightOpen, valid;

			leftOpen = rightOpen = valid = true;

			if (!IsInBounds(x + randWidth, y) 
				|| pseudoMap[y][x + randWidth] != 0
				|| pseudoMap[y][x] == 1) 
				if (!IsInBounds(x - 1, y) || !IsInBounds(x - 1, y))
					continue;

			for (int j = y - PLATFORM_TOP_EDGE_MIN / 2; j < y + PLATFORM_TOP_EDGE_MIN / 2; j++)
				for (int i = x - PLATFORM_TOP_EDGE_MIN / 2; i < x + PLATFORM_TOP_EDGE_MIN / 2; i++)
					if (!IsInBounds(i, j) || pseudoMap[j][i] != 0)
						leftOpen = false;

			for (int j = y - PLATFORM_TOP_EDGE_MIN / 2; j < y + PLATFORM_TOP_EDGE_MIN / 2; j++)
				for (int i = x + randWidth - PLATFORM_TOP_EDGE_MIN / 2; i < x + PLATFORM_TOP_EDGE_MIN / 2; i++)
					if (!IsInBounds(i, j) || pseudoMap[j][i] != 0)
						rightOpen = false;

			if (!rightOpen || !leftOpen)
				continue;

			randHeight = rand();
			randHeight = std::clamp(randHeight, PLATFORM_TOP_EDGE_MIN, PLATFORM_TOP_EDGE_MAX);

			for (int ix = x; valid && ix < x + randWidth && IsInBounds(ix, y) && randHeight > 0; ix++) {
				for (int iy = y; valid && iy < y + randHeight && IsInBounds(ix, iy) && randWidth > 0; iy++) {

					if (!IsInBounds(ix,iy) || randWidth < PLATFORM_TOP_EDGE_MIN || randHeight < PLATFORM_TOP_EDGE_MIN){
						valid = false; 
						continue;
					}

					if (!IsInBounds(ix - 1, iy) || pseudoMap[iy][ix - 1] == 1)
						platformStart.X++;

					if (!IsInBounds(ix + 1, iy) || pseudoMap[iy][ix + 1] == 1)
						randWidth = x - ix;

					if (!IsInBounds(ix, iy+1) || pseudoMap[iy + 1][ix] == 1)
						randHeight = iy - y - 1;

				}
			}

			if (!valid || randWidth < 1 || randHeight < 1)
				continue;

			for (int iy = y; iy < y + randHeight; iy++)
				for (int ix = x; ix < x + randWidth && IsInBounds(ix,iy); ix++)
					pseudoMap[iy][ix] = 1;

			miniWalk = new DrunkardsWalk(randWidth, randHeight);
			walk = miniWalk->Walk(rand() % (MIN(randWidth,randHeight) + 1), pseudoMap, Coord(x,y));

			for (auto coord : walk)
				pseudoMap[y + coord.Y][x + coord.X] = 0;

			platformStarts.push_back({y, x + randWidth});
			delete miniWalk;
			walk.clear();
		}
	}

	blur->BlurTileMap(pseudoMap);


	for (int y = 0; y < pseudoMap.size(); y++){
		for (int x = 0; x < pseudoMap[y].size(); x++) {
			SDL_Rect r{ x * 5,y * 5,5,5 };
			if (pseudoMap[y][x] == 0)
				SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255,255,255,255);
			else
				SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0,0,0,255);
			
			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
		}
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	//PrintMapToConsole(pseudoMap);

	delete blur;
}

std::unordered_set<Coord, PairHash> TileManager::GetWalkPerimeter(std::vector<Coord>& caveWalk)
{
	std::unordered_set<Coord, PairHash>p;
	int x, y;

	for (Coord step : caveWalk) {
		x = step.X;
		y = step.Y;

		//N Neighbor
		if (IsInBounds(x, y - 1) && tileMap[y - 1][x] && !p.contains(Coord(x, y - 1)))
			p.insert(Coord(x, y - 1));
		//E Neighbor
		if (IsInBounds(x + 1, y) && tileMap[y][x + 1] && !p.contains(Coord(x+1, y)))
			p.insert(Coord(x + 1, y));
		//S Neighbor
		if (IsInBounds(x, y + 1) && tileMap[y + 1][x] && !p.contains(Coord(x, y + 1)))
			p.insert(Coord(x, y + 1));
		//W Neighbor
		if (IsInBounds(x - 1, y) && tileMap[y][x - 1] && !p.contains(Coord(x-1,y)))
			p.insert(Coord(x - 1, y));

		////NE Neighbor
		if (IsInBounds(x + 1, y - 1) && tileMap[y - 1][x + 1] && !p.contains(Coord(x+1, y - 1)))
			p.insert(Coord(x + 1, y - 1));
		//SE Neighbor
		if (IsInBounds(x + 1, y + 1) && tileMap[y + 1][x + 1] && !p.contains(Coord(x+1, y + 1)))
			p.insert(Coord(x + 1, y + 1));
		//SW Neighbor
		if (IsInBounds(x - 1, y + 1) && tileMap[y + 1][x - 1] && !p.contains(Coord(x-1, y + 1)))
			p.insert(Coord(x - 1, y + 1));
		//NW Neighbor
		if (IsInBounds(x - 1, y - 1) && tileMap[y - 1][x - 1] && !p.contains(Coord(x-1, y - 1)))
			p.insert(Coord(x - 1, y - 1));

	}

	return p;
}

void TileManager::PrintMapToConsole(std::vector<std::vector<int>> const & pmap = {})
{
	std::vector<std::vector<int>> map;

	if (pmap.size() == 0) {
		for (int y = 0; y < tileMap.size(); y++) {
			std::vector<int>row;
			for (int x = 0; x < tileMap[y].size(); x++) {
				row.push_back(tileMap[y][x] != nullptr ? 1 : 0);
			}
			map.push_back(row);
		}
	}
	else {
		map = pmap;
	}



	system("cls");

	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

	consolesize.cbSize = sizeof(consolesize);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfoEx(hConsole, &consolesize);

	COORD c;
	c.X = tileMap[0].size();
	c.Y = tileMap.size();
	consolesize.dwSize = c;

	consolesize.srWindow.Left = 0;
	consolesize.srWindow.Right = tileMap[0].size();
	consolesize.srWindow.Top = 0;
	consolesize.srWindow.Bottom = tileMap.size();

	SetConsoleScreenBufferInfoEx(hConsole, &consolesize);

	for (int y = 0; y < map.size(); y++) {
		std::vector<int>row;
		for (int x = 0; x < map[y].size(); x++) {
			COORD coord;
			coord.X = x;
			coord.Y = y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			SetConsoleTextAttribute(hConsole, map[y][x]);
			std::cout << map[y][x];
		}
	}
	SetConsoleTextAttribute(hConsole, 0);
	system("cls");
}

void TileManager::CarveCaves(std::vector<std::vector<int>>& pseudoMap)
{
	GaussianBlur* blurStage = new GaussianBlur(GAUSSIAN_FILTER_SIZE);
	DrunkardsWalk* caveWalk = new DrunkardsWalk(worldCols, worldRows);
	std::vector<Coord> walk = caveWalk->Walk((rand() % DRUNK_WALK_MAX) + DRUNK_WALK_MIN, pseudoMap, Coord(-1,-1));
	std::unordered_set<Coord, PairHash> walkPerimeter;
	spawn = Vector2{ (walk[0].X * tileWidth * 1.0) + (tileWidth / 2), (walk[0].Y * tileHeight * 1.0) + (tileHeight / 2) };
	SDL_Rect testDraw{};


	graphicsRef->Vector2PixelsToMeters(spawn);

	SDL_RenderClear(graphicsRef->GetRenderer());

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255, 0, 0, 255);

	for (Coord coord : walk)
	{
		const SDL_Rect r(coord.X*5, coord.Y*5, 5, 5);

		Tile* t = tileMap[coord.Y][coord.X];

		tileMap[coord.Y][coord.X] = nullptr;

		delete t;

		SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	walkPerimeter = GetWalkPerimeter(walk);

	walk.clear();
	
	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);

	for (auto w : walkPerimeter) {
		testDraw = { w.X * 5, w.Y * 5, 5, 5 };
		SDL_RenderFillRect(graphicsRef->GetRenderer(), &testDraw);
		walk.push_back(w);
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());
	
	blurStage->BlurTileMap(pseudoMap);

	for (int y = 0; y < pseudoMap.size(); y++) {
		for (int x = 0; x < pseudoMap[y].size(); x++) {
			SDL_Rect rect(x * 5, y * 5, 5, 5);
			if(pseudoMap[y][x] == 0)	
				SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255, 255, 255, 255);
			else
				SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 255);

			SDL_RenderFillRect(graphicsRef->GetRenderer(), &rect);
		}
	}
 	SDL_RenderPresent(graphicsRef->GetRenderer());

	delete caveWalk;
}

void TileManager::CreateMapRenderTarget()
{
	Uint32 fmt;
	SDL_Renderer* ren = graphicsRef->GetRenderer();
	SDL_Texture* spriteSheetTexture = spriteSheet->GetTexture();
	SDL_QueryTexture(spriteSheet->GetTexture(), &fmt, nullptr, nullptr, nullptr);

	tileMapTexture = Sprite::CreateRenderTexture(worldCols * tileWidth, worldRows * tileHeight, graphicsRef, fmt);
	SDL_Texture* tex = tileMapTexture;
	

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

	SDL_RenderClear(ren);
	SDL_SetRenderTarget(ren, tex);

	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap[y].size(); x++) {

			SDL_Rect rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

			if (tileMap[y][x] != nullptr) {
				SDL_Rect sR = tileMap[y][x]->GetSourceRect();
				SDL_RendererFlip flip = tileMap[y][x]->GetFlipFlags();
				SDL_RenderCopyEx(ren, spriteSheetTexture, &sR, &rect, 0.f, nullptr, SDL_FLIP_NONE);
			}
		}
	}

	SDL_SetRenderTarget(ren, nullptr);
	SDL_RenderClear(ren);
}

void TileManager::CreateTileMapBodies(std::vector<std::vector<int>>& pseudoMap)
{
}

TileManager::TileManager(const char* filepath, const std::shared_ptr<Graphics>& graphics, b2World* world, Vector2 playerDim)
{
	std::vector<Tile*> tileRow;
	Tile* newTile;
	int x = 0, y = 0;

	playerDimensions = playerDim;

	graphicsRef = graphics;

	physics = world;

	worldSize = graphics->GetScreenDimensions();
	tileMapTexture = nullptr;

	tileMapTextureDrawPosition = Vector2(0, 0);

	groundTiles = new TileCollection();
	northEastHillTiles = new TileCollection();
	southEastHillTiles = new TileCollection();
	//platformTiles = new TileCollection();

	TileParseTypesFromJSON("data/tilemap/Grassland/grassTiles.json");

	worldRows = std::clamp(rand() % (int)(worldSize.y / 2), WORLD_ROW_MIN, WORLD_ROW_MAX);
	worldCols = std::clamp(rand() % (int)(worldSize.x / 2), WORLD_COL_MIN, WORLD_COL_MAX);
}

//Time to worry about the destructor again
TileManager::~TileManager()
{
	while (!tileMap.empty()) {
		std::vector<Tile*> row = tileMap.front();

		while (!row.empty()) {
			Tile* t = row.front();
			if (t) {
				if (t->GetBodyReference()) {
					physics->DestroyBody(t->GetBodyReference());
				}
				delete t;
			}
			row.erase(row.begin());
		}
		tileMap.erase(tileMap.begin());
	}
	tileMap.clear();

	delete groundTiles;
	delete southEastHillTiles;
	delete northEastHillTiles;
	//delete platformTiles;

	if(spriteSheet)
	delete spriteSheet;

	if(tileMapTexture)
	SDL_DestroyTexture(tileMapTexture);

	graphicsRef.reset();
}

void TileManager::UpdateMap()
{

}

void TileManager::DrawMap(Vector2 cameraOffset, SDL_Rect& cameraBounds)
{
	Vector2 sDim = graphicsRef->GetScreenDimensions();
	SDL_Rect srcRect(cameraOffset.x, cameraOffset.y, sDim.x, sDim.y);

	srcRect.x = std::clamp(cameraOffset.x, 0.0, worldCols * tileWidth * 1.0);
	srcRect.y = std::clamp(cameraOffset.y, 0.0, worldRows * tileHeight * 1.0);

	SDL_RenderCopy(graphicsRef->GetRenderer(),
		tileMapTexture,
		&srcRect,
		nullptr);

}

bool TileManager::IsInCameraBounds(Tile* t, SDL_Rect cameraBounds)
{
	Vector2 tPos = t->GetPixelPosition();

	return tPos.x + tileWidth >= cameraBounds.x
		&& tPos.x < cameraBounds.x + cameraBounds.w
		&& tPos.y + tileHeight >= cameraBounds.y
		&& tPos.y < cameraBounds.y + cameraBounds.h;
}

std::vector<std::vector<Tile*>>* TileManager::GenerateTileMap(b2World* physicsWorld, Vector2 pDim)
{
	SDL_Renderer* ren = graphicsRef->GetRenderer();
	Uint32 fmt;
	std::vector<Tile*>* groundTilesFullCapped = groundTiles->FindTilesOfDirection((Direction)(Direction::East|Direction::West));
	std::vector<std::vector<int>> pseudoMap;
	std::vector<Coord> platformStarts;
	bounds = Vector4(0, 0, worldCols * tileWidth, worldRows * tileHeight);

	tileMap.resize(worldRows);
	
	for(int y = 0; y < tileMap.size(); y++)
		tileMap[y].resize(worldCols);

	for (int row = 0; row < tileMap.size(); row++) {
		std::vector<int> pRow;
		for (int col = 0; col < tileMap[row].size(); col++) {
			tileMap[row][col] = new Tile(
				*groundTilesFullCapped->at(
					(int)(rand() % groundTilesFullCapped->size())
				)
			);
		
			tileMap[row][col]->SetGridPosition(col, row);
			pRow.push_back(1);
		}
		pseudoMap.push_back(pRow);
	}

	CarveCaves(pseudoMap);

	CreatePlatforms(pseudoMap, platformStarts);

	//FillHills(pseudoMap, platformStarts);

	CreateMapRenderTarget();

	return &tileMap;
}

bool TileManager::IsInBounds(int x, int y)
{
	return y >= 0 && y < tileMap.size() && x >= 0 && x < tileMap[y].size();
}

bool TileManager::IsOfPlatform(int x, int y) {
	return tileMap[y][x] != nullptr && !IsInBounds(x,y);
}

void TileManager::PlatformDFS(std::vector<std::vector<int>>& pmap, int x, int y, int & platformFlag)
{
	std::stack<Coord> stk;
	stk.push(Coord(x, y));
	while (!stk.empty()) {
		auto curr = stk.top();
		stk.pop();

		int xi = curr.X;
		int yi = curr.Y;
		if (xi < 0 || xi >= pmap[y].size() || yi < 0 || yi >= pmap.size()) {
			platformFlag = 0;
			continue;
		}

		if (pmap[yi][xi] == 1) {

			pmap[yi][xi] = 2;
			stk.push(Coord(xi - 1, yi));
			stk.push(Coord(xi, yi - 1));
			stk.push(Coord(xi, yi + 1));
			stk.push(Coord(xi + 1, yi));
		}
	}
}

std::vector<std::vector<SDL_Color>> TileManager::CopyRectOfTilePixelsFromTexture(SDL_Rect* sR)
{
	std::vector<std::vector<SDL_Color>> rect;
	std::vector<SDL_Color> row;

	for (int y = sR->y; y < sR->y + sR->h; y++) {
		auto yit = std::next(spriteSheetPixels.begin(), y);
		auto xsit = std::next(yit->begin(), sR->x);
		auto xeit = std::next(yit->begin(), sR->x + sR->w);

		row.clear();

		row.assign(xsit, xeit);

		rect.push_back(row);
	}

	return rect;
}

TileCollection::TileCollection()
{ 
	root = new TileNode();
	root->direction = Direction::None;
	root->child = nullptr;
}

TileCollection::~TileCollection()
{
	while (root->child) {
		TileNode* cur = root->child;
		TileNode* prev = root;

		while (cur->child) {
			prev = cur;
			cur = cur->child;			
		}

		while (!cur->tiles.empty()) {
			Tile* t = cur->tiles.back();

			delete t;

			cur->tiles.pop_back();
		}

		delete cur;
		prev->child = nullptr;
	}
	
	while (!root->tiles.empty()) {
		Tile* t = root->tiles.back();

		delete t;

		root->tiles.pop_back();
	}

	delete root;
}

void TileCollection::AddTile(Tile* newTile)
{
	TileNode* cur = root;
	TileNode* prev = nullptr;
	float* slopes = newTile->GetSlopes();
	Direction newTileDir = newTile->GetCappingDirection();
	bool found = false;

	while(cur){
		if ((cur->direction & newTileDir) == newTileDir) {
			if (cur->tiles.empty())
				cur->tiles.push_back(newTile);
			else if (slopes[0] < cur->tiles.front()->GetSlopes()[0])
				cur->tiles.insert(cur->tiles.begin(), newTile);
			else
				cur->tiles.push_back(newTile);

			return;
		}

		prev = cur;
		cur = cur->child;
	}

	prev->child = new TileNode();
	cur = prev->child;
	cur->direction = newTileDir;
	cur->tiles.push_back(newTile);
}

std::vector<Tile*>* TileCollection::FindTilesOfDirection(Direction dir)
{
	TileNode* cur = root;

	while (cur) {
		if ((cur->direction & dir) == dir)
			return &cur->tiles;

		cur = cur->child;
	}

	return nullptr;
}

Tile* TileCollection::FindNthTileOfDirection(Direction dir, uint16_t index)
{
	std::vector<Tile*>* tiles = FindTilesOfDirection(dir);

	if (index < tiles->size())
		return (*tiles)[index];

	return nullptr;
}
