#include <graphics.h>
#include <gameworld.h>

int main(int argc, char* argv[]) {
	//const char* testImage = "images/bobby.png";

	SDL_Event input;

	Uint8 quit = 0;

	Graphics* graphics = new Graphics();
	GameWorld* gameWorld = new GameWorld(graphics->GetRenderer());


	SDL_PollEvent(&input);

	while (!quit) {

		while (input.type != SDL_QUIT) {
			graphics->SetCurrentUpdateTime();

			SDL_PollEvent(&input);

			if (input.type == SDL_QUIT) {
				quit = 1;
			}

			SDL_RenderClear(graphics->GetRenderer());
						
			gameWorld->Update();

			graphics->NextFrame();
		}
	}

	delete graphics;

	delete gameWorld;

	return EXIT_SUCCESS;
}