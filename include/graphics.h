#pragma once

#include <SDL.h>
#include <SDL_image.h>


#define TARGET_FRAME_RATE 60

const float WIDTH = 1280.0f;
const float HEIGHT = 720.0f;

const double MET_TO_PIX = 20.0;
const double PIX_TO_MET = 1/MET_TO_PIX;


const float SCALED_WIDTH = WIDTH * PIX_TO_MET;
const float SCALED_HEIGHT = HEIGHT * PIX_TO_MET;

const float RAD2DEG = 180 / M_PI;

const double FRAME_DELAY = 1 / TARGET_FRAME_RATE;

const double MS = 1000;

const double DELTA_TIME = 0.01;

class Graphics {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	double currentTime;
	double accumulator;

	//Used for intialization of graphics, image, audio and text subsystems
	int SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight);

public:
	Graphics();

	~Graphics();

	void NextFrame();

	SDL_Renderer* GetRenderer();

	inline double GetGameTime() { return SDL_GetTicks() / MS; }
	inline double GetCurrentTime() { return currentTime / MS; }
	inline double GetAccumulatorTime() { return accumulator; }
	inline double GetFrameDeltaTime() { return ( SDL_GetTicks() - currentTime ) / MS; }

	inline void SetCurrentTime(Uint32 val) { currentTime = val; }
	inline void SetAccumulatorTime(Uint32 val) { accumulator = val; }
};