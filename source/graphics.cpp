#include "graphics.h"
#include <iostream>
#include <algorithm>


int Graphics::SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight){
	if (SDL_Init(flags) < 0){
		std::cout << "SDL failed to initialize!" << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	screenWidth = 1280;//DM.w;
	screenHeight = 720;// DM.h;
	scaledWidth = screenWidth / PIX_IN_MET;
	scaledHeight = screenHeight / PIX_IN_MET;


	std::cout << "Screen resolution: " << DM.w << "," << DM.h << std::endl;
	
	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		std::cout << "SDL_Image failed to initialize!" << std::endl;
		return -1;
	}
	
	window = SDL_CreateWindow("60 Seconds To Failure",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight,
		SDL_WINDOW_ALLOW_HIGHDPI);

	if (!window) {
		std::cout << "Could not create SDL_Window!" << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		std::cout << "Could not create renderer from SDL_Window" << std::endl;
		return -1;
	}

	std::cout << "SDL Subsystems initialized successfully!" << std::endl;

	return 1;
}

Graphics::Graphics() {
	renderer = NULL;
	window = NULL;
	newTime = 0.0;
	accumulator = 0.f;
	oldTime = 0.f;
	newTime = SDL_GetTicks64();

	if (!SDL2_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER, 1280, 720)) {
		return;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

Graphics::~Graphics() {
	int r = 0;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	std::cout << "SDL Subsystems closed successfully!" << std::endl;
	std::cout << "Press any key to quit..." << std::endl;
}

void Graphics::Vector2PixelsToMeters(Vector2& val)
{
	val.x *= MET_IN_PIX;
	val.y *= MET_IN_PIX;
}

void Graphics::Vector2MetersToPixels(Vector2& val)
{
	val.x = val.x * PIX_IN_MET;//((scaledWidth / 2.0f) + val.x) * PIX_IN_MET;
	val.y = val.y * PIX_IN_MET;//((scaledHeight / 2.0f) + val.y) * PIX_IN_MET;
}

void Graphics::Vector2MetersToPixels(Vector2& val, Vector2 dimensions)
{
	val.x = val.x * PIX_IN_MET;// ((scaledWidth / 2.0f) + val.x)* PIX_IN_MET - dimensions.x / 2;
	val.y = val.y * PIX_IN_MET;// ((scaledHeight / 2.0f) + val.y) * PIX_IN_MET - dimensions.y / 2;
}