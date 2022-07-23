#include "sprite.h"

Sprite::Sprite()
{
	texture = NULL;
	frame = 0;
	yOffset = 0;
	frameWidth = 0;
	frameHeight = 0;
	flip = { 0,0 };
	scale = { 0,0 };
	scaleCenter = { 0,0 };
	rotation = { 0,0,0 };
	color = { 0,0,0,255 };
	graphics = NULL;
	filepath = nullptr;
	srcRect = { 0,0,0,0 };
}

Sprite::Sprite(const char* fp, Vector2 drawPosition, Vector2 scle, Vector2 scleCen, Vector3 rot, Vector2 flp, Vector4 colorShift, int frm, int off, int width, int height, std::shared_ptr<Graphics> ren)
{
	filepath = (char*)fp;
	frame = frm;
	yOffset = off;
	frameWidth = width;
	frameHeight = height;
	flip = flp;
	scale = scle;
	scaleCenter = scleCen;
	rotation = rot;
	color = colorShift;
	graphics = ren;
	srcRect = { 0, frameHeight * off, frameWidth, frameHeight};
	LoadPNGImage(filepath);
}

Sprite::Sprite(const char* filepath, int width, int height, std::shared_ptr<Graphics> ren)
{
	this->filepath = (char*)filepath;
	frame = 1.0f;
	yOffset = 0;
	frameWidth = width;
	frameHeight = height;
	flip = {};
	scale = {};
	scaleCenter = {};
	rotation = {};
	color = {};
	graphics = ren;
	srcRect = { 0, 0, width, height };
	LoadPNGImage(filepath);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	graphics = nullptr;
}

Sprite::Sprite(const char* filepath, int imgWidth, int imgHeight, int width, int height, int yOffset, int xOffset, std::shared_ptr<Graphics> ren)
{
	frame = 1.0f;
	this->filepath = (char*)filepath;
	this->yOffset = yOffset;
	this->xOffset = xOffset;
	frameWidth = width;
	frameHeight = height;
	flip = {};
	scale = {};
	scaleCenter = {};
	rotation = {};
	color = {SDL_MAX_UINT8,SDL_MAX_UINT8,SDL_MAX_UINT8,SDL_MAX_UINT8 };
	graphics = ren;
	srcRect = { frameWidth * xOffset, frameHeight * yOffset, frameWidth, frameHeight };
	LoadPNGImage(filepath);
}

Uint8 Sprite::LoadPNGImage(const char* filepath)
{
	SDL_Surface* tempSurface = nullptr;
	
	tempSurface = IMG_Load(filepath);

	texture = SDL_CreateTextureFromSurface(graphics->GetRenderer(), tempSurface);

	SDL_FreeSurface(tempSurface);

	if (!texture) {
		std::cout << "Image " << filepath << " could not be loaded!" << std::endl;
		printf("IMG_Load: %s\n", IMG_GetError());
		return 0;
	}

	std::cout << "Image " << filepath << " was loaded!" << std::endl;

	return 1;
}

SDL_Surface* Sprite::LoadSurface(const char* filepath)
{
	return IMG_Load(filepath);
}

SDL_Texture* Sprite::GetTexture()
{
	return texture;
}

void Sprite::Draw(Sprite* sprite,
	Vector2 drawPosition,
	Vector2* scale,
	Vector2* scaleCenter,
	Vector3* rotation,
	Vector2 flip,
	Vector4* colorShift,
	int frame,
	int offset,
	int frameWidth,
	int frameHeight)
{
	SDL_Rect cell, target;
	SDL_RendererFlip flipFlags = SDL_FLIP_NONE;
	Vector3 r = {};
	SDL_Point c;
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
		vector2_copy(c, (*rotation));
		c.x *= scaleFactor.x;
		c.y *= scaleFactor.y;
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

	SDL_RenderCopyEx(graphics->GetRenderer(),
		sprite->GetTexture(),
		&cell,
		&target,
		rotation->z,
		NULL,
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

void Sprite::DrawSpriteImage(Sprite* image, Vector2 position, int width, int height)
{
	Vector2 scale = { 1,1 };
	Vector2 scaleCenter = { width / 2.0f, height / 2.0f };
	Vector3 rotation = { 0,0,0 };
	Vector4 colorShift = Vector4(SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8);
	Draw(image,
		position,
		&scale,
		&scaleCenter,
		&rotation,
		{ 0,0 },
		&colorShift,
		0,
		0,
		width,
		height);
}