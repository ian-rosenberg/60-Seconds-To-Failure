#pragma once

#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "vectortypes.h"

class SpriteManager {
private:
	class Sprite {
	private:
		Uint16 id;

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
	
	public:
		Sprite(int id);

		~Sprite();

		Uint8 LoadPNGImage(char* filepath, SDL_Renderer* renderer);

		SDL_Texture* GetTexture();

		Uint16 GetId();

		Vector2 GetPosition();
	};

	std::vector<Sprite*>* sprites;
	SDL_Renderer* renderer;

public:
	SpriteManager(SDL_Renderer* ren);

	~SpriteManager();

	Sprite* GetSpriteById(int id);

	void LoadSprite(char* filepath);

	void DrawSprite(SDL_Renderer* renderer,
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
		Uint32 frameHeight);

	void DrawSpriteImage(SDL_Renderer* renderer, Sprite* image, Vector2 position, Uint32 width, Uint32 height);

	void Draw();
};