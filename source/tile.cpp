#include "tile.h"
#include "drunkard.h"

#include <algorithm>
#include <random>
#include <complex>

extern "C" {
#include "simple_json.h"
}

#define EDGE_COUNT 2
#define MAX_CAPS 2

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

void Tile::TestDraw()
{
	Vector2 scale;
	Vector2 scaleCenter;
	Vector3 rot{ 0,0,0 };
	Vector4 color = { 255,255,255,255 };
	SDL_Rect dstRect;
	Vector2 sDim = graphicsRef->GetScreenDimensions();
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, camX = 0, camY = 0, ite = 0;

	std::vector<std::vector<b2Vec2>> chains;
	chains.push_back(topChain);
	chains.push_back(bottomChain);
	if (!eastCap.empty())
		chains.push_back(eastCap);
	if (!westCap.empty())
		chains.push_back(westCap);

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
	sprite->Draw(Vector2(dstRect.x, dstRect.y), sourceRect, flipFlags);
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


//Work on carving paths
//void TileManager::CarvePath()
//{
//	PerlinNoise *perlin = new PerlinNoise(Vector2(tileMap[0].size(), tileMap.size()));
//	std::vector<float> perlin1D = perlin->PerlinNoise1D();
//
//
//	for (int col = 1; col < perlin1D.size()-1; col++) {
//		int perlinVal = (perlin1D[col] * tileMap.size());
//		Tile* t = tileMap[perlinVal][col];
//		int stepN = (int)(gf2d_random() * 10) % (perlinVal * -1) - 1;
//		int stepE = (int)(gf2d_random() * 10) % (tileMap[perlinVal].size() - col) - 1;
//		int stepS = (int)(gf2d_random() * 10) % (tileMap.size() - perlinVal) - 1;
//		int stepW = (int)(gf2d_random() * 10) % (col * -1) - 1;
//		
//		tileMap[perlinVal][col] = nullptr;
//		delete t;
//
//		for(int n = stepN; n < 0; n++) {
//			t = tileMap[perlinVal+n][col];
//			tileMap[perlinVal+n][col] = nullptr;
//			delete t;
//		}
//		for (int e = stepE + col; e > 0; e--) {
//			t = tileMap[perlinVal][e];
//			tileMap[perlinVal][e] = nullptr;
//			delete t;
//		}
//		for (int s = stepS + perlinVal; s > 0; s--) {
//			t = tileMap[s][col];
//			tileMap[s][col] = nullptr;
//			delete t;
//		}
//		for (int w = stepW; w < 0; w++) {
//			t = tileMap[perlinVal][col + w];
//			tileMap[perlinVal][col + w] = nullptr;
//			delete t;
//		}
//	}
//}

void TileManager::FillHills(std::vector<std::pair<int, int>>& caveWalk)
{
	std::vector<Tile*> allocatedSpots;
	Tile* cTile;
	Tile* ne = nullptr;
	Tile* se = nullptr;
	Tile* sw = nullptr;
	Tile* nw = nullptr;
	Tile* north = nullptr;
	Tile* south = nullptr;
	Tile* east = nullptr;
	Tile* west = nullptr;
	int northSize, southSize;
	int ni, si, niStart, siStart;
	std::vector<std::vector<SDL_Color>> pixels;
	std::vector<Tile*> neTiles = *northEastHillTiles->FindTilesOfDirection((Direction)(East | West));
	std::vector<Tile*> seTiles = *southEastHillTiles->FindTilesOfDirection((Direction)(East | West));
	SDL_Rect sR;
	SDL_Rect dR;

	si = ni = rand() % neTiles.size();

	northSize = neTiles.size();
	southSize = seTiles.size();

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 255, 255);

	//for (int y = 2; y < tileMap.size() - 2; y++) {
	//	for (int x = 2; x < tileMap[y].size() - 2; x++) {
	for (std::pair<int, int> walk : caveWalk) {
		int x = walk.first;
		int y = walk.second;
		Tile* north = nullptr;
		Tile* south = nullptr;
		Tile* east = nullptr;
		Tile* west = nullptr;
		Tile* northwest = nullptr;
		Tile* northeast = nullptr;
		Tile* southwest = nullptr;
		Tile* southeast = nullptr;

		if (!tileMap[y][x])
			continue;

		//tileMap[y][x]->CreateTileBody(physics);

		niStart = ni = rand() % northSize;
		siStart = si = rand() % southSize;

		pixels.clear();

		north = (tileMap[y - 1][x] != nullptr) ? tileMap[y - 1][x] : nullptr;

		if (std::find(allocatedSpots.begin(), allocatedSpots.end(), north) != allocatedSpots.end())
			continue;

		south = (tileMap[y + 1][x] != nullptr) ? tileMap[y + 1][x] : nullptr;

		if (std::find(allocatedSpots.begin(), allocatedSpots.end(), south) != allocatedSpots.end())
			continue;

		east = (tileMap[y][x + 1] != nullptr) ? tileMap[y][x + 1] : nullptr;

		if (std::find(allocatedSpots.begin(), allocatedSpots.end(), east) != allocatedSpots.end())
			continue;

		west = (tileMap[y][x - 1] != nullptr) ? tileMap[y][x - 1] : nullptr;

		if (std::find(allocatedSpots.begin(), allocatedSpots.end(), west) != allocatedSpots.end())
			continue;

		northwest = (tileMap[y - 1][x - 1] != nullptr) ? tileMap[y - 1][x - 1] : nullptr;
		southwest = (tileMap[y + 1][x - 1] != nullptr) ? tileMap[y + 1][x - 1] : nullptr;
		northeast = (tileMap[y - 1][x + 1] != nullptr) ? tileMap[y - 1][x + 1] : nullptr;
		southeast = (tileMap[y + 1][x + 1] != nullptr) ? tileMap[y + 1][x + 1] : nullptr;

		if (!north && south) {
			if ((east != nullptr) != (west != nullptr)) {
				//EAST NOT NULL ----->
				if (east != nullptr && !west) {
					//While going west
					while (!west && south && si < southSize && std::find(allocatedSpots.begin(), allocatedSpots.end(), south) == allocatedSpots.end()) {
						if (!southwest && !tileMap[y + 1][x - 2]) {
							break;
						}

						cTile = tileMap[y][x];

						if (cTile && cTile->GetBodyReference()) {
							physics->DestroyBody(cTile->GetBodyReference());

							delete cTile;
						}

						tileMap[y][x] = cTile = new Tile(*seTiles[si]);
						allocatedSpots.push_back(cTile);

						//tileMap[y][x]->TilePhysicsInit(physics, playerDimensions);

						tileMap[y][x]->CreateTileBody(physics);
						tileMap[y][x]->TestDraw();
						tileMap[y][x]->SetGridPosition(x, y);

						si++;

						dR = SDL_Rect(x * 5, y * 5, 5, 5);

						SDL_RenderFillRect(graphicsRef->GetRenderer(), &dR);
						SDL_RenderPresent(graphicsRef->GetRenderer());

						x--;

						south = (tileMap[y + 1][x] != nullptr) ? tileMap[y + 1][x] : nullptr;
						west = (tileMap[y][x - 1] != nullptr) ? tileMap[y][x - 1] : nullptr;
						southwest = (tileMap[y + 1][x - 1] != nullptr) ? tileMap[y + 1][x - 1] : nullptr;

					}

					ni = std::min<int>(si, northSize - 1);

					while (!west && south && ni >= 0 && std::find(allocatedSpots.begin(), allocatedSpots.end(), south) == allocatedSpots.end()) {
						if (!southwest && !tileMap[y + 1][x - 2]) {
							break;
						}
						cTile = tileMap[y][x];

						if (cTile && cTile->GetBodyReference()) {
							physics->DestroyBody(cTile->GetBodyReference());

							delete cTile;
						}

						tileMap[y][x] = cTile = new Tile(*neTiles[ni]);
						allocatedSpots.push_back(cTile);

						//tileMap[y][x]->TilePhysicsInit(physics, playerDimensions);
						tileMap[y][x]->CreateTileBody(physics);

						tileMap[y][x]->TestDraw();
						tileMap[y][x]->SetGridPosition(x, y);
						ni--;

						dR = SDL_Rect(x * 5, y * 5, 5, 5);

						SDL_RenderFillRect(graphicsRef->GetRenderer(), &dR);
						SDL_RenderPresent(graphicsRef->GetRenderer());

						x--;

						south = (tileMap[y + 1][x] != nullptr) ? tileMap[y + 1][x] : nullptr;
						west = (tileMap[y][x - 1] != nullptr) ? tileMap[y][x - 1] : nullptr;
						southwest = (tileMap[y + 1][x - 1] != nullptr) ? tileMap[y + 1][x - 1] : nullptr;
					}
				}
				//WEST NOT NULL ----->
				else {
					
					//GOING EAST
					while (!east && south && ni < northSize && std::find(allocatedSpots.begin(), allocatedSpots.end(), south) == allocatedSpots.end()) {
						if (!southeast && !tileMap[y + 1][x + 2]) {
							break;
						}

						cTile = tileMap[y][x];

						if (cTile && cTile->GetBodyReference()) {
							physics->DestroyBody(cTile->GetBodyReference());

							delete cTile;
						}

						tileMap[y][x] = cTile = new Tile(*neTiles[ni]);
						allocatedSpots.push_back(cTile);

						tileMap[y][x]->TilePhysicsInit(physics, playerDimensions);
						tileMap[y][x]->CreateTileBody(physics);

						//tileMap[y][x]->TestDraw();
						tileMap[y][x]->SetGridPosition(x, y);
						ni++;

						dR = SDL_Rect(x * 5, y * 5, 5, 5);

						SDL_RenderFillRect(graphicsRef->GetRenderer(), &dR);
						SDL_RenderPresent(graphicsRef->GetRenderer());

						x++;

						south = (tileMap[y + 1][x] != nullptr) ? tileMap[y + 1][x] : nullptr;
						east = (tileMap[y][x+1] != nullptr) ? tileMap[y][x + 1] : nullptr;
						southeast = (tileMap[y + 1][x + 1] != nullptr) ? tileMap[y + 1][x + 1] : nullptr;
					}
					
					//FLIP HILLS

					si = std::min<int>(ni, southSize - 1);
						
					while (!east && south && si >= 0 && std::find(allocatedSpots.begin(), allocatedSpots.end(), south) == allocatedSpots.end()) {
						if (!southeast && !tileMap[y + 1][x + 2]) {
							break;
						}
						
						cTile = tileMap[y][x];

						if (cTile && cTile->GetBodyReference()) {
							physics->DestroyBody(cTile->GetBodyReference());

							delete cTile;
						}

						tileMap[y][x] = cTile = new Tile(*seTiles[si]);
						allocatedSpots.push_back(cTile);

						//tileMap[y][x]->TilePhysicsInit(physics, playerDimensions);
						tileMap[y][x]->CreateTileBody(physics);

						tileMap[y][x]->TestDraw();
						tileMap[y][x]->SetGridPosition(x, y);
						si--;

						dR = SDL_Rect(x * 5, y * 5, 5, 5);

						SDL_RenderFillRect(graphicsRef->GetRenderer(), &dR);
						SDL_RenderPresent(graphicsRef->GetRenderer());

						x++;

						south = (tileMap[y + 1][x] != nullptr) ? tileMap[y + 1][x] : nullptr;
						east = (tileMap[y][x+1] != nullptr) ? tileMap[y][x + 1] : nullptr;
						southeast = (tileMap[y + 1][x + 1] != nullptr) ? tileMap[y + 1][x + 1] : nullptr;
					}
				}
			}
		}
	}
}

std::unordered_set<std::pair<int, int>, PairHash> TileManager::GetWalkPerimeter(std::vector<std::pair<int, int>>& caveWalk)
{
	std::unordered_set<std::pair<int, int>, PairHash>p;
	int x, y;

	for (std::pair<int, int> step : caveWalk) {
		x = step.first;
		y = step.second;

		//N Neighbor
		if (InBounds(x, y - 1) && tileMap[y - 1][x] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x, y - 1));
		//E Neighbor
		if (InBounds(x + 1, y) && tileMap[y][x + 1] && !p.contains(std::pair<int, int>(x+1, y)))
			p.insert(std::pair<int, int>(x + 1, y));
		//S Neighbor
		if (InBounds(x, y + 1) && tileMap[y + 1][x] && !p.contains(std::pair<int, int>(x, y + 1)))
			p.insert(std::pair<int, int>(x, y + 1));
		//W Neighbor
		if (InBounds(x - 1, y) && tileMap[y][x - 1] && !p.contains(std::pair<int, int>(x-1,y)))
			p.insert(std::pair<int, int>(x - 1, y));

		////NE Neighbor
		if (InBounds(x + 1, y - 1) && tileMap[y - 1][x + 1] && !p.contains(std::pair<int, int>(x+1, y - 1)))
			p.insert(std::pair<int, int>(x + 1, y - 1));
		//SE Neighbor
		if (InBounds(x + 1, y + 1) && tileMap[y + 1][x + 1] && !p.contains(std::pair<int, int>(x+1, y + 1)))
			p.insert(std::pair<int, int>(x + 1, y + 1));
		//SW Neighbor
		if (InBounds(x - 1, y + 1) && tileMap[y + 1][x - 1] && !p.contains(std::pair<int, int>(x-1, y + 1)))
			p.insert(std::pair<int, int>(x - 1, y + 1));
		//NW Neighbor
		if (InBounds(x - 1, y - 1) && tileMap[y - 1][x - 1] && !p.contains(std::pair<int, int>(x-1, y - 1)))
			p.insert(std::pair<int, int>(x - 1, y - 1));

	}

	return p;
}

void TileManager::CarveCaves()
{

	DrunkardsWalk* caveWalk = new DrunkardsWalk(worldCols, worldRows);
	std::vector<std::pair<int, int>> walk = caveWalk->Walk();
	std::unordered_set<std::pair<int, int>, PairHash> walkPerimeter;
	spawn = Vector2{ (walk[0].first * tileWidth * 1.0) + (tileWidth / 2), (walk[0].second * tileHeight * 1.0) + (tileHeight / 2) };
	SDL_Rect testDraw{};


	graphicsRef->Vector2PixelsToMeters(spawn);

	SDL_RenderClear(graphicsRef->GetRenderer());

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255, 0, 0, 255);

	for (std::pair<int, int> coord : walk)
	{
		const SDL_Rect r(coord.first*5, coord.second*5, 5, 5);

		Tile* t = tileMap[coord.second][coord.first];

		tileMap[coord.second][coord.first] = nullptr;

		delete t;


		SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	walkPerimeter = GetWalkPerimeter(walk);

	walk.clear();

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);

	for (auto w : walkPerimeter) {
		tileMap[w.second][w.first]->CreateTileBody(physics);
		testDraw = { w.first * 5, w.second * 5, 5, 5 };
		SDL_RenderFillRect(graphicsRef->GetRenderer(), &testDraw);
		walk.push_back(w);
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	FillHills(walk);

	delete caveWalk;

	SDL_RenderPresent(graphicsRef->GetRenderer());

	SDL_RenderClear(graphicsRef->GetRenderer());
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

	worldRows = worldSize.y / PIX_IN_MET;
	worldCols = worldSize.x / PIX_IN_MET;
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
	
	bounds = Vector4(0, 0, worldCols * tileWidth, worldRows * tileHeight);

	tileMap.resize(worldRows);
	
	for(int y = 0; y < tileMap.size(); y++)
		tileMap[y].resize(worldCols);

	for (int row = 0; row < tileMap.size(); row++) {
		for (int col = 0; col < tileMap[row].size(); col++) {
			tileMap[row][col] = new Tile(
				*groundTilesFullCapped->at(
					(int)(rand() % groundTilesFullCapped->size())
				)
			);
		
			tileMap[row][col]->SetGridPosition(col, row);
		}
	}

	CarveCaves();

	CreateMapRenderTarget();

	//Carve out walkable tunnel
	//for (int i = 0; i < MAX_TUNNELS; i += (int)((int)(gf2d_random() * 10) % MAX_TUNNELS)/2)
		//CarvePath();

	return &tileMap;
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
