#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "vectortypes.h"

#define TARGET_FRAME_RATE 60.f

const double RAD2DEG = 180 / M_PI;
const float DELTA_TIME = 1 / TARGET_FRAME_RATE;
const Uint32 MS = 1000;
const float MET_TO_PIX = 10.f;
const float PIX_TO_MET = 1 / MET_TO_PIX;

class Graphics {
private:
	SDL_Window*		window;
	SDL_Renderer*	renderer;

	float			accumulator;
	float			newTime;
	float			oldTime;

	int				screenWidth;
	int				screenHeight;

	float			scaledWidth;
	float			scaledHeight;

	//Used for intialization of graphics, image, audio and text subsystems
	int SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight);

public:
	Graphics();

	~Graphics();

	SDL_Renderer* GetRenderer() { return renderer; }
	SDL_Window* GetWindow() { return window; }

	float GetNewTime() { return newTime; }
	float GetOldTime() { return oldTime; }
	float GetScaledWidth() { return scaledWidth; }
	float GetScaledHeight() { return scaledHeight; }
	float GetAccumulatorTime() { return accumulator; }
	float GetFrameDeltaTime() { return newTime - oldTime; }
	Vector2 GetScreenDimensions() { return Vector2(screenWidth, screenHeight); }
	Vector2 GetScaledScreenDimensions() { return Vector2(scaledWidth, scaledHeight); }

	void SetNewTime(Uint32 val) { newTime = val; }
	void SetOldTime(Uint32 val) { oldTime = val; }
	void SetAccumulatorTime(Uint32 val) { accumulator = val; }

	void Vector2PixelsToMeters(Vector2 &val);
	void Vector2MetersToPixels(Vector2 &val);
};