#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "vectortypes.h"


#define TARGET_FRAME_RATE 60

const float RAD2DEG = 180 / M_PI;

const double FRAME_DELAY = 1 / TARGET_FRAME_RATE;

const double MS = 1000;

const double DELTA_TIME = 0.01;

const float MET_TO_PIX = 1.250f;
const float PIX_TO_MET = 1 / MET_TO_PIX;

class Graphics {
private:
	SDL_Window*		window;
	SDL_Renderer*	renderer;

	double			accumulator;
	double			currentTime;

	int				screenWidth;
	int				screenHeight;

	float			scaledWidth;
	float			scaledHeight;

	//Used for intialization of graphics, image, audio and text subsystems
	int SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight);

public:
	Graphics();

	~Graphics();

	void NextFrame();

	inline SDL_Renderer* GetRenderer() { return renderer; }
	inline SDL_Window* GetWindow() { return window; }

	inline double GetGameTime() { return SDL_GetTicks() / MS; }
	inline double GetCurrentTime() { return currentTime / MS; }
	inline double GetScaledWidth() { return scaledWidth; }
	inline double GetScaledHeight() { return scaledHeight; }
	inline double GetAccumulatorTime() { return accumulator; }
	inline double GetFrameDeltaTime() { return ( SDL_GetTicks() - currentTime ) / MS; }
	inline Vector2 GetScreenDimensions() { return Vector2(screenWidth, screenHeight); }
	inline Vector2 GetScaledScreenDimensions() { return Vector2(scaledWidth, scaledHeight); }

	inline void SetCurrentTime(Uint32 val) { currentTime = val; }
	inline void SetAccumulatorTime(Uint32 val) { accumulator = val; }

	void Vector2PixelsToMeters(Vector2 &val);
	void Vector2MetersToPixels(Vector2 &val);
};