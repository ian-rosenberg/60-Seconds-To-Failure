#include <gameworld.h>

int main(int argc, char* argv[]) {
	bool flag = false;
	float accumulator = 0.0f;

	GameWorld* gameWorld = new GameWorld();
	gameWorld->EnableDebugDraw();

	while (!flag) {	
		flag = gameWorld->GameLoop(accumulator);
	}

	delete gameWorld;

	return EXIT_SUCCESS;
}