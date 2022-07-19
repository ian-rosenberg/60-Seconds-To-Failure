#include "tile.h"

std::vector<b2Vec2> Tile::CreatePhysicsEdges()
{
	std::vector<std::vector<SDL_Color>> pixels = GetPixelDataFromFile(animSprite->GetSprite()->GetFilePath());
	std::vector<b2Vec2> chain;
	b2Vec2 vert;


	for (int col = 0; col < pixels.size(); col++)
	{
		for (int row = pixels.size()-1; row >= 0; row--)
		{
			if (pixels[row][col].a == 0) {
				vert = { b2Vec2(col * PIX_TO_MET, (row+2) * PIX_TO_MET)};
				chain.push_back(vert);
				col += (pixelDimensions.x/2);
				break;
			}
		}
	}

	return chain;
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

Tile::Tile(std::shared_ptr<Graphics> graphics, Vector2 playerDim, DebugDraw* dd)
{
	Vector2 pixDim = playerDim;

	graphicsRef = graphics;
	animSprite = new Animation(std::string("idle"), 
		std::string("images/GeometryTest.png"), 
		1, 
		64,
		64,
		0,
		vector4(255,255,255,255),
		1.0f,
		0.0f,
		AnimationType::AT_LOOP,
		graphics->GetRenderer());
	pixelDimensions = playerDim;
	debugDraw = dd;
}

Tile::~Tile()
{
	chainShape = nullptr;

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

	pixelPosition = { wPos.x, wPos.y };

	graphicsRef->Vector2MetersToPixels(pixelPosition);

	pixelPosition.x += (pixelDimensions.x / 2.0);
	pixelPosition.y += (pixelDimensions.y / 2.0);


	animSprite->GetSprite()->DrawSpriteImage(s, pixelPosition, pixelDimensions.x, pixelDimensions.y);

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

TileManager::TileManager(const char* filepath, std::shared_ptr<Graphics> graphics, b2World* world, Vector2 playerDimensions, DebugDraw* ddRef)
{
	std::vector<Tile*> tileRow;
	b2ChainShape chainShape;
	b2BodyDef bd;
	b2Body* b;
	b2FixtureDef fd;
	b2Vec2 pg, ng;

	tileMap.clear();

	graphicsRef = graphics;

	Tile* testTile = new Tile(graphics, playerDimensions, ddRef);
	std::vector<b2Vec2> verts = testTile->CreatePhysicsEdges();

	pg = verts.at(0);
	ng = verts.at(verts.size() - 1);
	
	chainShape.m_count = verts.size();
	chainShape.m_vertices = verts.data();
	chainShape.CreateChain(verts.data(), verts.size(), pg, ng);
	
	bd.type = b2_staticBody;
	bd.position = b2Vec2(0, 0);
	
	b = world->CreateBody(&bd);
	ddRef->SetBodyReference(b);


	fd.shape = &chainShape;

	b->CreateFixture(&fd);

	testTile->SetBody(b);

	tileRow.push_back(testTile);
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

