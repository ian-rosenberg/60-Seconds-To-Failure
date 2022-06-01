#include <gameworld.h>

int main(int argc, char* argv[]) {
	bool flag = false;
	double accumulator = 0.0;

	GameWorld* gameWorld = new GameWorld();
	gameWorld->EnableDebugDraw();

	while (!flag) {	
		flag = gameWorld->GameLoop(accumulator);
	}

	delete gameWorld;

	return EXIT_SUCCESS;
}