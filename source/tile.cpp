#include "tile.h"
#include "RSJparser.tcc"

std::vector<b2Vec2> Tile::CreatePhysicsEdges()
{
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath());
	std::vector<b2Vec2> chain;
	b2Vec2 vert;
	Vector2 pixel;
	int col=0, row=pixels.size()-1;

	while(col < pixels[row].size()) {
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

	return chain;
}

void Tile::TilePhysicsInit(b2World* world)
{
	b2ChainShape chainShape;
	b2BodyDef bd;
	b2FixtureDef fd;
	std::vector<b2Vec2> verts = CreatePhysicsEdges();
	b2Vec2 pg = verts.at(0);
	b2Vec2 ng = verts.at(verts.size() - 1);

	chainShape.m_count = verts.size();
	chainShape.m_vertices = verts.data();
	chainShape.CreateChain(verts.data(), verts.size(), pg, ng);

	bd.type = b2_staticBody;
	bd.position.SetZero();

	physicsBody = world->CreateBody(&bd);
	debugDraw->SetBodyReference(physicsBody);


	fd.shape = &chainShape;

	physicsBody->CreateFixture(&fd);
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
	animSprite = new Animation(std::string("pipeTile-" + std::to_string(gridPosition.x) + "-" + std::to_string(gridPosition.y)),
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

	graphicsRef.reset();
	graphicsRef = nullptr;

	delete debugDraw;
}

void Tile::Draw()
{
	Sprite* s = animSprite->GetSprite();
	b2Fixture* f;
	b2Shape::Type shapeType;
	b2ChainShape* chain;
	b2Vec2 wPos = physicsBody->GetPosition();
	Vector2 drawPosition = { wPos.x, wPos.y };
	Vector2 scale = { 1,1 };
	Vector2 scaleCenter = { 0.5f, 0.5f };
	Vector2 flip = { 0,0 };
	Vector3 rotation = { 0,0,0 };
	Vector4 colorShift = { SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8 };

	graphicsRef->Vector2MetersToPixels(drawPosition);

	s->Draw(s,
		drawPosition,
		&scale,
		&scaleCenter,
		&rotation,
		flip,
		&colorShift,
		0, 0, pixelDimensions.x, pixelDimensions.y);

	//Debug Drawing
	if (debugDraw) {
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
	typeName = jsonResource["name"].as<std::string>();
	filepath = jsonResource["image"].as<std::string>();
	//Maybe a loading screen for parsing tiles
	for (int i = 0; i < tileCount; i++) {
		s = new Sprite(filepath.c_str(), imgWidth, imgHeight, tileWidth, tileHeight, spacing, 0, graphicsRef);
		t = new Tile(s,
			new DebugDraw(graphicsRef, typeName.c_str(), playerDimensions),
			{ -1,-1 },
			playerDimensions,
			graphicsRef);

		t->TilePhysicsInit(physics);

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
	tileTypes.at("pipeTiles")[0]->Draw();
	SDL_RenderPresent(graphics->GetRenderer());
	/*std::vector<b2Vec2> verts = testTile->CreatePhysicsEdges();

	pg = verts.at(0);
	ng = verts.at(verts.size() - 1);
	
	chainShape.m_count = verts.size();
	chainShape.m_vertices = verts.data();
	chainShape.CreateChain(verts.data(), verts.size(), pg, ng);
	
	bd.type = b2_staticBody;
	bd.position.SetZero();
	
	b = world->CreateBody(&bd);
	ddRef->SetBodyReference(b);


	fd.shape = &chainShape;

	b->CreateFixture(&fd);

	testTile->SetBody(b);

	tileRow.push_back(testTile);*/
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

