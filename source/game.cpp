#include "graphics.h"
#include "player.h"

int main(int argc, char* argv[]) {
	//const char* testImage = "images/bobby.png";

	SDL_Event input;

	Uint8 quit = 0;

	Graphics* graphics = new Graphics();

	Player* player = new Player(graphics->GetRenderer());
	EntityManager* entityManager = new EntityManager();
	entityManager->AddEntity(player);


	SDL_PollEvent(&input);

	while (!quit) {

		while (input.type != SDL_QUIT) {
			graphics->SetCurrentUpdateTime();

			SDL_PollEvent(&input);

			if (input.type == SDL_QUIT) {
				quit = 1;
			}

			SDL_RenderClear(graphics->GetRenderer());
						
			entityManager->EntityThinkAll();
			entityManager->EntityUpdateAll();
			entityManager->EntityDrawAll();

			graphics->NextFrame();
		}
	}

	delete graphics;

	return 0;
}