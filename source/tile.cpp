#include "tile.h"

extern "C" {
#include "simple_json.h"
}

#define EDGE_COUNT 2
#define MAX_CAPS 2

void Tile::CreatePhysicsEdges(std::vector<std::vector<SDL_Color>>& pixels, Vector2 playerDimensions)
{
	SDL_Rect boundingRect = {INT_MAX, INT_MAX, -1, -1};
	SDL_Rect r = sourceRect;
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (topChain.size() > 0 && bottomChain.size() > 0)
		return;

	if (direction == Direction::North) {
		//scan north -> south
		std::cout << "Scanning north to south" << std::endl;
		for (col = r.x; col < r.x + r.w; col++) {
			for (row = r.y; row < r.y+ r.h; row++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2((col - r.x) * MET_IN_PIX, (row - r.y) * MET_IN_PIX );
					topChain.push_back(vert);
					col += (playerDimensions.x / 2);
					break;
				}
			}
		}

		for (col = r.x+r.w-1, row = r.y; row < r.y+r.h; row++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2((col - r.x) * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning south to north" << std::endl;

		//scan from south -> north
		for (col = r.x; col < r.x + r.w;col++) {
			for (row = r.y + r.h-1; row >= r.y; row--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2((col - r.x) * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
					bottomChain.push_back(vert);
					col += (playerDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.x + r.w - 1, row = r.y + r.h - 1; row >= r.y; row--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2((col - r.x) * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				bottomChain.push_back(vert);
				break;
			}
		}
	}
	else if (direction == Direction::East) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = r.y;
		for (; row < r.y + r.h; row++) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row - r.y));
					topChain.push_back(vert);
					row += (playerDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row - r.y));
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row - r.y));
					bottomChain.push_back(vert);
					row += (playerDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row - r.y));
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

		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row - r.y));
					topChain.push_back(vert);
					row += (playerDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row - r.y));
				topChain.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from west -> east
		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col, (row - r.y));
					bottomChain.push_back(vert);
					row += (playerDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col, (row - r.y));
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
}

Tile::Tile(int id, const std::shared_ptr<Sprite>& s, Vector2 gridPosition, Vector2 dim, Direction dir, const std::shared_ptr<Graphics>& graphics, float zRotation, SDL_Rect srcRect)
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

	this->topChain = oldTile.topChain;
	this->bottomChain = oldTile.bottomChain;
	this->eastCap = oldTile.eastCap;
	this->westCap = oldTile.westCap;
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

void TileManager::TileParseTypesFromJSON(std::string json)
{
	const char* dirStr;
	std::vector<Tile*>* tiles = new std::vector<Tile*>();
	float zRot;
	Direction direction;
	SDL_Rect srcRect;
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

	spriteSheetPixels = spriteSheet->GetPixelData();

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < sj_array_get_count(genDescList); i++) {
		int layerCount = 0;

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
			sR);
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
					if (layers == TileLayer::Empty)
						layers = TileLayer::Ground;
					else
						layers = (TileLayer)(layers | TileLayer::Ground);
				}
				if (strcmp(layerName, "wall") == 0) {
					if (layers == TileLayer::Empty)
						layers = TileLayer::Wall;
					else
						layers = (TileLayer)(layers | TileLayer::Wall);
				}
				if (strcmp(layerName, "hill") == 0) {
					layers = TileLayer::Hill;
				}
				if (strcmp(layerName, "platform") == 0) {
					if (layers == TileLayer::Empty)
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

				yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::South | Direction::East));

				yMirrorHillTile->SetHillOrientation((Direction)(Direction::South | Direction::East));

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

				yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::North | Direction::East));

				yMirrorHillTile->SetHillOrientation((Direction)(Direction::North | Direction::East));

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

			t->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);

			xMirrorHillTile->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			xMirrorHillTileE->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			xMirrorHillTileW->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			xMirrorHillTileF->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);

			yMirrorHillTile->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			yMirrorHillTileE->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			yMirrorHillTileW->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			yMirrorHillTileF->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
		
			hillTiles->at(Direction::East).push_back(xMirrorHillTileE);
			hillTiles->at(Direction::West).push_back(xMirrorHillTileW);
			hillTiles->at((Direction)(Direction::East | Direction::West)).push_back(xMirrorHillTileF);
			hillTiles->at(Direction::None).push_back(xMirrorHillTile);
					 
			hillTiles->at(Direction::East).push_back(yMirrorHillTileE);
			hillTiles->at(Direction::West).push_back(yMirrorHillTileW);
			hillTiles->at((Direction)(Direction::East | Direction::West)).push_back(yMirrorHillTileF);
			hillTiles->at(Direction::None).push_back(yMirrorHillTile);
					 
			hillTiles->at(Direction::East).push_back(tE);
			hillTiles->at(Direction::West).push_back(tW);
			hillTiles->at((Direction)(Direction::East | Direction::West)).push_back(tF);
			hillTiles->at(Direction::None).push_back(t);
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

			tE->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			tW->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			tF->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);
			t->TilePhysicsInit(physics, playerDimensions, spriteSheetPixels);

			groundTiles->at(Direction::East).push_back(tE);
			groundTiles->at(Direction::West).push_back(tW);
			groundTiles->at((Direction)(Direction::East | Direction::West)).push_back(tF);
			groundTiles->at(Direction::None).push_back(t);
		}
	}

	sj_free(jsonResource);
	sj_free(genDesc);
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

	groundTiles = new std::unordered_map<Direction, std::vector<Tile*>>();
	groundTiles->insert_or_assign(Direction::None, std::vector<Tile*>());
	groundTiles->insert_or_assign(Direction::East, std::vector<Tile*>());
	groundTiles->insert_or_assign(Direction::West, std::vector<Tile*>());
	groundTiles->insert_or_assign((Direction)(Direction::East|Direction::West), std::vector<Tile*>());

	hillTiles = new std::unordered_map<Direction, std::vector<Tile*>>();
	hillTiles->insert_or_assign(Direction::None, std::vector<Tile*>());
	hillTiles->insert_or_assign(Direction::East, std::vector<Tile*>());
	hillTiles->insert_or_assign(Direction::West, std::vector<Tile*>());
	hillTiles->insert_or_assign((Direction)(Direction::East | Direction::West), std::vector<Tile*>());

	platformTiles = new std::unordered_map<Direction, std::vector<Tile*>>();
	platformTiles->insert_or_assign(Direction::None, std::vector<Tile*>());
	platformTiles->insert_or_assign(Direction::East, std::vector<Tile*>());
	platformTiles->insert_or_assign(Direction::West, std::vector<Tile*>());
	platformTiles->insert_or_assign((Direction)(Direction::East | Direction::West), std::vector<Tile*>());

	wallTiles = new std::unordered_map<Direction, std::vector<Tile*>>();
	wallTiles->insert_or_assign(Direction::None, std::vector<Tile*>());
	wallTiles->insert_or_assign(Direction::East, std::vector<Tile*>());
	wallTiles->insert_or_assign(Direction::West, std::vector<Tile*>());
	wallTiles->insert_or_assign((Direction)(Direction::East | Direction::West), std::vector<Tile*>());

	TileParseTypesFromJSON("data/tilemap/Grassland/grassTiles.json");

	worldRows = worldSize.y / tileHeight;
	worldCols = worldSize.x;
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
	
	while (!groundTiles->empty()) {
		std::pair<Direction, std::vector<Tile*>> p = *groundTiles->begin();
		std::vector<Tile*> tileList = p.second;
		
		while (!tileList.empty()) {
			Tile* t = tileList.back();
			if (t)
				delete t;
			tileList.pop_back();
		}

		groundTiles->erase(groundTiles->begin());
	}
	delete groundTiles;

	while (!hillTiles->empty()) {
		std::pair<Direction, std::vector<Tile*>> p = *hillTiles->begin();
		std::vector<Tile*> tileList = p.second;

		while (!tileList.empty()) {
			Tile* t = tileList.back();
			if(t)
				delete t;
			tileList.pop_back();
		}

		hillTiles->erase(hillTiles->begin());
	}
	delete hillTiles;

	while (!platformTiles->empty()) {
		std::pair<Direction, std::vector<Tile*>> p = *platformTiles->begin();
		std::vector<Tile*> tileList = p.second;

		while (!tileList.empty()) {
			Tile* t = tileList.back();
			if (t)
				delete t;
			tileList.pop_back();
		}

		platformTiles->erase(platformTiles->begin());
	}
	delete platformTiles;

	while (!wallTiles->empty()) {
		std::pair<Direction, std::vector<Tile*>> p = *wallTiles->begin();
		std::vector<Tile*> tileList = p.second;

		while (!tileList.empty()) {
			Tile* t = tileList.back();
			if (t)
				delete t;
			tileList.pop_back();
		}

		wallTiles->erase(wallTiles->begin());
	}
	delete wallTiles;

	spriteSheet.reset();

	graphicsRef.reset();
}

void TileManager::UpdateMap()
{
}

void TileManager::DrawMap(Vector2 cameraOffset, SDL_Rect& cameraBounds)
{
	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap.at(y).size(); x++) {
			if (tileMap.at(y).at(x) != nullptr && IsInCameraBounds(tileMap.at(y).at(x), cameraBounds))
				tileMap.at(y).at(x)->Draw(cameraOffset);
		}
	}

}

bool TileManager::IsInCameraBounds(Tile* t, SDL_Rect cameraBounds)
{
	Vector2 tPos = t->GetPixelPosition();

	return tPos.x + tileWidth >= cameraBounds.x
		&& tPos.x < cameraBounds.x + cameraBounds.w
		&& tPos.y + tileHeight >= cameraBounds.y
		&& tPos.y < cameraBounds.y + cameraBounds.h;
}

std::vector<std::vector<Tile*>>* TileManager::GenerateTileMap(PerlinNoise* perlin, b2World* physicsWorld, Vector2 pDim)
{
	int rowsToFill = 2;

	SDL_Rect r(0, 0, worldCols, rowsToFill);
	SDL_Rect test(0, 0, 128, 128);
	
	GenerateTileMapRectArea(r);

	r.y = (int)(worldSize.y / tileHeight) - rowsToFill;

	GenerateTileMapRectArea(r);

	/*SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);

	for (int x = 0, y = 0; y < tileMap.size(); x++) {
		if (x >= tileMap[y].size()) {
			x = 0;
			if (y + 1 >= tileMap.size())
				break;
			else
				y++;
		}
		
		if (tileMap[y][x] != nullptr) {
			test = SDL_Rect(x * 128, y * 128, 128, 128);
			SDL_RenderDrawRect(graphicsRef->GetRenderer(), &test);
		}
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());*/

	return &tileMap;
}

void TileManager::GenerateTileMapRectArea(SDL_Rect& r)
{
	PerlinNoise* perlin = new PerlinNoise(Vector2(r.w,r.h));
	std::vector<float> perlin1D = perlin->PerlinNoise1D();
	Vector2 dim = groundTiles->at(Direction::West).at(0)->GetPixelDimensions();
	Tile* tmp = groundTiles->at(Direction::West).at(0);

	tileMap.resize((int)(worldSize.y / tileHeight));
	for (int i = 0; i < tileMap.size(); i++)
		tileMap.at(i).resize(r.w);

	for (int x = 0; x < perlin1D.size(); x++) {
		int yIndex = (int)(perlin1D[x] * graphicsRef->GetScreenDimensions().y);

		tileMap.at((int)(yIndex + r.y) % worldRows).at(x + r.x) = new Tile(*groundTiles->at((Direction)(Direction::East| Direction::West)).at((int)(rand() % groundTiles->size())));
		tileMap.at((int)(yIndex + r.y) % worldRows).at(x + r.x)->SetGridPosition(x, yIndex);
	}

	delete perlin;
}

void TileManager::LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap)
{
	int x, y;

	y = 0;
	
	for (std::vector<Tile*> row : *tilemap) {
		b2Vec2 tpg = b2Vec2_zero;
		b2Vec2 tng = b2Vec2_zero;
		b2Vec2 bpg = b2Vec2_zero;
		b2Vec2 bng = b2Vec2_zero;

		x = 0;

		for (Tile* tile : row) {
			if (!tile) {
				x++;

				continue;
			}
			Tile* prev = nullptr;
			Tile* next = nullptr;

			//Check if there are adjacent tiles EXCEPT NORTH AND SOUTH of tile
			if (x - 1 >= 0 && y - 1 >= 0 && tileMap.at(y - 1).at(x - 1) != nullptr) {
				prev = tileMap.at(y - 1).at(x - 1);
			}
			else if (x - 1 >= 0 && tileMap.at(y).at(x - 1) != nullptr) {
				prev = tileMap.at(y).at(x - 1);
			}
			else if (x - 1 >= 0 && y + 1 < tileMap.size() && tileMap.at(y + 1).at(x - 1) != nullptr) {
				prev = tileMap.at(y + 1).at(x - 1);
			}

			if (!prev) {
				tpg = tile->GetTopChainLastVertex();
				bpg = tile->GetBottomChainLastVertex();
			}
			else{
				tpg = prev->GetTopChainLastVertex();
				bpg = prev->GetBottomChainLastVertex();
			}


			if (x + 1 < tileMap.at(y).size() && y - 1 >= 0 && tileMap.at(y - 1).at(x + 1) != nullptr) {
				next = tileMap.at(y - 1).at(x + 1);
			}
			else if (x + 1 < tileMap.at(y).size() && tileMap.at(y).at(x + 1) != nullptr) {
				next = tileMap.at(y).at(x + 1);
			}
			else if (x + 1 < tileMap.at(y).size() && y + 1 < tileMap.size() && tileMap.at(y + 1).at(x + 1) != nullptr) {
				next = tileMap.at(y + 1).at(x + 1);
			}

			if(!next) {
				tpg = tile->GetTopChainFirstVertex();
				bpg = tile->GetBottomChainFirstVertex();
			}
			else {
				tpg = next->GetTopChainFirstVertex();
				bpg = next->GetBottomChainFirstVertex();
			}

			tile->CreateTileBody(
				physics,
				tpg, 
				tng,
				bpg,
				bng
			);
			x++;
		}
		y++;
	}
}

