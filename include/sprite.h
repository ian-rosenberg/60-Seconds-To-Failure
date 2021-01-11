#pragma once

#include <SDL_image.h>

#include <vector>

class Sprite {
private:
	SDL_Texture* texture;

public:
	Sprite();

	~Sprite();

	Uint8 LoadPNGImage(char* filepath, SDL_Rect source);
};

class SpriteManager {
private:
	std::vector<Sprite> sprites;

public:

};