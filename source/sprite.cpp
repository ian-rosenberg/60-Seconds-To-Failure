#include "sprite.h"
#include "vectortypes.h"

SpriteManager::Sprite::Sprite(int ID)
{
	texture = NULL;
	id = ID;
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
}

SpriteManager::Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}

Uint8 SpriteManager::Sprite::LoadPNGImage(char* filepath, SDL_Renderer* renderer)
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

SDL_Texture* SpriteManager::Sprite::GetTexture()
{
	return texture;
}

Uint16 SpriteManager::Sprite::GetId()
{
	return id;
}

Vector2 SpriteManager::Sprite::GetPosition()
{
	return position;
}

void SpriteManager::DrawSprite(SDL_Renderer* renderer,
	Sprite* sprite,
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

void SpriteManager::DrawSpriteImage(SDL_Renderer* renderer, Sprite* image, Vector2 position, Uint32 width, Uint32 height)
{
	DrawSprite(renderer,
		image,
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

SpriteManager::SpriteManager(SDL_Renderer *ren)
{
	sprites = new std::vector<Sprite*>();
	renderer = ren;
}

SpriteManager::~SpriteManager()
{
	delete sprites;
}

SpriteManager::Sprite* SpriteManager::GetSpriteById(int id)
{
	if (id < 0 || id >= sprites->size()) {
		std::cout << "Sprite with ID " << id << " was not found!" << std::endl;
		
		return NULL;
	}

	return sprites->at(id);
}

void SpriteManager::LoadSprite(char* filepath)
{
	Sprite* sprite = new Sprite(sprites->size());

	sprite->LoadPNGImage(filepath, renderer);

	sprites->push_back(sprite);
}

void SpriteManager::Draw()
{
	for (int i = 0; i < sprites->size(); i++) {
		Sprite* sprite = sprites->at(i);

		DrawSpriteImage(renderer, sprite, sprite->GetPosition(), 1280, 720);
	}
}



