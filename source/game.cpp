#include <gameworld.h>

int main(int argc, char* argv[]) {
	float acc = 0.f;
	GameWorld* gameWorld = new GameWorld();

	gameWorld->GameLoop(acc);

	delete gameWorld;

	return EXIT_SUCCESS;
}