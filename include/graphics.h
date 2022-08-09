#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "vectortypes.h"

#define TARGET_FRAME_RATE 60.0f

const double RAD2DEG = 180 / M_PI;
const float FRAME_DELAY = 1 / TARGET_FRAME_RATE;
const Uint32 MS = 1000;
const double DELTA_TIME = 0.01;
const float MET_TO_PIX = 10.f;
const float PIX_TO_MET = 1 / MET_TO_PIX;
const int FRAME_DELAY_MS = FRAME_DELAY * MS;

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

	inline SDL_Renderer* GetRenderer() { return renderer; }
	inline SDL_Window* GetWindow() { return window; }

	inline float GetNewTime() { return newTime; }
	inline float GetOldTime() { return oldTime; }
	inline float GetScaledWidth() { return scaledWidth; }
	inline float GetScaledHeight() { return scaledHeight; }
	inline float GetAccumulatorTime() { return accumulator; }
	inline float GetFrameDeltaTime() { return newTime - oldTime; }
	inline Vector2 GetScreenDimensions() { return Vector2(screenWidth, screenHeight); }
	inline Vector2 GetScaledScreenDimensions() { return Vector2(scaledWidth, scaledHeight); }

	inline void SetNewTime(Uint32 val) { newTime = val; }
	inline void SetOldTime(Uint32 val) { oldTime = val; }
	inline void SetAccumulatorTime(Uint32 val) { accumulator = val; }

	void Vector2PixelsToMeters(Vector2 &val);
	void Vector2MetersToPixels(Vector2 &val);
};