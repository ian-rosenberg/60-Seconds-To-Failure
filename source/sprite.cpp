#include "sprite.h"

Sprite::Sprite()
{
	texture = NULL;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}

Uint8 Sprite::LoadPNGImage(char* filepath, SDL_Rect source)
{


	return 1;
}
