#include "tile.h"

int Tile::CreatePhysicsEdges(SDL_Surface* readSurface)
{
	std::vector<Vector2> topPixels;
	Vector2 pix;
	b2Vec2* edge;
	int pitch, w, h, bpp;
	Uint32* pixel;
	Uint8 creatingEdge = 0,
		r = 0,g = 0,b = 0,a = 0;

	SDL_LockSurface(readSurface);
	bpp = readSurface->format->BytesPerPixel;


	for (int x = 0; x < readSurface->w; x++) {
		for (int y = 0; y < readSurface->h; y++) {
			pixel = (Uint32*)readSurface->pixels + y * readSurface->pitch + x * bpp;
			SDL_GetRGBA(*pixel, readSurface->format, &r, &g, &b, &a);
			//0 alpha is fully transparent
			if (a != 0) {
				topPixels.push_back(vector2(x, y - 1));
				break;
			}

		}
	}

	

	SDL_UnlockSurface(readSurface);
	SDL_FreeSurface(readSurface);
	return 0;
}

Tile::Tile(std::shared_ptr<Graphics> graphics)
{
	graphicsRef = graphics;
	animSprite = new Animation(std::string("idle"), std::string("images/GeometryTest.png"), 1, 64,64,0,vector4(0,0,0,0), 1.0f, 0.0f, AnimationType::AT_LOOP, graphics->GetRenderer());
	
	CreatePhysicsEdges(animSprite->GetSprite()->LoadSurface("images/GeometryTest.png"));
}

Tile::~Tile()
{
	edges.clear();

	if (animSprite)
		delete animSprite;

	graphicsRef.reset();
	graphicsRef = nullptr;
}

void Tile::Draw()
{
}

TileManager::TileManager(const char* filepath, std::shared_ptr<Graphics> graphics)
{
	tileMap = new std::vector<std::vector<Tile*>*>();
	graphicsRef = graphics;

	Tile* testTile = new Tile(graphics);
}

TileManager::~TileManager()
{
	while (!tileMap->empty()) {
		std::vector<Tile*>* row = tileMap->front();
		tileMap->erase(tileMap->begin());

		while (!row->empty()) {
			Tile* t = row->front();
			row->erase(row->begin());
			delete t;
		}
	}

	graphicsRef.reset();
	graphicsRef = nullptr;
}
