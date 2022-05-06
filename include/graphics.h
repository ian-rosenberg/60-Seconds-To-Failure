#pragma once

#include <SDL.h>
#include <SDL_image.h>

#define TARGET_FRAME_RATE 60

const float WIDTH = 1280.0f;
const float HEIGHT = 720.0f;

const float MET_TO_PIX = 20.0f;
const float PIX_TO_MET = 1/MET_TO_PIX;


const float SCALED_WIDTH = WIDTH * PIX_TO_MET;
const float SCALED_HEIGHT = HEIGHT * PIX_TO_MET;

const float RAD2DEG = 180 / M_PI;

const float FRAME_DELAY = 1 / TARGET_FRAME_RATE;

const float MS = (1000.0 / TARGET_FRAME_RATE) / 1000.0;

static float DELTA_TIME;

class Graphics {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	//Used for intialization of graphics, image, audio and text subsystems
	int SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight);

protected:
	float lastUpdateTime;
	float currentUpdateTime;

	float framerate;

public:

	Graphics();

	~Graphics();

	void NextFrame();

	void FrameDelay();

	void SetCurrentUpdateTime();

	SDL_Renderer* GetRenderer();
};