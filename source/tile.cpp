#include "tile.h"
#include "RSJparser.tcc"

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

void Tile::TilePhysicsInit(b2World* world, Vector2 p, Vector2 playerDim)
{
	std::vector<std::vector<SDL_Color>> pixels = spriteSheet->GetPixelData();
	Vector2 tmp;

	pixelPosition = {
		p.x * pixelDimensions.x,
		p.y * pixelDimensions.y
	};

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

void Tile::CreateTileBody(b2World* world, b2Vec2 tpg, b2Vec2 tng, b2Vec2 bpg, b2Vec2 bng)
{
	b2BodyDef bd;
	b2FixtureDef fd;
	Vector2 tV;
	b2ChainShape c1, c2, c3, c4;
	b2Vec2 center;

	bd.type = b2_staticBody;

	bd.position.Set(worldPosition.x, worldPosition.y);

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

	spriteSheet = nullptr;

	graphicsRef = nullptr;

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	zRot = 0.f;

	debugColor = SDL_Color(0, 255, 0, 255);
}

Tile::Tile(int id, std::shared_ptr<Sprite> s, Vector2 gridPosition, Vector2 dim, Direction dir, std::shared_ptr<Graphics> g, float zRotation, SDL_Rect srcRect)
{
	this->id = id;

	sourceRect = srcRect;

	graphicsRef = g;
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
	id = oldTile.id;
	direction = oldTile.direction;
	pixelDimensions = oldTile.pixelDimensions;
	worldDimensions = oldTile.worldDimensions;

	pixelPosition = { 0,0 };

	worldPosition = { 0,0 };

	physicsBody = nullptr;
	
	spriteSheet = oldTile.spriteSheet;

	graphicsRef = oldTile.graphicsRef;

	capDirection = oldTile.capDirection;

	zRot = oldTile.zRot;

	debugColor = oldTile.debugColor;

	sourceRect = oldTile.sourceRect;
}

Tile::~Tile()
{
	spriteSheet = nullptr;
	graphicsRef = nullptr;
	physicsBody = nullptr;

	while (!possibleConnections.empty()) {
		TileConnection* tc = *(possibleConnections.end()-1);
		possibleConnections.pop_back();
		delete tc;
	}

}

void Tile::AddPossibleConnection(Vector2 v, TileLayer layer, Direction hillDir)
{
	possibleConnections.push_back(new TileConnection{
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
	std::vector<Tile*> tiles;
	float zRot;
	Direction direction;
	SDL_Rect srcRect;
	std::ifstream fileInputstream(json);
	RSJresource jsonResource(fileInputstream);
	std::ifstream fileInputstreamDescription(jsonResource["generationDescription"].as_str());
	RSJresource genDesc(fileInputstreamDescription);
	RSJarray genDescList = genDesc.as_array();
	Tile* t;
	TileSpriteSheet* tss = new TileSpriteSheet();
	std::shared_ptr<Sprite> spriteSheet;

	tss->tileCount = jsonResource["tilecount"].as<int>();
	tss->columns = jsonResource["columns"].as<int>();
	tss->tileWidth = jsonResource["tilewidth"].as<int>();
	tss->tileHeight = jsonResource["tileheight"].as<int>();
	tss->imageWidth = jsonResource["imagewidth"].as<int>();
	tss->imageHeight = jsonResource["imageheight"].as<int>();
	tss->spacing = jsonResource["spacing"].as<int>();
	tss->margin = jsonResource["margin"].as<int>();
	if (jsonResource["direction"].as<std::string>() == std::string("north"))
		direction = Direction::North;
	else if (jsonResource["direction"].as<std::string>() == std::string("east"))
		direction = Direction::East;
	else if (jsonResource["direction"].as<std::string>() == std::string("south"))
		direction = Direction::South;
	else if (jsonResource["direction"].as<std::string>() == std::string("west"))
		direction = Direction::West;
	else
		direction = Direction::None;

	tss->name = jsonResource["name"].as<std::string>();
	tss->filepath = jsonResource["image"].as<std::string>();
	spriteSheet = std::make_shared<Sprite>(tss->filepath.c_str(), tss->imageWidth, tss->imageHeight, graphicsRef);

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (Uint16 i = 0; i < tss->columns * tss->columns; i++) {
		SDL_Rect sR = {
			tss->tileWidth * (i % tss->columns) + tss->spacing * (i % tss->columns) + tss->margin,
			tss->tileHeight * (i / tss->columns) + tss->spacing * (i / tss->columns) + tss->margin,
			tss->tileWidth,
			tss->tileHeight
		};

		if(!spriteSheet->CheckIfViableTexture(sR))
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

		t = new Tile(tiles.size()+1,
			spriteSheet,
			{ INT32_MIN,INT32_MIN },
			Vector2(tss->tileWidth, tss->tileHeight),
			direction,
			graphicsRef,
			zRot,
			sR);

		for (RSJresource coordInfo : genDesc[i]["possibleConnects"].as_vector<RSJresource>()) {
			Tile* xMirrorHillTile = nullptr;
			Tile* yMirrorHillTile = nullptr;
			TileLayer layers = TileLayer::Empty;
			Direction hillOrient;
			std::string o = genDesc[i]["hillOrientation"].as_str();

			for (RSJresource layerName : coordInfo["layers"].as<RSJarray>()) {
				if (layerName.as_str().compare("ground") == 0 ) 
					layers = (TileLayer)(layers | TileLayer::Ground);
				
				if (layerName.as_str().compare("wall") == 0) 
					layers = (TileLayer)(layers | TileLayer::Wall);
									
				if (layerName.as_str().compare("hill") == 0) 
					layers = (TileLayer)(layers | TileLayer::Hill);
				
				if (layerName.as_str().compare("platform") == 0) 
					layers = (TileLayer)(layers | TileLayer::Platform);
				
			}

			if (o.compare("Northeast") == 0) {
				xMirrorHillTile = new Tile(*t);
				yMirrorHillTile = new Tile(*t);

				xMirrorHillTile->SetSpriteDirection((Direction)(Direction::North | Direction::West));
				xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

				yMirrorHillTile->SetSpriteDirection((Direction)(Direction::South | Direction::East));
				yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);

				xMirrorHillTile->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>() * -1,
						coordInfo["y"].as<int>()
					),
					layers,
					(Direction)(Direction::North | Direction::West));

				yMirrorHillTile->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>(),
						coordInfo["y"].as<int>() * -1
					),
					layers,
					(Direction)(Direction::South | Direction::East));

				t->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>(),
						coordInfo["y"].as<int>()
					),
					layers,
					(Direction)(Direction::North | Direction::East));

				tiles.push_back(xMirrorHillTile);
				tiles.push_back(yMirrorHillTile);
			}
			else if (o.compare("SouthEast") == 0) {
				xMirrorHillTile = new Tile(*t);
				yMirrorHillTile = new Tile(*t);

				xMirrorHillTile->SetSpriteDirection((Direction)(Direction::South | Direction::West));
				xMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);

				yMirrorHillTile->SetSpriteDirection((Direction)(Direction::North | Direction::East));
				yMirrorHillTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);

				xMirrorHillTile->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>() * -1,
						coordInfo["y"].as<int>()
					),
					layers,
					(Direction)(Direction::South | Direction::East));

				yMirrorHillTile->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>(),
						coordInfo["y"].as<int>() * -1
					),
					layers,
					(Direction)(Direction::South | Direction::West));

				t->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>() * -1,
						coordInfo["y"].as<int>()
					),
					layers,
					(Direction)(Direction::South | Direction::East));

				tiles.push_back(xMirrorHillTile);
				tiles.push_back(yMirrorHillTile);
			}
			else {
				t->AddPossibleConnection(
					Vector2(coordInfo["x"].as<int>(),
						coordInfo["y"].as<int>()
					),
					layers,
					Direction::None);
			}


		}
		tiles.push_back(t);
	}

	tileTypes.insert_or_assign(tss->name, tiles);
	typeSheets.insert_or_assign(tss->name, tss->sheet);

	tss->sheet = nullptr;

	delete tss;

	fileInputstreamDescription.close();
	fileInputstream.close();
}

TileManager::TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDim)
{
	std::vector<Tile*> tileRow;
	Tile* newTile;
	int x = 0, y = 0;

	tileTypes.clear();

	tileMap.clear();

	playerDimensions = playerDim;

	graphicsRef = graphics;

	physics = world;

	//tileTypes.insert(TileParseTypesFromJSON("data/tilemap/factory/pipeTiles.json"));
	TileParseTypesFromJSON("data/tilemap/Grassland/grassTiles.json");

	//for (int i = 0; i < tileTypes.at("Grassland").size(); i++) {
	/*for (int i = tileRow.size() - 1; i >= 1; i--) {
		newTile = new Tile(*tileRow[i]);

		if (i == 1) {
			newTile->SetCappingDirection(Direction::East);
		}

		newTile->SetSDL_RendererFlipFlags(SDL_FLIP_VERTICAL);
		newTile->FlipTileSprite();
		newTile->TilePhysicsInit(physics, vector2(x++, y));
		newTile->TileCreateBody(physics);
		tileRow.push_back(newTile);
	}
	
	srand(time(nullptr));

	newTile = new Tile(*tileTypes["Grassland"][1]);
	newTile->SetCappingDirection((Direction)(Direction::West | Direction::East));
	newTile->SetSDL_RendererFlipFlags(SDL_FLIP_HORIZONTAL);
	newTile->FlipTileSprite();
	newTile->TilePhysicsInit(physics, vector2(x++, y));
	newTile->TileCreateBody(physics);
	tileRow.push_back(newTile);*/

	//tileMap.push_back(tileRow);
}

TileManager::~TileManager()
{
	while (!tileMap.empty()) {
		std::vector<Tile*> row = tileMap.front();

		while (!row.empty()) {
			Tile* t = row.front();
			row.erase(row.begin());
			delete t;
		}
		tileMap.erase(tileMap.begin());
	}

	for (auto p : tileTypes) {
		std::vector<Tile*> tList = p.second;
		while (!tList.empty()) {
			Tile* t = *tList.begin();

			tList.erase(tList.begin());

			delete t;
		}
	}

	for (auto pair : typeSheets) {
		Sprite* s = pair.second;
		
		delete s;
	}

	graphicsRef = nullptr;
}

void TileManager::UpdateMap()
{
}

void TileManager::DrawMap(Vector2 cameraOffset)
{
	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap[y].size(); x++) {
			if (tileMap[y][x] != nullptr)
				tileMap[y][x]->Draw(cameraOffset);
		}
	}

}

std::vector<std::vector<Tile*>>* TileManager::CreateTileMap()
{
	Tile* newTile = nullptr;
	Vector2 playerDim = playerDimensions;
	std::vector<Tile*> tileRow;
	
	for (int i = 0; i < tileTypes.at("Grassland").size() / 2; i++) {
		newTile = new Tile(*tileTypes["Grassland"][i]);
		if (i == 0)
			newTile->SetCappingDirection((Direction)(Direction::West | Direction::East));

		newTile->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
		//newTile->FlipTileSprite();
		newTile->TilePhysicsInit(physics, vector2(i, 2), playerDim);
		tileRow.push_back(newTile);
	}

	tileMap.push_back(tileRow);

	return &tileMap;
}

void TileManager::LinkTilemapGhostVertices(std::vector<std::vector<Tile*>>* tilemap)
{
	int x, y;

	y = 0;
	
	for (std::vector<Tile*> row : *tilemap) {
		x = 0;
		for (Tile* tile : row) {
			Tile* prev = (x > 0) ? (*tilemap)[y][x - 1] : nullptr;
			Tile* next = (x + 1 < row.size()) ? (*tilemap)[y][x + 1] : nullptr;

			b2Vec2 tpg = (prev ? prev->GetTopChainLastVertex() + prev->GetWorldPosition(): tile->GetTopChainFirstVertex() + tile->GetWorldPosition());
			b2Vec2 tng = (next ? next->GetTopChainFirstVertex() + next->GetWorldPosition(): tile->GetTopChainLastVertex() + tile->GetWorldPosition());
			b2Vec2 bpg = (prev ? prev->GetBottomChainLastVertex() + prev->GetWorldPosition(): tile->GetBottomChainFirstVertex() + tile->GetWorldPosition());
			b2Vec2 bng = (next ? next->GetTopChainFirstVertex() + next->GetWorldPosition(): tile->GetBottomChainLastVertex() + tile->GetWorldPosition());


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

