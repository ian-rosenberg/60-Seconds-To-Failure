#include "tile.h"
#include "RSJparser.tcc"

std::vector<std::vector<b2Vec2>> Tile::CreatePhysicsEdges(int dir)
{
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath());
	std::vector<std::vector<b2Vec2>> chains;
	std::vector<b2Vec2> c1;
	std::vector<b2Vec2> c2;
	SDL_Rect r = animSprite->GetSprite()->GetSourceRect();
	b2Vec2 vert;
	int col = 0,
		row = 0;

	if (dir == 1) {
		//scan north -> south
	}
	else if (dir == 2) {
		//scan from west -> east
		std::cout << "Scanning west to east" << std::endl;
		row = r.y;
		while (row < r.y + r.h) {
			for (col = 0; col < r.w; col++)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, row * PIX_TO_MET);
					c1.push_back(vert);
					row += (pixelDimensions.x /4);
					break;
				}
			}
		}

		for (col = 0, row = r.y + r.h-1; col < r.w; col++) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, row * PIX_TO_MET);
				c1.push_back(vert);
				break;
			}
		}

		chains.push_back(c1);


		std::cout << "Scanning east to west" << std::endl;

		//scan from east -> west
		row = r.y;
		while (row < r.y + r.h) {
			for (col = r.x +  r.w - 1; col >= 0; col--)
			{
				if (pixels[row][col].a != 0) {
					vert = b2Vec2(col * PIX_TO_MET, row * PIX_TO_MET);
					c2.push_back(vert);
					row += (pixelDimensions.x / 4);
					break;
				}
			}
		}

		for (col = r.x + r.w-1, row = r.y + r.h-1; col >= 0; col--) {
			if (pixels[row][col].a != 0) {
				vert = b2Vec2(col * PIX_TO_MET, row * PIX_TO_MET);
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

void Tile::TilePhysicsInit(b2World* world, int dir)
{
	b2BodyDef bd;

	bd.type = b2_staticBody;
	bd.position.Set(0, 0);

	physicsBody = world->CreateBody(&bd);

	debugDraw->SetBodyReference(physicsBody);
	
	std::vector<std::vector<b2Vec2>> chains = CreatePhysicsEdges(dir);
	for (std::vector<b2Vec2> chain : chains) {
		b2ChainShape chainShape;
		b2FixtureDef fd;
		b2Vec2 pg = chain.at(0);
		b2Vec2 ng = chain.at(chain.size() - 1);

		chainShape.m_count = chain.size();
		chainShape.m_vertices = chain.data();
		chainShape.CreateChain(chain.data(), chain.size(), pg, ng);


		fd.shape = &chainShape;

		physicsBody->CreateFixture(&fd);
	}
}

std::vector<std::vector<SDL_Color>> Tile::GetPixelDataFromFile(const char* file)
{
	int x, y;
	SDL_PixelFormat* fmt;
	SDL_Surface* surf = NULL;
	std::vector<std::vector<SDL_Color>> pixelResults;
	SDL_Color* pixels;
	std::vector<SDL_Color> resultLine;

	int bpp;

	surf = IMG_Load(file);

	SDL_LockSurface(surf);

	pixels = (SDL_Color*)malloc(sizeof(SDL_Color) * surf->pitch * surf->h);
	memset(pixels, 0, sizeof(SDL_Color) * surf->pitch * surf->h);

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

Tile::Tile(Sprite* s, DebugDraw* dd, Vector2 gridPosition, Vector2 playerDim, std::shared_ptr<Graphics> g)
{
	graphicsRef = g;
	animSprite = new Animation(std::string("pipeTile-" + std::to_string((int)gridPosition.x) + "-" + std::to_string((int)gridPosition.y)),
		s,
		pixelDimensions.x,
		pixelDimensions.y,
		s->GetXOffset(),
		s->GetYOffset(),
		{ SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8 });
	pixelDimensions = playerDim;
	debugDraw = dd;
	
	pixelPosition = { gridPosition.x * pixelDimensions.x, gridPosition.y * pixelDimensions.y };
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
	SDL_Point c = { pixelDimensions.x / 2 , pixelDimensions.y / 2 };

	worldPosition = { wPos.x, wPos.y };
	graphicsRef->Vector2MetersToPixels(worldToPixelPosition);

	//vector2_add(pixelPosition, worldToPixelPosition, pixelDimensions);

	dstRect = { (int)worldToPixelPosition.x, (int)worldToPixelPosition.y, (int)pixelDimensions.x, (int)pixelDimensions.y };

	SDL_RenderCopyEx(graphicsRef->GetRenderer(),
		tex,
		&rect,
		&dstRect,
		s->GetRotation().z,
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
		s = new Sprite(filepath.c_str(), imgWidth, imgHeight, tileWidth, tileHeight, i, 0, graphicsRef);
		t = new Tile(s,
			new DebugDraw(graphicsRef, typeName.c_str(), playerDimensions),
			{ -1,-1 },
			playerDimensions,
			graphicsRef);

		t->TilePhysicsInit(physics, direction);

		tiles.push_back(t);

		spacing += tileHeight;
	}

	tilesOfType = std::make_pair(typeName, tiles);

	return tilesOfType;
}

TileManager::TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDim)
{
	std::vector<Tile*> tileRow;

	tileTypes.clear();

	tileMap.clear();

	playerDimensions = playerDim;

	graphicsRef = graphics;

	physics = world;

	tileTypes.insert(TileParseTypesFromJSON("data/tilemap/factory/pipeTiles.json"));
	tileRow.push_back(*tileTypes.at("pipeTiles").begin());

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

