#include "tile.h"
#include "RSJparser.tcc"

#define EDGE_COUNT 2
#define MAX_CAPS 2
#define NORTH_ROTATION 0.f
#define EAST_ROTATION 

std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> Tile::CreatePhysicsEdges()
{
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> chainPair;
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath().c_str());
	std::vector<b2Vec2> c1;
	std::vector<b2Vec2> c2;
	SDL_Rect r = animSprite->GetSprite()->GetSourceRect();
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (direction == Direction::North) {
		//scan north -> south
	}
	else if (direction == Direction::East) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = r.y;
		while (row < r.y + r.h - 1) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
				c1.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		row = r.y;
		while (row < r.y + r.h - 1) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
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
		std::cout << "Scanning east to west" << std::endl;

		while (row < r.y + r.h - 1) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h - 1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
				c1.push_back(vert);
				break;
			}
		}

		std::cout << "Scanning west to east" << std::endl;

		//scan from west -> east
		row = r.y;
		while (row < r.y + r.h - 1) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = r.w - 1, row = r.y + r.h - 1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
				c2.push_back(vert);
				break;
			}
		}
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

	//Decide to cap north, south or both
	if (capDirection != Direction::None) {
		if (capDirection & Direction::East) {
			c3.push_back(c1.back());

			for (col = c1.back().x + (pixelDimensions.x / 4.f); col < c2.back().x;) {
				for (row = c1.back().x; pixels[row][col].a != 0; row++)
					continue;

				col -= (pixelDimensions.y / 4.f);
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
				c3.push_back(vert);
			}


			c3.push_back(c2.back());

			cappings.first = c3;
		}

		if (capDirection & Direction::West) {
			c4.push_back(c1.front());

			for (col = c1.front().x + (pixelDimensions.x / 4.f); col < c2.front().x;) {
				for (row = 0; pixels[row][col].a != 0; row--)
					continue;

				col += (pixelDimensions.y / 4.f);
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y) * PIX_TO_MET);
				c4.push_back(vert);
			}


			c4.push_back(c2.front());

			cappings.second = c4;
		}
	}

	return cappings;
}

/*
Change to have the pair of chains flipped and rotated, THEN capped.
*/
void Tile::TilePhysicsInit(b2World* world, Vector2 p, SDL_RendererFlip flip)
{
	b2BodyDef bd;
	float temp;
	b2FixtureDef fd;
	b2Vec2 pg;
	b2Vec2 ng;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> chains;
	std::pair<std::vector<b2Vec2>, std::vector<b2Vec2>> cappings;
	std::vector<std::pair<std::vector<b2Vec2>, bool>> chainsAndCaps;
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath().c_str());

	flipFlags = flip;

	bd.type = b2_staticBody;
	bd.position.Set(p.x * pixelDimensions.x * PIX_TO_MET, p.y * pixelDimensions.y * PIX_TO_MET);

	chains = CreatePhysicsEdges();


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

	//Logic for ghost vertices ( previous and next ) goes here!!!

		
	RotateChain(chains.first, bd.angle);

	FlipChain(chains.first, flip);	
	
	RotateChain(chains.second, bd.angle);

	FlipChain(chains.second, flip);

	cappings = DecideCapping(chains.first, chains.second, pixels, animSprite->GetSprite()->GetSourceRect());

	if (!chains.first.empty())
		chainsAndCaps.push_back(std::make_pair(chains.first, true));
	if (!chains.second.empty())
		chainsAndCaps.push_back(std::make_pair(chains.second, true));
	if (!cappings.first.empty())
		chainsAndCaps.push_back(std::make_pair(cappings.first, false));
	if (!cappings.second.empty())
		chainsAndCaps.push_back(std::make_pair(cappings.second, false));

	for (std::pair<std::vector<b2Vec2>, bool> chain : chainsAndCaps) {
		b2ChainShape chainShape;

		chainShape.CreateChain(static_cast<b2Vec2*>(chain.first.data()), chain.first.size(), chain.first.front(), chain.first.back());

		fd.shape = &chainShape;
		fd.friction = 0.9f;
		physicsBody->CreateFixture(&fd);
	}
	CreateDebugDraw();

	debugDraw->SetBodyReference(physicsBody);
}

void Tile::SetCappingDirection(Direction capping)
{
	this->capDirection = capping;
}

std::vector<std::vector<SDL_Color>> Tile::GetPixelDataFromFile(const char* file)
{
	int x, y;
	SDL_PixelFormat* fmt;
	SDL_Surface* surf = NULL;
	std::vector<std::vector<SDL_Color>> pixelResults;
	std::vector<SDL_Color> pixels;
	std::vector<SDL_Color> resultLine;

	int bpp;

	surf = IMG_Load(file);

	SDL_LockSurface(surf);

	pixels.resize(sizeof(SDL_Color) * surf->pitch * surf->h);

	fmt = surf->format;
	bpp = surf->format->BytesPerPixel;
	//cols then rows
	for (y = 0; y < surf->h; y++)
	{
		resultLine.clear();
		for (x = 0; x < surf->w; x++)
		{
			SDL_GetRGBA(GetPixel(surf, x, y),
				fmt,
				&pixels[(y * surf->w + x)].r,
				&pixels[(y * surf->w + x)].g,
				&pixels[(y * surf->w + x)].b,
				&pixels[(y * surf->w + x)].a);
			resultLine.push_back(pixels[(y * surf->w + x)]);
		}
		pixelResults.push_back(resultLine);
	}

	SDL_UnlockSurface(surf);
	SDL_FreeSurface(surf);

	return pixelResults;
}

//from SDL docs
Uint32 Tile::GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
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

Tile::Tile(Sprite* s, DebugDraw* dd, Vector2 gridPosition, Vector2 dim, Direction dir, std::shared_ptr<Graphics> g, float zRotation)
{
	graphicsRef = g;
	animSprite = new Animation(std::string("pipeTile-" + std::to_string((int)gridPosition.x) + "-" + std::to_string((int)gridPosition.y)),
		s,
		pixelDimensions.x,
		pixelDimensions.y,
		s->GetXOffset(),
		s->GetYOffset(),
		{ SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8 });
	pixelDimensions = dim;
	debugDraw = dd;

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

	animSprite = new Animation(oldTile.animSprite);

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

}

void Tile::RotateChain(std::vector<b2Vec2>& chain, float theta)
{
	float c = 0, s = 0;

	c = cos(theta);
	s = sin(theta);

	for (b2Vec2 vert : chain) {
		vert.x = vert.x * c + vert.y * s;
		vert.y = -vert.x * s + vert.y * c;
	}
}

void Tile::FlipChain(std::vector<b2Vec2>& chain, SDL_RendererFlip flip) {
	switch (flip) {
	case SDL_FLIP_HORIZONTAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			std::swap(chain[i].y, chain[j].y);
		break;
	case SDL_FLIP_VERTICAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			std::swap(chain[i].x, chain[j].x);
		break;
	case SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL:
		for (int i = 0, j = chain.size() - 1; i < j; i++, j--)
			std::swap(chain[i], chain[j]);
		break;
	default:
		break;
	}
}

void Tile::Draw()
{
	Sprite* s = animSprite->GetSprite();
	SDL_Rect rect = s->GetSourceRect();
	SDL_Texture* tex = s->GetTexture();
	b2Fixture* f;
	b2Shape::Type shapeType;
	b2ChainShape* chain;
	b2Vec2 wPos = physicsBody->GetPosition();
	Vector2 worldToPixelPosition = { wPos.x, wPos.y };
	SDL_Rect dstRect;
	SDL_Point c = { pixelDimensions.x / 2.0 , pixelDimensions.y / 2.0 };

	worldPosition = { wPos.x, wPos.y };
	graphicsRef->Vector2MetersToPixels(worldToPixelPosition);

	dstRect = { (int)worldToPixelPosition.x, (int)worldToPixelPosition.y - (int)pixelDimensions.y, (int)pixelDimensions.x, (int)pixelDimensions.y };

	SDL_RenderCopyEx(graphicsRef->GetRenderer(),
		tex,
		&rect,
		&dstRect,
		zRot,
		&c,
		flipFlags);

	//Debug Drawing
	if (debugDraw) {
		debugDraw->UpdateBodyPosition(worldPosition);

		for (f = physicsBody->GetFixtureList(); f; f = f->GetNext()) {
			shapeType = f->GetType();

			if (shapeType == b2Shape::e_chain) {
				chain = (b2ChainShape*)f->GetShape();
				debugDraw->DrawChainShape(chain->m_vertices, chain->m_count, chain->m_prevVertex, chain->m_nextVertex);
			}
		}
	}
}

std::pair<std::string, std::vector<Tile*>> TileManager::TileParseTypesFromJSON(std::string json)
{
	std::pair<std::string, std::vector<Tile*>> tilesOfType;
	std::vector<Tile*> tiles;
	std::string typeName;
	std::string filepath;
	int tileCount, tileWidth, tileHeight,
		imgWidth, imgHeight, spacing;
	float zRot;
	Direction direction;
	SDL_Rect srcRect;
	std::ifstream fileInputstream(json);
	RSJresource jsonResource(fileInputstream);
	Sprite* s;
	Tile* t;

	tileCount = jsonResource["tilecount"].as<int>();
	tileWidth = jsonResource["tilewidth"].as<int>();
	tileHeight = jsonResource["tileheight"].as<int>();
	imgWidth = jsonResource["imagewidth"].as<int>();
	imgHeight = jsonResource["imageheight"].as<int>();
	spacing = jsonResource["spacing"].as<int>();
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

	typeName = jsonResource["name"].as<std::string>();
	filepath = jsonResource["image"].as<std::string>();
	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < tileCount; i++) {
		spacing = i * tileHeight;
		s = new Sprite(filepath.c_str(), imgWidth, imgHeight, tileWidth, tileHeight, i, 0, graphicsRef);

		switch (direction) {
		case North:
			zRot = 180.f;
			break;

		case East:
			zRot = 270.f;
			break;

		case South:
			zRot = 0.f;
			break;

		case West:
			zRot = 90.f;
			break;

		default:
			zRot = 0.f;
			break;
		}

		s->RotateTextureZ(zRot);

		t = new Tile(s,
			nullptr,
			{ INT8_MIN,INT8_MIN },
			vector2(tileWidth, tileHeight),
			direction,
			graphicsRef,
			zRot);

		tiles.push_back(t);
	}

	tilesOfType = std::make_pair(typeName, tiles);

	return tilesOfType;
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

	tileTypes.insert(TileParseTypesFromJSON("data/tilemap/factory/pipeTiles.json"));

	for (int i = 0; i < tileTypes.at("pipeTiles").size(); i++) {
		newTile = new Tile(*tileTypes["pipeTiles"][i]);
		if (i == 0)
			newTile->SetCappingDirection(Direction::West);

		newTile->TilePhysicsInit(physics, vector2(x++, y), SDL_FLIP_NONE);
		tileRow.push_back(newTile);
	}
	for (int i = tileRow.size() - 1; i >= 1; i--) {
		newTile = new Tile(*tileRow[i]);

		if (i == 1) {
			newTile->SetCappingDirection(Direction::East);
		}

		newTile->TilePhysicsInit(physics, vector2(x++, y), SDL_FLIP_VERTICAL);
		tileRow.push_back(newTile);
	}

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
				tileMap[y][x]->Draw();
		}
	}

}

