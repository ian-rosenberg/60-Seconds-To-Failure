#pragma once

#include <SDL.h>
#include <SDL_image.h>

#define FRAME_DELAY 16

class Graphics {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	//Used for intialization of graphics, image, audio and text subsystems
	int SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight);

protected:
	int32_t deltaTime;
	Uint32 lastUpdateTime;
	Uint32 currentUpdateTime;

	float framerate;

public:

	Graphics();

	~Graphics();

	void NextFrame();

	void FrameDelay();

	void SetCurrentUpdateTime();

	SDL_Renderer* GetRenderer();
};