#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"
#include "graphics.h"

class Sprite {
private:
	void							ClipSourceRectPixels();

protected:
	std::string						filepath;
	int								frame;
	int								yOffset;
	int								xOffset;
	int								frameWidth;
	int								frameHeight;

	SDL_RendererFlip				flipFlags;
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
		const std::shared_ptr<Graphics>& ren);		
	
	Sprite(SDL_Texture* tex,
		SDL_Rect* sourceRect,
		Vector2 drawPosition,
		Vector3 rotation,
		Vector2 flip,
		Vector4 colorShift,
		const std::shared_ptr<Graphics>& ren, std::string fp);

	Sprite(SDL_Texture* t,
		SDL_Rect* sourceRect,
		const std::shared_ptr<Graphics>& ren);

	Sprite(std::string filepath,
		int imgWidth,
		int imgHeight,
		int width, 
		int height,
		int yOffset,
		int xOffset, 
		const std::shared_ptr<Graphics>& ren);

	Sprite(std::string filepath,
		int width,
		int height,
		const std::shared_ptr<Graphics>& graphics);

	Sprite& operator= (const Sprite& other);

	~Sprite();

	static SDL_Texture* CreateRenderTexture(int width,
		int height,
		const std::shared_ptr<Graphics>& graphics, Uint32 fmt);

	static std::vector<std::vector<SDL_Color>> GetPixelData(const char* filepath, SDL_Rect* r, const std::shared_ptr<Graphics>& graphics);

	static std::vector<std::vector<SDL_Color>> GetPixelData(const std::shared_ptr<Graphics>& graphics, Sprite* s);

	static SDL_Color translate_color(Uint32 int_color);

	static Uint32 GetPixel(SDL_Surface* surface, int x, int y);

	Uint8 LoadPNGImage(std::string filepath);

	SDL_Surface* LoadSurface(std::string filepath);

	SDL_Texture* GetTexture();

	void RotateTextureZ(float theta);

	std::string GetFilePath() { return (std::string)filepath; }


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
		SDL_RendererFlip flipFlags);

	void DrawSpriteImage(Sprite* image, Vector2 position, int width, int height);

	Sprite* MakeFlippedTexture(SDL_RendererFlip flip);

	bool CheckIfViableTexture(SDL_Rect sR);

	void UpdateSrcRect(SDL_Rect r);
};