#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"
#include "graphics.h"

class Sprite {
protected:
	char* filepath;
	Uint32 frame;
	Uint32 offset;
	Uint32 frameWidth;
	Uint32 frameHeight;

	Vector2 flip;
	Vector2 scale;
	Vector2 scaleCenter;
	Vector2 position;

	Vector3 rotation;

	Vector4 color;

	SDL_Texture* texture;
	SDL_Renderer* renderer;
	
public:
	Sprite();

	Sprite(const char* filepath,
		Vector2 drawPosition,
		Vector2 scale,
		Vector2 scaleCenter,
		Vector3 rotation,
		Vector2 flip,
		Vector4 colorShift,
		Uint32 frame,
		Uint32 offset,
		Uint32 frameWidth,
		Uint32 frameHeight,
		SDL_Renderer* ren);

	Sprite(const char* filepath, 
		Uint32 width, 
		Uint32 height, 
		SDL_Renderer* ren);

	~Sprite();

	Uint8 LoadPNGImage(const char* filepath);

	SDL_Surface* LoadSurface(const char* filepath);

	SDL_Texture* GetTexture();

	Vector2 GetPosition();

	inline const char* GetFilePath() { return (const char*)filepath; }

	void Draw(Sprite* sprite,
		Vector2 drawPosition,
		Vector2* scale,
		Vector2* scaleCenter,
		Vector3* rotation,
		Vector2 flip,
		Vector4* colorShift,
		Uint32 frame,
		Uint32 offset,
		Uint32 frameWidth,
		Uint32 frameHeight);

	void DrawSpriteImage(Sprite* image, Vector2 position, Uint32 width, Uint32 height);
};