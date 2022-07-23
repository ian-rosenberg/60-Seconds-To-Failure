#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"
#include "graphics.h"

class Sprite {
protected:
	char*						filepath;
	int							frame;
	int							yOffset;
	int							xOffset;
	int							frameWidth;
	int							frameHeight;

	Vector2						flip;
	Vector2						scale;
	Vector2						scaleCenter;

	Vector3						rotation;

	Vector4						color;

	SDL_Texture*				texture;
	SDL_Rect					srcRect;
	std::shared_ptr<Graphics>	graphics;
	
public:
	Sprite();

	Sprite(const char* filepath,
		Vector2 drawPosition,
		Vector2 scale,
		Vector2 scaleCenter,
		Vector3 rotation,
		Vector2 flip,
		Vector4 colorShift,
		int frame,
		int offset,
		int frameWidth,
		int frameHeight,
		std::shared_ptr<Graphics> ren);

	Sprite(const char* filepath,
		int imgWidth,
		int imgHeight,
		int width, 
		int height,
		int yOffset,
		int xOffset, 
		std::shared_ptr<Graphics> ren);

	Sprite(const char* filepath,
		int width,
		int height,
		std::shared_ptr<Graphics> g);

	~Sprite();

	Uint8 LoadPNGImage(const char* filepath);

	SDL_Surface* LoadSurface(const char* filepath);

	SDL_Texture* GetTexture();

	Vector2 GetPosition();

	inline const char* GetFilePath() { return (const char*)filepath; }

	inline int GetXOffset() { return xOffset; }
	inline int GetYOffset() { return yOffset; }

	inline SDL_Rect* GetSourceRect() { return &srcRect; }

	inline Vector3 GetRotation() { return rotation; }

	void Draw(Sprite* sprite,
		Vector2 drawPosition,
		Vector2* scale,
		Vector2* scaleCenter,
		Vector3* rotation,
		Vector2 flip,
		Vector4* colorShift,
		int frame,
		int offset,
		int frameWidth,
		int frameHeight);

	void DrawSpriteImage(Sprite* image, Vector2 position, int width, int height);
};