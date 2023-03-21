#include "tile.h"

extern "C" {
#include "simple_json.h"
}

#define EDGE_COUNT 2
#define MAX_CAPS 2

void Tile::CreatePhysicsEdges(Vector2 playerDimensions)
{
	std::vector<std::vector<SDL_Color>> pixels = spriteSheet->GetPixelData();
	SDL_Rect boundingRect = {INT_MAX, INT_MAX, -1, -1};
	SDL_Rect r = sourceRect;
	b2Vec2 vert;
	int col = 0,
		row = 0;

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
	case SDL_FLIP_VERTICAL:
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		break;
	case SDL_FLIP_HORIZONTAL:
		for (auto vert = topChain.begin(), vertEnd = topChain.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}
		for (auto vert = bottomChain.begin(), vertEnd = bottomChain.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}

		std::reverse(topChain.begin(), topChain.end());
		std::reverse(bottomChain.begin(), bottomChain.end());

		break;
	default:
		break;
	}
}

void Tile::DecideCapping(std::vector<std::vector<SDL_Color>>& pixels, SDL_Rect r)
{
	b2Vec2 vert;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> cappings;
	int row, col;

	//Decide to cap cardinal direction(s)
	if (capDirection != Direction::None) {
		if (capDirection & Direction::East) {
			eastCap.push_back(topChain.back());

			for (col = topChain.back().x + (pixelDimensions.x/ 2.f); col < bottomChain.back().x;) {
				for (row = topChain.back().x; pixels[row][col].a != 0; row++)
					continue;

				col -= (pixelDimensions.y/ 2.f);
				vert = b2Vec2(col * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				eastCap.push_back(vert);
			}


			eastCap.push_back(bottomChain.back());
		}

		if (capDirection & Direction::West) {
			westCap.push_back(topChain.front());

			for (col = topChain.front().x + (pixelDimensions.x/ 2.f); col < bottomChain.front().x;) {
				for (row = 0; pixels[row][col].a != 0; row--)
					continue;

				col += (pixelDimensions.y/ 2.f);
				vert = b2Vec2(col * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				westCap.push_back(vert);
			}


			westCap.push_back(bottomChain.front());
		}
	}
}

void Tile::TilePhysicsInit(b2World* world, Vector2 playerDim)
{
	std::vector<std::vector<SDL_Color>> pixels = spriteSheet->GetPixelData();
	Vector2 tmp;

	tmp = pixelPosition;

	graphicsRef->Vector2PixelsToMeters(tmp);

	worldPosition = {
		(float)tmp.x,
		(float)tmp.y
	};

	CreatePhysicsEdges(playerDim);

	//Logic for ghost vertices ( previous and next ) goes here!!!

	DecideCapping(pixels, spriteSheet->GetSourceRect());
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

	c2.CreateChain(static_cast<b2Vec2*>(bottomChain.data()), bottomChain.size(), bpg, bng);

	fd.shape = &c2;
	fd.friction = 0.7f;
	physicsBody->CreateFixture(&fd);

	if (eastCap.size() > 0) {
		c3.CreateChain(static_cast<b2Vec2*>(eastCap.data()), eastCap.size(), *(topChain.begin()), *(bottomChain.begin()));
		fd.shape = &c3;
		fd.friction = 0.7f;
		physicsBody->CreateFixture(&fd);
	}

	if (westCap.size() > 0) {
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

	spriteSheet.~shared_ptr();

	graphicsRef.~shared_ptr();

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	zRot = 0.f;

	debugColor = SDL_Color(0, 255, 0, 255);
}

Tile::Tile(int id, std::shared_ptr<Sprite> s, Vector2 gridPosition, Vector2 dim, Direction dir, std::shared_ptr<Graphics> g, float zRotation, SDL_Rect srcRect)
{
	this->id = id;

	sourceRect = SDL_Rect(srcRect);

	graphicsRef = g;
	spriteSheet = s;

	pixelDimensions = dim;

	worldDimensions.Set(dim.x * MET_IN_PIX, dim.y * MET_IN_PIX);

	pixelPosition = { gridPosition.x * pixelDimensions.x, gridPosition.y * pixelDimensions.y };
	direction = dir;
	zRot = zRotation;

	debugColor = SDL_Color(0, 255, 0, 255);

	topChain = std::vector<b2Vec2>();
	bottomChain = std::vector<b2Vec2>();
	eastCap = std::vector<b2Vec2>();
	westCap = std::vector<b2Vec2>();
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

	this->graphicsRef = oldTile.graphicsRef;

	this->spriteSheet = oldTile.spriteSheet;

	for (auto connect : oldTile.possibleConnections) {
		this->possibleConnections.push_back(TileConnection(connect));
	}

	this->capDirection = oldTile.capDirection;

	this->zRot = oldTile.zRot;

	this->debugColor = oldTile.debugColor;

	this->sourceRect = oldTile.sourceRect;

	this->topChain = oldTile.topChain;
	this->bottomChain = oldTile.bottomChain;
	this->eastCap = oldTile.eastCap;
	this->westCap = oldTile.westCap;
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
		sourceRect.w / 2.0,
		sourceRect.h / 2.0
	};

	dstRect = { 
		(int)(pixelPosition.x - cameraOffset.x),
		(int)(pixelPosition.y - cameraOffset.y),
		(int)pixelDimensions.x, 
		(int)pixelDimensions.y 
	};

	spriteSheet->Draw(Vector2(dstRect.x, dstRect.y), sourceRect, &scale, &scaleCenter, &color);
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

	groundTiles = new std::vector<Tile*>();
	hillTiles = new std::vector<Tile*>();
	platformTiles = new std::vector<Tile*>();
	wallTiles = new std::vector<Tile*>();

	Tile* t = nullptr;
	TileSpriteSheet* tss = new TileSpriteSheet();
	std::shared_ptr<Sprite> spriteSheet;

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

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < tss->tileCount; i++) {		
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

		if(!spriteSheet.get()->CheckIfViableTexture(sR))
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

		t = new Tile(tiles->size()+1,
			spriteSheet,
			{ INT32_MIN,INT32_MIN },
			Vector2(tss->tileWidth, tss->tileHeight),
			direction,
			graphicsRef,
			zRot,
			sR);


		for (int l = 0; l < layerCount; l++) {
			const char* layerName = sj_get_string_value(sj_array_get_nth(sj_object_get_value(sj_array_get_nth(genDescList, i), "layers"), l));
			if (strcmp(layerName, "ground") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Ground));
				groundTiles->push_back(t);
			}
			if (strcmp(layerName, "wall") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Wall));
				wallTiles->push_back(t);
			}
			if (strcmp(layerName, "hill") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Hill));
				hillTiles->push_back(t);

			}
			if (strcmp(layerName, "platform") == 0) {
				t->SetTileLayer((TileLayer)(t->GetTileLayer() | TileLayer::Platform));
				platformTiles->push_back(t);
			}
		}

		for (int j = 0; j < possibleConnects; j++) {
			TileLayer layers = TileLayer::Empty;
			Direction hillOrient;
			SJson* pConnect = sj_array_get_nth(tileIndex, j);
			int allowedLayerCount = sj_array_get_count(sj_object_get_value(pConnect, "allowedLayers"));
			Tile* xMirrorHillTile = nullptr;
			Tile* yMirrorHillTile = nullptr;
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
				xMirrorHillTile = new Tile(*t);
				yMirrorHillTile = new Tile(*t);

				xMirrorHillTile->SetSpriteDirection((Direction)(Direction::North | Direction::West));
				xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

				yMirrorHillTile->SetSpriteDirection((Direction)(Direction::South | Direction::East));
				yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);

				xMirrorHillTile->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::North | Direction::West));

				yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::South | Direction::East));

				t->AddPossibleConnection(
					Vector2(x, y),
					layers,
					(Direction)(Direction::North | Direction::East));

				hillTiles->push_back(xMirrorHillTile);
				hillTiles->push_back(yMirrorHillTile);
				hillTiles->push_back(t);
			}
			else if (strcmp(o, "Southeast") == 0) {
				xMirrorHillTile = new Tile(*t);
				yMirrorHillTile = new Tile(*t);

				xMirrorHillTile->SetSpriteDirection((Direction)(Direction::South | Direction::West));
				xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

				yMirrorHillTile->SetSpriteDirection((Direction)(Direction::North | Direction::East));
				yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);

				xMirrorHillTile->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::South | Direction::East));

				yMirrorHillTile->AddPossibleConnection(
					Vector2(x, y * -1),
					layers,
					(Direction)(Direction::South | Direction::West));

				t->AddPossibleConnection(
					Vector2(x * -1, y),
					layers,
					(Direction)(Direction::South | Direction::East));

				hillTiles->push_back(xMirrorHillTile);
				hillTiles->push_back(yMirrorHillTile);
				hillTiles->push_back(t);
			}
			else {
				t->AddPossibleConnection(
					Vector2(x, y),
					layers,
					Direction::None);
			}
		}
	}


	sj_free(jsonResource);
	sj_free(genDesc);
}

TileManager::TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDim)
{
	std::vector<Tile*> tileRow;
	Tile* newTile;
	int x = 0, y = 0;

	playerDimensions = playerDim;

	graphicsRef = graphics;

	physics = world;

	worldSize = graphics->GetScreenDimensions();
	worldSize.x /= 4;

	//tileTypes.insert(TileParseTypesFromJSON("data/tilemap/factory/pipeTiles.json"));
	TileParseTypesFromJSON("data/tilemap/Grassland/grassTiles.json");

	//for (int i = 0; i < tileTypes.at("Grassland").size(); i++) {
	/*for (int i = tilerow->size() - 1; i >= 1; i--) {
		newTile = new Tile(*tileRow[i]);

		if (i == 1) {
			newTile->SetCappingDirection(Direction::East);
		}

		newTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
		newTile->FlipTileSprite();
		newTile->TilePhysicsInit(physics, vector2(x++, y));
		newTile->TileCreateBody(physics);
		tilerow->push_back(newTile);
	}
	
	srand(time(nullptr));

	newTile = new Tile(*tileTypes["Grassland"][1]);
	newTile->SetCappingDirection((Direction)(Direction::West | Direction::East));
	newTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);
	newTile->FlipTileSprite();
	newTile->TilePhysicsInit(physics, vector2(x++, y));
	newTile->TileCreateBody(physics);
	tilerow->push_back(newTile);*/

	//tileMap.push_back(tileRow);
}

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
		Tile* t = groundTiles->back();
		groundTiles->pop_back();
	}
	delete groundTiles;

	while (!hillTiles->empty()) {
		Tile* t = hillTiles->back();
		hillTiles->pop_back();
	}
	delete hillTiles;

	while (!platformTiles->empty()) {
		Tile* t = platformTiles->back();
		platformTiles->pop_back();
	}
	delete platformTiles;

	while (!wallTiles->empty()) {
		Tile* t = wallTiles->back();
		wallTiles->pop_back();
	}
	delete wallTiles;

	spriteSheet.~shared_ptr();

	graphicsRef.reset();
}

void TileManager::UpdateMap()
{
}

void TileManager::DrawMap(Vector2 cameraOffset)
{
	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap.at(y).size(); x++) {
			if (tileMap.at(y).at(x) != nullptr)
				tileMap.at(y).at(x)->Draw(cameraOffset);
		}
	}

}

std::vector<std::vector<Tile*>>* TileManager::GenerateTileMap(PerlinNoise* perlin, b2World* physicsWorld, Vector2 pDim)
{
	std::vector<float> perlin1D = perlin->PerlinNoise1D();

	Vector2 dim = groundTiles->at(0)->GetPixelDimensions();
	Tile* tmp = groundTiles->at(rand() % groundTiles->size());
	int rowCount = worldSize.y / tmp->GetPixelDimensions().y;
	//ScalePerlinNoise1D(perlin1D, graphicsRef->GetScreenDimensions());

	for (int i = 0; i < rowCount; i++) {
		std::vector<Tile*> tileRow;
		tileRow.assign(perlin1D.size(), nullptr);
		tileMap.push_back(tileRow);
	}

	for (int x = 0; x < perlin1D.size(); x++) {
		int yIndex = (int)(perlin1D[x] * graphicsRef->GetScreenDimensions().y) % rowCount;
			
		tileMap.at(yIndex).at(x) = new Tile(*groundTiles->at(rand() % groundTiles->size()));
		tileMap.at(yIndex).at(x)->SetGridPosition(x, yIndex);
		tileMap.at(yIndex).at(x)->TilePhysicsInit(physicsWorld, pDim);
	}

	delete perlin;

	return &tileMap;
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

