#include "tile.h"
#include "drunkard.h"

#include <algorithm>
#include <random>

extern "C" {
#include "simple_json.h"
}

#define EDGE_COUNT 2
#define MAX_CAPS 2

void Tile::CreatePhysicsEdges(std::vector<std::vector<SDL_Color>>& pixels, Vector2 playerDimensions)
{
	SDL_Rect boundingRect = {INT_MAX, INT_MAX, -1, -1};
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (topChain.size() > 0 && bottomChain.size() > 0)
		return;

	if (direction == Direction::North) {
		//scan north -> south
		std::cout << "Scanning north to south" << std::endl;
		for (col = 0; col < pixels[row].size(); col++) {
			for (; row < pixels.size(); row++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2((col) * MET_IN_PIX, (row) * MET_IN_PIX );
					topChain.push_back(vert);
					col += (playerDimensions.x / 2);
					break;
				}
			}

			row = 0;
		}

		for (col = pixels[row].size()-1, row = 0; row < pixels.size(); row++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2((col) * MET_IN_PIX, (row) * MET_IN_PIX);
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
					vert = b2Vec2((col) * MET_IN_PIX, (row) * MET_IN_PIX);
					bottomChain.push_back(vert);
					col += (playerDimensions.x/ 2);
					break;
				}
			}
			row = pixels.size() - 1;
		}
			

		for (col = pixels[row].size() - 1, row = pixels.size() - 1; row >= 0; row--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2((col) * MET_IN_PIX, (row) * MET_IN_PIX);
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
					row += (playerDimensions.x/ 2);
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
					row += (playerDimensions.x/ 2);
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
					row += (playerDimensions.x/ 2);
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
					row += (playerDimensions.x/ 2);
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


	switch (flipFlags) {
		case (SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL):
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}
		break;

		case SDL_FLIP_VERTICAL:
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		break;

		case SDL_FLIP_HORIZONTAL:
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert  < vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}

		std::reverse(topChain.begin(), topChain.end());
		std::reverse(bottomChain.begin(), bottomChain.end());

		break;

		default:
		break;
	}
}

void Tile::DecideCapping(std::vector<std::vector<SDL_Color>>& pixels)
{
	b2Vec2 vert;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> cappings;
	int row, col;
	SDL_Rect r = sourceRect;

	//Decide to cap cardinal direction(s)
	if (capDirection != Direction::None) {
		if (capDirection & Direction::East) {

			eastCap.push_back(topChain.back());
			eastCap.push_back(bottomChain.back());
		}

		if (capDirection & Direction::West) {
			westCap.push_back(bottomChain.front());
			westCap.push_back(topChain.front());
		}
	}
}

void Tile::TilePhysicsInit(b2World* world, Vector2 playerDim, std::vector<std::vector<SDL_Color>>& pixels)
{
	Vector2 tmp;
	tmp = pixelPosition;

	graphicsRef->Vector2PixelsToMeters(tmp);

	worldPosition = {
		(float)tmp.x,
		(float)tmp.y
	};

	CreatePhysicsEdges(pixels, playerDim);

	//Logic for ghost vertices ( previous and next ) goes here!!!

	DecideCapping(pixels);
}

void Tile::SetSDL_RendererFlipFlags(SDL_RendererFlip flip)
{
	flipFlags = flip;
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

void Tile::CreateTileBody(b2World* world, b2Vec2 tpg, b2Vec2 tng, b2Vec2 bpg, b2Vec2 bng)
{
	b2BodyDef bd;
	b2FixtureDef fd;
	Vector2 tV;
	b2ChainShape c1, c2, c3, c4;
	b2Vec2 center;

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
	RotateChain(topChain, bd.angle);
	RotateChain(bottomChain, bd.angle);
	RotateChain(westCap, bd.angle);
	RotateChain(eastCap, bd.angle);

	FlipChain(flipFlags, topChain);
	FlipChain(flipFlags, bottomChain);
	FlipChain(flipFlags, westCap);
	FlipChain(flipFlags, eastCap);


	if(flipFlags == SDL_FLIP_HORIZONTAL)
		for (int i = 0; i < MAX_EDGES; i++)
			slopes[i] *= -1;

	if(flipFlags == SDL_FLIP_VERTICAL)
		for (int i = 0; i < MAX_EDGES; i++)
			slopes[i] *= -1;
	

	//Need to add prev and next ghost vertices
	c1.CreateChain(static_cast<b2Vec2*>(topChain.data()), topChain.size(), tpg, tng);

	fd.shape = &c1;
	fd.friction = 0.7f;
	physicsBody->CreateFixture(&fd);

	std::reverse(bottomChain.begin(), bottomChain.end());
	c2.CreateChain(static_cast<b2Vec2*>(bottomChain.data()), bottomChain.size(), bpg, bng);

	fd.shape = &c2;
	fd.friction = 0.7f;
	physicsBody->CreateFixture(&fd);

	if (eastCap.size() > 0) {
		Uint8 flag = 0;
		for (int i = 0, j = eastCap.size() - 1; i < j; i++, j--)
			if (eastCap[i].x == eastCap[j].x && eastCap[i].y == eastCap[j].y)
				flag = 1;
		if (!flag) {
			c3.CreateChain(static_cast<b2Vec2*>(eastCap.data()), eastCap.size(), *(topChain.begin()), *(bottomChain.begin()));
			fd.shape = &c3;
			fd.friction = 0.7f;
			physicsBody->CreateFixture(&fd);
		}
	}

	if (westCap.size() > 0) {
		for (int i = 0, j = westCap.size() - 1; i < j; i++, j--)
			if (westCap[i].x == westCap[j].x && westCap[i].y == westCap[j].y)
				return;

		//std::reverse(westCap.begin(), westCap.end());
		c4.CreateChain(static_cast<b2Vec2*>(westCap.data()), westCap.size(), *(topChain.end()-1), *(bottomChain.end()-1));
		fd.shape = &c4;
		fd.friction = 0.7f;
		physicsBody->CreateFixture(&fd);
	}
}

void Tile::SetCappingDirection(Direction capping)
{
	this->capDirection = capping;
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

	spriteSheet = nullptr;
			   
	graphicsRef = nullptr;

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	zRot = 0.f;

	debugColor = SDL_Color(0, 255, 0, 255);

	hillOrientation = Direction::None;
	slopes = nullptr;
}

Tile::Tile(int id, const std::shared_ptr<Sprite>& s, Vector2 gridPosition, Vector2 dim, Direction dir, const std::shared_ptr<Graphics>& graphics, float zRotation, SDL_Rect srcRect, float* slopes)
{
	this->id = id;

	sourceRect = SDL_Rect(srcRect);

	graphicsRef = graphics;
	spriteSheet = s;

	pixelDimensions = dim;

	worldDimensions.Set(dim.x * MET_IN_PIX, dim.y * MET_IN_PIX);

	pixelPosition = { gridPosition.x * pixelDimensions.x, gridPosition.y * pixelDimensions.y };
	direction = dir;
	zRot = zRotation;

	debugColor = SDL_Color(0, 255, 0, 255);
	this->slopes = slopes;
}

Tile::Tile(const Tile &oldTile)
{
	this->id = oldTile.id;
	this->direction = oldTile.direction;
	this->pixelDimensions = oldTile.pixelDimensions;
	this->worldDimensions = oldTile.worldDimensions;

	this->pixelPosition = { 0,0 };

	this->worldPosition = { 0,0 };

	this->physicsBody = nullptr;

    this->graphicsRef = (const std::shared_ptr<Graphics>&)oldTile.graphicsRef;
	this->spriteSheet = (const std::shared_ptr<Sprite>&)oldTile.spriteSheet;

	this->possibleConnections = oldTile.possibleConnections;

	this->capDirection = oldTile.capDirection;
	this->direction = oldTile.direction;

	this->zRot = oldTile.zRot;

	this->debugColor = oldTile.debugColor;

	this->sourceRect = oldTile.sourceRect;

	std::copy(oldTile.topChain.begin(), oldTile.topChain.end(), std::back_inserter(this->topChain));
	std::copy(oldTile.bottomChain.begin(), oldTile.bottomChain.end(), std::back_inserter(this->bottomChain));
	std::copy(oldTile.eastCap.begin(), oldTile.eastCap.end(), std::back_inserter(this->eastCap));
	std::copy(oldTile.westCap.begin(), oldTile.westCap.end(), std::back_inserter(this->westCap));

	this->slopes = oldTile.slopes;
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
	this->spriteSheet = rhs.spriteSheet;

	this->possibleConnections = rhs.possibleConnections;

	this->capDirection = rhs.capDirection;
	this->direction = rhs.direction;

	this->zRot = rhs.zRot;

	this->debugColor = rhs.debugColor;

	this->sourceRect = rhs.sourceRect;

	this->topChain = rhs.topChain;
	this->bottomChain = rhs.bottomChain;
	this->eastCap = rhs.eastCap;
	this->westCap = rhs.westCap;
	this->slopes = rhs.slopes;
	return *this;
}

Tile::~Tile()
{
	possibleConnections.clear();

	spriteSheet.reset();
	graphicsRef.reset();
	topFix = nullptr;
	bottomFix = nullptr;
	eastFix = nullptr;
	westFix = nullptr;

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

	spriteSheet->Draw(Vector2(dstRect.x, dstRect.y), sourceRect, &scale, &scaleCenter, &color, flipFlags);
}

void Tile::TestDraw()
{
	Vector2 scale;
	Vector2 scaleCenter;
	Vector3 rot{ 0,0,0 };
	Vector4 color = { 255,255,255,255 };
	SDL_Rect dstRect;
	Vector2 sDim = graphicsRef->GetScreenDimensions();

	scale = { 1,1 };

	scaleCenter = {
		sourceRect.w / 2.0f,
		sourceRect.h / 2.0f
	};

	dstRect = { (int)(sDim.x / 2 - pixelDimensions.x / 2),
		(int)(sDim.y / 2 - pixelDimensions.y / 2),
		(int)pixelDimensions.x, 
		(int)pixelDimensions.y 
	};
	SDL_RenderClear(graphicsRef->GetRenderer());
	spriteSheet->Draw(Vector2(dstRect.x, dstRect.y), sourceRect, &scale, &scaleCenter, &color, flipFlags);
	SDL_RenderPresent(graphicsRef->GetRenderer());
}

std::vector<std::vector<SDL_Color>> Tile::GetTilePixels()
{
	SDL_Rect sR = GetSourceRect();
	
	return Sprite::GetPixelData(spriteSheet->GetFilePath().c_str(), &sR, graphicsRef);
}

void Tile::FlipChain(SDL_RendererFlip flip, std::vector<b2Vec2> &chain)
{
	switch (flip) {
	case SDL_FLIP_HORIZONTAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			chain[i].x *= -1;
		break;
	case SDL_FLIP_VERTICAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			chain[i].y *= -1;
		break;
	case SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			chain[j] *= -1;
		break;
	default:
		break;
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
	Tile* t = nullptr;
	Tile* tE = nullptr;
	Tile* tW = nullptr;
	Tile* tF = nullptr;

	Tile* xMirrorHillTile = nullptr;
	Tile* xMirrorHillTileE = nullptr;
	Tile* xMirrorHillTileW = nullptr;
	Tile* xMirrorHillTileF = nullptr;

	Tile* yMirrorHillTile = nullptr;
	Tile* yMirrorHillTileE = nullptr;
	Tile* yMirrorHillTileW = nullptr;
	Tile* yMirrorHillTileF = nullptr;


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

	spriteSheet = std::shared_ptr<Sprite>(
		new Sprite(tss->filepath, 
			tss->imageWidth, 
			tss->imageHeight, 
			graphicsRef));
	tss->sheet = spriteSheet.get();
	tileWidth = tss->tileWidth;
	tileHeight = tss->tileHeight;

	imageRect = { 0,0,(int)tss->imageWidth, (int)tss->imageHeight };

	spriteSheetPixels = spriteSheet.get()->GetPixelData(tss->filepath, &imageRect, graphicsRef);

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < sj_array_get_count(genDescList); i++) {
		int layerCount = 0;
		float** slopes = new float*[MAX_EDGES];


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

		for (int i = 0; i < slopeCount; i++)
			 sj_get_float_value(sj_array_get_nth(tileSlopes, slopeCount), slopes[i]);

		if (!spriteSheet.get()->CheckIfViableTexture(sR))
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
			*slopes);
		t = new Tile(*t);
		tE = new Tile(*t);
		tW = new Tile(*t);
		tF = new Tile(*t);
		xMirrorHillTile = new Tile(*t);
		xMirrorHillTileE = new Tile(*t);
		xMirrorHillTileW = new Tile(*t);
		xMirrorHillTileF = new Tile(*t);
		yMirrorHillTile = new Tile(*t);
		yMirrorHillTileE = new Tile(*t);
		yMirrorHillTileW = new Tile(*t);
		yMirrorHillTileF = new Tile(*t);



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

		if ((TileLayer)(t->GetTileLayer() & TileLayer::Hill) == TileLayer::Hill) {
			xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);
			xMirrorHillTileE->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);
			xMirrorHillTileW->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);
			xMirrorHillTileF->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

			yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
			yMirrorHillTileE->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
			yMirrorHillTileW->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
			yMirrorHillTileF->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);

			xMirrorHillTileE->SetCappingDirection(Direction::East);
			xMirrorHillTileW->SetCappingDirection(Direction::West);
			xMirrorHillTileF->SetCappingDirection((Direction)(Direction::East | Direction::West));

			yMirrorHillTileE->SetCappingDirection(Direction::East);
			yMirrorHillTileW->SetCappingDirection(Direction::West);
			yMirrorHillTileF->SetCappingDirection((Direction)(Direction::East | Direction::West));

			t->TilePhysicsInit(physics, playerDimensions, tilePixels);

			xMirrorHillTile->TilePhysicsInit(physics, playerDimensions, tilePixels);
			xMirrorHillTileE->TilePhysicsInit(physics, playerDimensions, tilePixels);
			xMirrorHillTileW->TilePhysicsInit(physics, playerDimensions, tilePixels);
			xMirrorHillTileF->TilePhysicsInit(physics, playerDimensions, tilePixels);

			yMirrorHillTile->TilePhysicsInit(physics, playerDimensions, tilePixels);
			yMirrorHillTileE->TilePhysicsInit(physics, playerDimensions, tilePixels);
			yMirrorHillTileW->TilePhysicsInit(physics, playerDimensions, tilePixels);
			yMirrorHillTileF->TilePhysicsInit(physics, playerDimensions, tilePixels);
		
			if ((t->GetHillDirection() & (unsigned short)Direction::North) == (unsigned short)Direction::North) {
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
			SDL_Rect r(0, 0, 128, 128);
			SDL_Rect sR = t->GetSourceRect();
			SDL_RenderClear(graphicsRef->GetRenderer());
			SDL_RenderCopy(graphicsRef->GetRenderer(), spriteSheet->GetTexture().get(), &sR, &r);
			SDL_RenderPresent(graphicsRef->GetRenderer());
		}
		else {
			delete xMirrorHillTileE;
			delete xMirrorHillTileW;
			delete xMirrorHillTileF;
			delete xMirrorHillTile;

			delete yMirrorHillTileE;
			delete yMirrorHillTileW;
			delete yMirrorHillTileF;
			delete yMirrorHillTile;


			tE->SetCappingDirection(Direction::East);
			tW->SetCappingDirection(Direction::West);
			tF->SetCappingDirection((Direction)(Direction::East | Direction::West));

			tE->TilePhysicsInit(physics, playerDimensions, tilePixels);
			tW->TilePhysicsInit(physics, playerDimensions, tilePixels);
			tF->TilePhysicsInit(physics, playerDimensions, tilePixels);
			t->TilePhysicsInit(physics, playerDimensions, tilePixels);

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

void TileManager::FillHills(std::vector<std::pair<int,int>>& caveWalk)
{
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
	int ni, si;
	int randIndex = 0;
	std::vector<std::vector<SDL_Color>> pixels;
	SDL_Rect sR;
	SDL_Rect dR;

	si = ni = 0;

	northSize = northEastHillTiles->at((Direction)(Direction::West | Direction::East)).size();
	southSize = southEastHillTiles->at((Direction)(Direction::West | Direction::East)).size();

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 255, 255);

	//for (int y = 2; y < tileMap.size() - 2; y++) {
	//	for (int x = 2; x < tileMap[y].size() - 2; x++) {
	for(std::pair<int,int> walk : caveWalk){
		int x = walk.first;
		int y = walk.second;
		cTile = tileMap[y][x];

		ni = rand() % northSize;
		si = rand() % southSize;

		pixels.clear();

		if (y - 1 > 1 && x + 1 < tileMap.at(y - 1).size() - 1 && tileMap.at(y - 1).at(x + 1) != nullptr)
			ne = tileMap.at(y - 1).at(x + 1);
		if (y + 1 < tileMap.size() - 1 && x + 1 < tileMap.at(y + 1).size() - 1 && tileMap.at(y + 1).at(x + 1) != nullptr)
			se = tileMap.at(y + 1).at(x + 1);
		if (y + 1 < tileMap.size() - 1 && x - 1 > 1 && tileMap.at(y + 1).at(x - 1) != nullptr)
			sw = tileMap.at(y + 1).at(x - 1);
		if (y - 1 > 1 && x - 1 > 1 && tileMap.at(y - 1).at(x - 1) != nullptr)
			nw = tileMap.at(y - 1).at(x - 1);
		if (y - 1 > 1 && tileMap.at(y - 1).at(x) != nullptr)
			north = tileMap.at(y - 1).at(x);
		if (y + 1 < tileMap.size() - 1 && tileMap.at(y + 1).at(x) != nullptr)
			south = tileMap.at(y + 1).at(x);
		if (x - 1 > 1 && tileMap.at(y).at(x - 1) != nullptr)
			west = tileMap.at(y).at(x - 1);
		if (x + 1 < tileMap.at(y).size() - 1 && tileMap.at(y).at(x + 1) != nullptr)
			east = tileMap.at(y).at(x + 1);

		if (se && sw) {
			if (!(east != nullptr) != !(west != nullptr)) {
				if (!east) {
					Tile* randomHill = southEastHillTiles->at((Direction)(Direction::West | Direction::East))
						.at(si);
					sR = randomHill->GetSourceRect();
					pixels = CopyRectOfTilePixelsFromTexture(&sR);

					randomHill->TilePhysicsInit(physics, playerDimensions, pixels);
					tileMap[y][x] = new Tile(*randomHill);
				}
				else {
					Tile* randomHill = northEastHillTiles->at((Direction)(Direction::West | Direction::East))
						.at(ni);
					sR = randomHill->GetSourceRect();
					pixels = CopyRectOfTilePixelsFromTexture(&sR);

					randomHill->TilePhysicsInit(physics, playerDimensions, pixels);
					tileMap[y][x] = new Tile(*randomHill);
				}
			}
		}
		else if (!se && sw) {
			if (!(east != nullptr) != !(west != nullptr)) {
				if (!east) {
					Tile* randomHill = southEastHillTiles->at((Direction)(Direction::West | Direction::East))
						.at(si);
					sR = randomHill->GetSourceRect();
					pixels = CopyRectOfTilePixelsFromTexture(&sR);

					randomHill->TilePhysicsInit(physics, playerDimensions, pixels);
					tileMap[y][x] = new Tile(*randomHill);
				}
				else {
					Tile* randomHill = northEastHillTiles->at((Direction)(Direction::West | Direction::East))
						.at(ni);
					sR = randomHill->GetSourceRect();
					pixels = CopyRectOfTilePixelsFromTexture(&sR);

					randomHill->TilePhysicsInit(physics, playerDimensions, pixels);
					tileMap[y][x] = new Tile(*randomHill);
				}
			}
		}
		else {
			continue;
		}
		


		dR = SDL_Rect(x, y, 1, 1);

		SDL_RenderDrawRect(graphicsRef->GetRenderer(), &dR);
		SDL_RenderPresent(graphicsRef->GetRenderer());
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
		if (InBounds(x + 1, y) && tileMap[y][x + 1] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x + 1, y));
		//S Neighbor
		if (InBounds(x, y + 1) && tileMap[y + 1][x] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x, y + 1));
		//W Neighbor
		if (InBounds(x - 1, y) && tileMap[y][x - 1] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x - 1, y));

		//NE Neighbor
		if (InBounds(x + 1, y - 1) && tileMap[y - 1][x + 1] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x + 1, y - 1));
		//SE Neighbor
		if (InBounds(x + 1, y + 1) && tileMap[y + 1][x + 1] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x + 1, y + 1));
		//SW Neighbor
		if (InBounds(x - 1, y + 1) && tileMap[y + 1][x - 1] && !p.contains(std::pair<int, int>(x, y - 1)))
			p.insert(std::pair<int, int>(x - 1, y + 1));
		//NW Neighbor
		if (InBounds(x - 1, y - 1) && tileMap[y - 1][x - 1] && !p.contains(std::pair<int, int>(x, y - 1)))
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

	graphicsRef->Vector2PixelsToMeters(spawn);

	SDL_RenderClear(graphicsRef->GetRenderer());

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 255, 0, 0, 255);

	for (std::pair<int, int> coord : walk)
	{
		const SDL_Rect r(coord.first, coord.second, 1, 1);

		Tile* t = tileMap[coord.second][coord.first];
		tileMap[coord.second][coord.first] = nullptr;
		delete t;


		SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	walkPerimeter = GetWalkPerimeter(walk);

	walk.clear();

	walk.reserve(walkPerimeter.size());
	for (auto it = walkPerimeter.begin(); it != walkPerimeter.end();)
		walk.push_back(std::move(walkPerimeter.extract(it++).value()));

	FillHills(walk);


	delete caveWalk;

	SDL_RenderPresent(graphicsRef->GetRenderer());

	SDL_RenderClear(graphicsRef->GetRenderer());
}

void TileManager::CreateMapRenderTarget()
{
	Uint32 fmt;
	SDL_Renderer* ren = graphicsRef->GetRenderer();
	SDL_Texture* spriteSheetTexture = spriteSheet->GetTexture().get();
	SDL_QueryTexture(spriteSheet.get()->GetTexture().get(), &fmt, nullptr, nullptr, nullptr);

	tileMapTexture = Sprite::CreateRenderTexture(worldCols * tileWidth, worldRows * tileHeight, graphicsRef, fmt);
	SDL_Texture* tex = tileMapTexture.get();
	

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

	SDL_RenderClear(ren);
	SDL_SetRenderTarget(ren, tex);

	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap[y].size(); x++) {

			SDL_Rect rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

			if (tileMap[y][x] != nullptr) {
				SDL_Rect sR = tileMap[y][x]->GetSourceRect();
				SDL_RenderCopy(ren, spriteSheetTexture, &sR, &rect);
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
	platformTiles = new TileCollection();

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
			if (t)
				delete t;
			row.erase(row.begin());
		}
		tileMap.erase(tileMap.begin());
	}
	tileMap.clear();

	delete groundTiles;
	delete southEastHillTiles;
	delete northEastHillTiles;
	delete platformTiles;

	spriteSheet.reset();

	tileMapTexture.reset();

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
		tileMapTexture.get(),
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

	//Carve out walkable tunnel
	//for (int i = 0; i < MAX_TUNNELS; i += (int)((int)(gf2d_random() * 10) % MAX_TUNNELS)/2)
		//CarvePath();

	return &tileMap;
}

void TileManager::LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap)
{
	for (int y = 0; y < tileMap.size(); y++) {
		b2Vec2 tpg = b2Vec2_zero;
		b2Vec2 tng = b2Vec2_zero;
		b2Vec2 bpg = b2Vec2_zero;
		b2Vec2 bng = b2Vec2_zero;

		for (int x = 0; x < tileMap[y].size(); x++) {
			Tile* tile = tileMap[y][x];
			Tile* ne = nullptr;
			Tile* se = nullptr;
			Tile* sw = nullptr;
			Tile* nw = nullptr;
			Tile* north = nullptr;
			Tile* south = nullptr;
			Tile* east = nullptr;
			Tile* west = nullptr;

			if (!tile)
				continue;

			if (y - 1 >= 1 && x + 1 < tileMap.at(y - 1).size() && tileMap.at(y - 1).at(x + 1) != nullptr && tileMap.at(y - 1).at(x + 1)->GetBodyReference())
				ne = tileMap.at(y - 1).at(x + 1);
			if (y + 1 < tileMap.size() && x + 1 < tileMap.at(y + 1).size() && tileMap.at(y + 1).at(x + 1) != nullptr && tileMap.at(y + 1).at(x + 1)->GetBodyReference())
				se = tileMap.at(y + 1).at(x + 1);
			if (y + 1 < tileMap.size() && x - 1 >= 0 && tileMap.at(y + 1).at(x - 1) != nullptr && tileMap.at(y + 1).at(x - 1)->GetBodyReference())
				sw = tileMap.at(y + 1).at(x - 1);
			if (y - 1 >= 1 && x - 1 >= 0 && tileMap.at(y - 1).at(x - 1) != nullptr && tileMap.at(y - 1).at(x - 1)->GetBodyReference())
				nw = tileMap.at(y - 1).at(x - 1);
			if (y - 1 < tileMap.size() && tileMap.at(y - 1).at(x) != nullptr && tileMap.at(y - 1).at(x)->GetBodyReference())
				north = tileMap.at(y - 1).at(x);
			if (y + 1 < tileMap.size() && tileMap.at(y + 1).at(x) != nullptr && tileMap.at(y + 1).at(x)->GetBodyReference())
				south = tileMap.at(y + 1).at(x);
			if (x - 1 < tileMap.at(y).size() && tileMap.at(y).at(x - 1) != nullptr && tileMap.at(y).at(x - 1)->GetBodyReference())
				west = tileMap.at(y).at(x - 1);
			if (x + 1 < tileMap.at(y).size() && tileMap.at(y).at(x + 1) != nullptr && tileMap.at(y).at(x + 1)->GetBodyReference())
				east = tileMap.at(y).at(x + 1);

			if (west) {
				tpg = west->GetTopChainLastVertex();
				bpg = west->GetBottomChainLastVertex();
			}
			else if (nw) {
				tpg = nw->GetTopChainLastVertex();
				bpg = nw->GetBottomChainLastVertex();
			}
			else if (sw) {
				tpg = sw->GetTopChainLastVertex();
				bpg = sw->GetBottomChainLastVertex();
			}
			else{
				tpg = tile->GetTopChainFirstVertex();
				bpg = tile->GetBottomChainFirstVertex();
			}

			if (east) {
				tng = east->GetTopChainFirstVertex();
				bng = east->GetBottomChainFirstVertex();
			}
			else if (ne) {
				tng = ne->GetTopChainFirstVertex();
				bng = ne->GetBottomChainFirstVertex();
			}
			else if (se) {
				tng = se->GetTopChainFirstVertex();
				bng = se->GetBottomChainFirstVertex();
			}
			else {
				tng = tile->GetTopChainLastVertex();
				bng = tile->GetBottomChainLastVertex();
			}

			tile->CreateTileBody(
					physics,
					tpg,
					tng,
					bpg,
					bng
				);

			if ((tile->GetHillDirection() & Direction::None) == 0)
				tile->SetGridPosition(x, y);
		}
	}

	CreateMapRenderTarget();
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

bool TileCollection::AddDirection(Direction dir)
{
	return false;
}

TileCollection::TileCollection()
{
	root = nullptr;
}

TileCollection::~TileCollection()
{
	while (root) {
	TileNode* current = root;
		while (current) {
			current = current->child;
		}

		while (!current->tiles.empty()) {
			Tile* tile = current->tiles.back();
			current->tiles.pop_back();
			delete tile;
		}

		delete current;
		current = nullptr;
	}

	root = nullptr;
}

bool TileCollection::AddTile(Tile* newTile)
{
	TileNode* cur = root;
	float* slopes = newTile->GetSlopes();

	do {
		if (!cur) 
			break;
		else if (cur->direction == newTile->GetCappingDirection()) {
			if (std::find(cur->tiles.begin(), cur->tiles.end(), newTile) == cur->tiles.end()) {
				if (slopes[0] < cur->tiles.front()->GetSlopes()[0])
					cur->tiles.emplace(cur->tiles.begin(), newTile);
				if (slopes[0] > cur->tiles.back()->GetSlopes()[0])
					cur->tiles.push_back(newTile);

				return true;
			}
		}

		cur = cur->child;
	} while (cur->child);

	cur->child = new TileNode();
	cur->direction = newTile->GetCappingDirection();
	cur->tiles.push_back(newTile);

	return true;
}

std::vector<Tile*>* TileCollection::FindTilesOfDirection(Direction dir)
{
	TileNode* cur = root;

	while (cur) {
		if (cur->direction & dir)
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
