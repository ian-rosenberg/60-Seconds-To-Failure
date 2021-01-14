#include "graphics.h"
#include "sprite.h"

int main(int argc, char* argv[]) {
	const char* testImage = "images/bobby.png";

	SDL_Event input;

	Uint8 quit = 0;

	Graphics* graphics = new Graphics();
	SpriteManager* spriteManager = new SpriteManager(graphics->GetRenderer());
	spriteManager->LoadSprite((char*)testImage);

	SDL_PollEvent(&input);

	while (!quit) {

		while (input.type != SDL_QUIT) {
			graphics->SetCurrentUpdateTime();

			SDL_PollEvent(&input);

			if (input.type == SDL_QUIT) {
				quit = 1;
			}

			SDL_RenderClear(graphics->GetRenderer());

			spriteManager->Draw();

			graphics->NextFrame();
		}
	}

	delete graphics;

	return 0;
}