#include "graphics.h"

int main(int argc, char* argv[]) {
	Graphics* graphics = new Graphics();
	
	SDL_Event input;

	Uint8 quit = 0;

	SDL_PollEvent(&input);

	while (!quit) {

		while (input.type != SDL_QUIT) {
			graphics->SetCurrentUpdateTime();

			SDL_PollEvent(&input);

			if (input.type == SDL_QUIT) {
				quit = 1;
			}

			graphics->NextFrame();
		}
	}

	delete graphics;

	return 0;
}