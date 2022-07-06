#include "graphics.h"
#include <iostream>
#include <algorithm>


int Graphics::SDL2_Init(Uint8 flags, Uint16 windowWidth, Uint16 windowHeight){
	if (SDL_Init(flags) < 0){
		std::cout << "SDL failed to initialize!" << SDL_GetError() << std::endl;
		return -1;
	}
	
	if (!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "SDL_Image failed to initialize!" << std::endl;
		return -1;
	}
	
	window = SDL_CreateWindow("60 Seconds To Failure",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		0);

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
	currentTime = 0.0;
	accumulator = 0.0;

	if (!SDL2_Init(SDL_INIT_EVERYTHING, 1280, 720)) {
		return;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

Graphics::~Graphics() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	std::cout << "SDL Subsystems closed successfully!" << std::endl;
}

void Graphics::NextFrame() {

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}
