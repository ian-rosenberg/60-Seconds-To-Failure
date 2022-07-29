#include "tile.h"
#include "RSJparser.tcc"

std::vector<std::vector<b2Vec2>> Tile::CreatePhysicsEdges()
{
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath().c_str());
	std::vector<std::vector<b2Vec2>> chains;
	std::vector<b2Vec2> c1;
	std::vector<b2Vec2> c2;
	SDL_Rect r = animSprite->GetSprite()->GetSourceRect();
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (direction == 1) {
		//scan north -> south
	}
	else if (direction == 2) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = r.y;
		while (row < r.y + r.h-1) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y )* PIX_TO_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x /4);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h-1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y )* PIX_TO_MET);
				c1.push_back(vert);
				break;
			}
		}

		chains.push_back(c1);

		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		row = r.y;
		while (row < r.y + r.h-1) {
			for (col = r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, (row - r.y )* PIX_TO_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = r.w-1, row = r.y + r.h-1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, (row - r.y )* PIX_TO_MET);
				c2.push_back(vert);
				break;
			}
		}
		
		chains.push_back(c2);
	}
	/*else if (dir == 3) {
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
	}
	else if (dir == 4) {
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

	return chains;
}

void Tile::TilePhysicsInit(b2World* world, Vector2 p)
{
	b2BodyDef bd;

	bd.type = b2_staticBody;
	bd.position.Set(p.x * pixelDimensions.x * PIX_TO_MET, p.y * pixelDimensions.y * PIX_TO_MET);


	
	std::vector<std::vector<b2Vec2>> chains = CreatePhysicsEdges();
	for (std::vector<b2Vec2> chain : chains) {
		b2ChainShape chainShape;
		b2FixtureDef fd;
		b2Vec2 pg = chain.at(0);
		b2Vec2 ng = chain.at(chain.size() - 1);

		chainShape.m_count = chain.size();
		chainShape.m_vertices = chain.data();
		chainShape.CreateChain(chain.data(), chain.size(), pg, ng);

		fd.shape = &chainShape;
	
		if (!physicsBody) {
				bd.angle = M_PI * 1.5f;
			
			physicsBody = world->CreateBody(&bd);
		}
		physicsBody->CreateFixture(&fd);
	}

	debugDraw->SetBodyReference(physicsBody);
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
	direction = 1;
	pixelDimensions = { 0,0 };
	worldDimensions = { 0,0 };

	pixelPosition = { 0,0 };
	worldPosition = { 0,0 };

	physicsBody = nullptr;

	animSprite = nullptr;

	graphicsRef = nullptr;

	debugDraw = nullptr;
}

Tile::Tile(Sprite* s, DebugDraw* dd, Vector2 gridPosition, Vector2 dim, int dir, std::shared_ptr<Graphics> g)
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
}

Tile::Tile(const Tile& oldTile)
{
	id = oldTile.id;
	direction = oldTile.direction;
	pixelDimensions = oldTile.pixelDimensions;
	worldDimensions = oldTile.worldDimensions;
	
	pixelPosition = oldTile.pixelPosition;
	worldPosition = oldTile.worldPosition;

	physicsBody = nullptr;
	
	*animSprite = *(oldTile.animSprite);

	graphicsRef = oldTile.graphicsRef;

	*debugDraw = *(oldTile.debugDraw);
}

Tile::~Tile()
{
	if (animSprite)
		delete animSprite;

	if(debugDraw)
		delete debugDraw;

	graphicsRef.reset();
	graphicsRef = nullptr;

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

	dstRect = { (int)worldToPixelPosition.x, (int)worldToPixelPosition.y - (int)pixelDimensions.y, (int)pixelDimensions.x, (int)pixelDimensions.y};

	SDL_RenderCopyEx(graphicsRef->GetRenderer(),
		tex,
		&rect,
		&dstRect,
		270,
		&c,
		SDL_RendererFlip::SDL_FLIP_NONE);

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
		imgWidth, imgHeight, spacing, direction;
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
		direction = 1;
	else if (jsonResource["direction"].as<std::string>() == std::string("east"))
		direction = 2;
	else if (jsonResource["direction"].as<std::string>() == std::string("south"))
		direction = 3;
	else if (jsonResource["direction"].as<std::string>() == std::string("west"))
		direction = 4;
	else
		direction = 0;

	typeName = jsonResource["name"].as<std::string>();
	filepath = jsonResource["image"].as<std::string>();
	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < tileCount; i++) {
		spacing = i * tileHeight;
		s = new Sprite(filepath.c_str(), imgWidth, imgHeight, tileWidth, tileHeight, i, 0, graphicsRef);
		t = new Tile(s,
			new DebugDraw(graphicsRef, typeName.c_str(), vector2(tileWidth, tileHeight)),
			{0,0},
			vector2( tileWidth, tileHeight ),
			direction,
			graphicsRef);

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
	
	for (auto t = tileTypes.at("pipeTiles").begin(); t != tileTypes.at("pipeTiles").end(); t++) {
		newTile = new Tile();
		newTile = *t;
		newTile->TilePhysicsInit(physics, vector2(x++, y));
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

	graphicsRef.reset();
	graphicsRef = nullptr;
}

void TileManager::UpdateMap()
{
}

void TileManager::DrawMap(Vector2 cameraOffset)
{
	for (int y = 0; y < tileMap.size(); y++) {
		for (int x = 0; x < tileMap[y].size(); x++) {
			tileMap[y][x]->Draw();
		}
	}

}

