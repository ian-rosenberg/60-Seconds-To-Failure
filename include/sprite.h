#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"
#include "graphics.h"

class Sprite {
protected:
	std::string					filepath;
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

	Sprite(Sprite* oldSprite);

	Sprite(std::string filepath,
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

	Sprite(std::string filepath,
		int imgWidth,
		int imgHeight,
		int width, 
		int height,
		int yOffset,
		int xOffset, 
		std::shared_ptr<Graphics> ren);

	Sprite(std::string filepath,
		int width,
		int height,
		std::shared_ptr<Graphics> g);

	~Sprite();

	Uint8 LoadPNGImage(std::string filepath);

	SDL_Surface* LoadSurface(std::string filepath);

	SDL_Texture* GetTexture();

	Vector2 GetPosition();

	std::string GetFilePath() { return (std::string)filepath; }

	int GetXOffset() { return xOffset; }
	int GetYOffset() { return yOffset; }

	SDL_Rect GetSourceRect() { return srcRect; }

	Vector3 GetRotation() { return rotation; }

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