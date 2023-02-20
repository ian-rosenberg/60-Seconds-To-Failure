#include "tile.h"
#include "RSJparser.tcc"

#define EDGE_COUNT 2
#define MAX_CAPS 2

std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> Tile::CreatePhysicsEdges()
{
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> chainPair;
	//std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath().c_str());
	std::vector<std::vector<SDL_Color>> pixels = animSprite->GetSprite()->GetPixelData();
	std::vector<b2Vec2> c1;
	std::vector<b2Vec2> c2;
	SDL_Rect boundingRect = {INT_MAX, INT_MAX, -1, -1};
	SDL_Rect r = animSprite->GetSprite()->GetSourceRect();
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
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c1.push_back(vert);
					col += (pixelDimensions.x / 2);
					break;
				}
			}
		}

		for (col = r.x+r.w-1, row = r.y; row < r.y+r.h; row++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c1.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning south to north" << std::endl;

		//scan from south -> north
		for (col = r.x; col < r.x + r.w;col++) {
			for (row = r.y + r.h-1; row >= r.y; row--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c2.push_back(vert);
					col += (pixelDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.x + r.w - 1, row = r.y + r.h - 1; row >= r.y; row--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c2.push_back(vert);
				break;
			}
		}
	}
	else if (direction == Direction::East) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = r.y;
		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c1.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c2.push_back(vert);
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
					col += (pixelDimensions.x / 2);
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
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c1.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from west -> east
		for (row = r.y; row < r.y + r.h - 1; row++) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x/ 2);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_IN_MET, (row - r.y) * PIX_IN_MET);
				c2.push_back(vert);
				break;
			}
		}
	}

	switch (flipFlags) {
	case SDL_FLIP_VERTICAL:
		for (auto vert = c1.begin(), vertEnd = c1.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		for (auto vert = c2.begin(), vertEnd = c2.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->x, vertEnd->x);
		}
		break;
	case SDL_FLIP_HORIZONTAL:
		for (auto vert = c1.begin(), vertEnd = c1.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}
		for (auto vert = c2.begin(), vertEnd = c2.end() - 1; vert != vertEnd; vert++, vertEnd--) {
			std::swap(vert->y, vertEnd->y);
		}

		std::reverse(c1.begin(), c1.end());
		std::reverse(c2.begin(), c2.end());

		break;
	default:
		break;
	}

	chainPair.first = c1;
	chainPair.second = c2;


	return chainPair;
}

std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> Tile::DecideCapping(std::vector<b2Vec2>& c1, std::vector<b2Vec2>& c2, std::vector<std::vector<SDL_Color>>& pixels, SDL_Rect r)
{
	std::vector<b2Vec2> c3;
	std::vector<b2Vec2> c4;
	b2Vec2 vert;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> cappings;
	int row, col;

	//Decide to cap cardinal direction(s)
	if (capDirection != Direction::None) {
		if (capDirection & Direction::East) {
			c3.push_back(c1.back());

			for (col = c1.back().x + (pixelDimensions.x/ 2.f); col < c2.back().x;) {
				for (row = c1.back().x; pixels[row][col].a != 0; row++)
					continue;

				col -= (pixelDimensions.y/ 2.f);
				vert = b2Vec2(col * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				c3.push_back(vert);
			}


			c3.push_back(c2.back());

			cappings.first = c3;
		}

		if (capDirection & Direction::West) {
			c4.push_back(c1.front());

			for (col = c1.front().x + (pixelDimensions.x/ 2.f); col < c2.front().x;) {
				for (row = 0; pixels[row][col].a != 0; row--)
					continue;

				col += (pixelDimensions.y/ 2.f);
				vert = b2Vec2(col * MET_IN_PIX, (row - r.y) * MET_IN_PIX);
				c4.push_back(vert);
			}


			c4.push_back(c2.front());

			cappings.second = c4;
		}
	}

	return cappings;
}

void Tile::TilePhysicsInit(b2World* world, Vector2 p)
{
	float temp;
	b2Vec2 pg;
	b2Vec2 ng;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> chains = {};
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> cappings;
	std::vector<std::vector<SDL_Color>> pixels = animSprite->GetSprite()->GetPixelData();


	pixelPosition = { p.x * pixelDimensions.x , p.y * pixelDimensions.y };

	chains = CreatePhysicsEdges();

	//Logic for ghost vertices ( previous and next ) goes here!!!

	cappings = DecideCapping(chains.first, chains.second, pixels, animSprite->GetSprite()->GetSourceRect());

	if (!chains.first.empty())
		chainsAndCaps.push_back(std::make_pair(chains.first, true));
	if (!chains.second.empty())
		chainsAndCaps.push_back(std::make_pair(chains.second, true));
	if (!cappings.first.empty())
		chainsAndCaps.push_back(std::make_pair(cappings.first, false));
	if (!cappings.second.empty())
		chainsAndCaps.push_back(std::make_pair(cappings.second, false));
}

void Tile::TileCreateBody(b2World* world)
{
	b2BodyDef bd;
	b2FixtureDef fd;
	Vector2 tV;
	//b2Vec2 center;

	bd.type = b2_staticBody;

	tV = pixelPosition;

	bd.position.Set(tV.x * PIX_IN_MET, tV.y * PIX_IN_MET);
	//center.Set(worldDimensions.x / 2, worldDimensions.y / 2);
	
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
	worldPosition = physicsBody->GetPosition();

	for (std::pair<std::vector<b2Vec2>, bool> chain : chainsAndCaps) {
		b2ChainShape chainShape;
		/*for (auto& vertex : chain.first) {
			float x = cos(bd.angle) * (vertex.x - center.x) - sin(bd.angle) * (vertex.y - center.y) + center.x;
			float y = sin(bd.angle) * (vertex.x - center.x) + cos(bd.angle) * (vertex.y - center.y) + center.y;
			vertex.x = x;
			vertex.y = y;
		}*/

		chainShape.CreateChain(static_cast<b2Vec2*>(chain.first.data()), chain.first.size(), chain.first.front(), chain.first.back());

		fd.shape = &chainShape;
		fd.friction = 0.9f;
		physicsBody->CreateFixture(&fd);
	}
}

void Tile::SetCappingDirection(Direction capping)
{
	this->capDirection = capping;
}

void Tile::FlipTileSprite()
{
	Sprite* s = animSprite->GetSprite();
	s->FlipTexture(flipFlags);
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

	animSprite = nullptr;

	graphicsRef = nullptr;

	debugDraw = nullptr;

	flipFlags = SDL_FLIP_NONE;

	capDirection = Direction::None;

	zRot = 0.f;
}

Tile::Tile(int id, Sprite* s, DebugDraw* dd, Vector2 gridPosition, Vector2 dim, Direction dir, std::shared_ptr<Graphics> g, float zRotation)
{
	Vector2 temp;
	int x, y;

	this->id = id;

	graphicsRef = g;
	animSprite = new Animation(std::string("-" + std::to_string((int)gridPosition.x) + "-" + std::to_string((int)gridPosition.y)),
		s,
		pixelDimensions.x,
		pixelDimensions.y,
		s->GetXOffset(),
		s->GetYOffset(),
		{ SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8 });
	pixelDimensions = dim;
	debugDraw = dd;

	x = animSprite->GetSprite()->GetSourceRect().x;
	y = animSprite->GetSprite()->GetSourceRect().y;


	graphicsRef->Vector2PixelsToMeters(temp);
	worldDimensions.Set(x, y);

	pixelPosition = { gridPosition.x * pixelDimensions.x, gridPosition.y * pixelDimensions.y };
	direction = dir;
	zRot = zRotation;
}

Tile::Tile(const Tile &oldTile)
{
	id = oldTile.id;
	direction = oldTile.direction;
	pixelDimensions = oldTile.pixelDimensions;
	worldDimensions = oldTile.worldDimensions;

	pixelPosition = oldTile.pixelPosition;
	worldPosition = oldTile.worldPosition;

	physicsBody = nullptr;

	animSprite = new Animation(*oldTile.animSprite);

	graphicsRef = oldTile.graphicsRef;

	debugDraw = nullptr;

	capDirection = oldTile.capDirection;

	zRot = oldTile.zRot;
}

Tile::~Tile()
{
	if (animSprite)
		delete animSprite;

	if (debugDraw)
		delete debugDraw;

	graphicsRef = nullptr;
	physicsBody = nullptr;

}

void Tile::RotateChain(std::vector<b2Vec2>& chain, float theta)
{
	float c = 0, s = 0, cx = 0, cy = 0;
	float xNew, yNew;
	Vector2 ctr = {pixelPosition.x + pixelDimensions.x/2, pixelPosition.y + pixelDimensions.y / 2 };

	c = cos(-theta);
	s = sin(-theta);

	worldPosition = physicsBody->GetPosition();

	cx = pixelPosition.x;// +worldPosition.x;
	cy = pixelPosition.y;// +worldPosition.y;

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
	Sprite* s = animSprite->GetSprite();
	SDL_Rect rect = s->GetSourceRect();
	SDL_Texture* tex = s->GetTexture();
	b2Fixture* f;
	b2Shape::Type shapeType;
	b2ChainShape* chain;
	Vector2 pPos;
	SDL_Rect dstRect;
	SDL_Point c = { 0,0 };

	pPos = { worldPosition.x, worldPosition.y };

	graphicsRef->Vector2MetersToPixels(pPos);

	dstRect = { (int)(pPos.x - cameraOffset.x),
		(int)(pPos.y - cameraOffset.y),
		(int)pixelDimensions.x, 
		(int)pixelDimensions.y };

	c = { rect.w / 2, rect.h / 2 };

	SDL_RenderCopyEx(graphicsRef->GetRenderer(),
		tex,
		&rect,
		&dstRect,
		zRot,
		&c,
		flipFlags);
}

void TileManager::TileParseTypesFromJSON(std::string json)
{
	std::vector<Tile*> tiles;
	float zRot;
	Direction direction;
	SDL_Rect srcRect;
	std::ifstream fileInputstream(json);
	RSJresource jsonResource(fileInputstream);
	Tile* t;
	TileSpriteSheet* tss = new TileSpriteSheet();

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
	tss->sheet = new Sprite(tss->filepath.c_str(), tss->imageWidth, tss->imageHeight, graphicsRef);

	//LOADED SPRITE SHEET

	//Maybe a loading screen for parsing tiles
	for (Uint8 i = 0; i < tss->columns * tss->columns; i++) {
		SDL_Rect sR = {
			tss->tileWidth * (i % tss->columns) + tss->spacing * (i % tss->columns) + tss->margin,
			tss->tileHeight * (i / tss->columns) + tss->spacing * (i / tss->columns) + tss->margin,
			tss->tileWidth,
			tss->tileHeight
		};

		if(!tss->sheet->CheckIfViableTexture(sR))
			continue;

		Sprite* s = new Sprite(tss->sheet->GetTexture(),
		    &sR,
			Vector2(INT_MIN, INT_MIN),
			Vector3(1.f, 1.f, 1.f),
			Vector2(0.f, 0.f),
			Vector4(255,255,255,255),
			graphicsRef,
			tss->filepath);

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
			s,
			nullptr,
			{ INT32_MIN,INT32_MIN },
			Vector2(tss->tileWidth, tss->tileHeight),
			direction,
			graphicsRef,
			zRot);

		tiles.push_back(t);
	}

	tileTypes.insert_or_assign(tss->name, tiles);
	typeSheets.insert_or_assign(tss->name, tss->sheet);

	tss->sheet = nullptr;

	delete tss;
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
	//for (int i = 0; i < tileTypes.at("Grassland").size(); i++) {
		newTile = new Tile(*tileTypes["Grassland"][0]);
		//if (i == 0)
			newTile->SetCappingDirection(Direction::West);

		newTile->SetSDL_RendererFlipFlags(SDL_FLIP_NONE);
		//newTile->FlipTileSprite();
		newTile->TilePhysicsInit(physics, vector2(x++, y));
		newTile->TileCreateBody(physics);
		tileRow.push_back(newTile);
	//}
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

	tileMap.push_back(tileRow);
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

