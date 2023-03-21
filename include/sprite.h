#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"
#include "graphics.h"

class Sprite {
protected:
	std::string						filepath;
	int								frame;
	int								yOffset;
	int								xOffset;
	int								frameWidth;
	int								frameHeight;

	Vector2							flip;
	Vector2							scale;
	Vector2							scaleCenter;

	Vector3							rotation;

	Vector4							color;

	std::shared_ptr<SDL_Texture>	texture;
	std::shared_ptr<SDL_Surface>	surf;
	SDL_Rect						srcRect;
	std::shared_ptr<Graphics>		graphics;
	
public:
	Sprite();

	Sprite(const Sprite &oldSprite);

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
	
	Sprite(std::shared_ptr<SDL_Texture> tex,

		SDL_Rect* sourceRect,

		Vector2 drawPosition,

		Vector3 rotation,

		Vector2 flip,

		Vector4 colorShift,
		std::shared_ptr<Graphics> ren, std::string fp);

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

	std::shared_ptr<SDL_Surface> LoadSurface(std::string filepath);

	std::shared_ptr<SDL_Texture> GetTexture();

	void RotateTextureZ(float theta);

	std::string GetFilePath() { return (std::string)filepath; }

	std::vector<std::vector<SDL_Color>> GetPixelData();

	SDL_Color translate_color(Uint32 int_color);

	Uint32 GetPixel(std::shared_ptr<SDL_Surface> surface, int x, int y);

	int GetXOffset() { return xOffset; }
	int GetYOffset() { return yOffset; }

	SDL_Rect GetSourceRect() { return srcRect; }

	void SetSourceRectLocation(Vector2 pos);

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

	void Draw(Vector2 drawPosition,
		SDL_Rect srcRect,
		Vector2* scale,
		Vector2* scaleCenter,
		Vector4* colorShift);

	void DrawSpriteImage(Sprite* image, Vector2 position, int width, int height);

	void FlipTexture(SDL_RendererFlip flip);

	bool CheckIfViableTexture(SDL_Rect sR);
};