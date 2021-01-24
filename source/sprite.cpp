#include "sprite.h"

Sprite::Sprite()
{
	texture = NULL;
	frame = 0;
	offset = 0;
	frameWidth = 0;
	frameHeight = 0;
	flip = { 0,0 };
	scale = { 0,0 };
	scaleCenter = { 0,0 };
	position = { 0,0 };
	rotation = { 0,0,0 };
	color = { 0,0,0,255 };
	renderer = NULL;
}

Sprite::Sprite(const char* filepath, Vector2 drawPosition, Vector2* scle, Vector2* scleCen, Vector3* rot, Vector2 flp, Vector4* colorShift, Uint32 frm, Uint32 off, Uint32 width, Uint32 height, SDL_Renderer* ren)
{
	frame = frm;
	offset = off;
	frameWidth = width;
	frameHeight = height;
	flip = flp;
	scale = *scle;
	scaleCenter = *scleCen;
	position = drawPosition;
	rotation = *rot;
	color = *colorShift;
	renderer = ren;
	LoadPNGImage(filepath);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}

Uint8 Sprite::LoadPNGImage(const char* filepath)
{
	SDL_Surface* tempSurface = nullptr;
	
	tempSurface = IMG_Load(filepath);

	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

	SDL_FreeSurface(tempSurface);

	if (!texture) {
		std::cout << "Image " << filepath << " could not be loaded!" << std::endl;
		printf("IMG_Load: %s\n", IMG_GetError());
		return 0;
	}

	return 1;
}

SDL_Texture* Sprite::GetTexture()
{
	return texture;
}

Vector2 Sprite::GetPosition()
{
	return position;
}

void Sprite::Draw(Sprite* sprite,
	Vector2 drawPosition,
	Vector2* scale,
	Vector2* scaleCenter,
	Vector3* rotation,
	Vector2 flip,
	Vector4* colorShift,
	Uint32 frame,
	Uint32 offset,
	Uint32 frameWidth,
	Uint32 frameHeight)
{
	SDL_Rect cell, target;
	SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
	SDL_Point r;
	Vector2 scaleFactor = { 1, 1 };
	Vector2 scaleOffset = { 0, 0 };

	if (!sprite)
	{
		return;
	}
	if (scale != NULL)
	{
		vector2_copy(scaleFactor, (*scale));
	}
	else
	{
		vector2_copy(scaleFactor, (vector2(1, 1)));
	}
	if (scaleCenter != NULL)
	{
		vector2_copy(scaleOffset, (*scaleCenter));
	}
	else
	{
		vector2_copy(scaleOffset, (vector2(0.5f, 0.5f)));
	}
	if (rotation != NULL)
	{
		vector2_copy(r, (*rotation));
		r.x *= scaleFactor.x;
		r.y *= scaleFactor.y;
	}
	else
	{
		vector2_copy(r, vector2(0, 1));
	}

	if (flip.x || flip.y)
	{
		if (flip.x)flipFlags = (SDL_RendererFlip)(flipFlags | SDL_FLIP_HORIZONTAL);
		if (flip.y)flipFlags = (SDL_RendererFlip)(flipFlags | SDL_FLIP_VERTICAL);
	}
	else
	{
		flipFlags = SDL_FLIP_NONE;
	}

	if (colorShift)
	{
		SDL_SetTextureColorMod(
			sprite->GetTexture(),
			colorShift->x,
			colorShift->y,
			colorShift->z);
		SDL_SetTextureAlphaMod(
			sprite->GetTexture(),
			colorShift->w);
	}

	gf2d_rect_set(
		cell,
		0,
		(frameHeight * (int)frame),
		frameWidth,
		frameHeight);
	gf2d_rect_set(
		target,
		drawPosition.x - (scaleFactor.x * scaleOffset.x),
		drawPosition.y - (scaleFactor.y * scaleOffset.y),
		frameWidth * scaleFactor.x,
		frameHeight * scaleFactor.y);

	SDL_RenderCopyEx(renderer,
		sprite->GetTexture(),
		&cell,
		&target,
		rotation ? rotation->z : 0,
		rotation ? &r : NULL,
		flipFlags);
	if (colorShift)
	{
		SDL_SetTextureColorMod(
			sprite->GetTexture(),
			255,
			255,
			255);
		SDL_SetTextureAlphaMod(
			sprite->GetTexture(),
			255);
	}
}

void Sprite::DrawSpriteImage(Sprite* image, Vector2 position, Uint32 width, Uint32 height)
{
	Draw(image,
		position,
		NULL,
		NULL,
		NULL,
		{ 0,0 },
		NULL,
		0,
		0,
		width,
		height);
}