#include "Game.h"


int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(Game::FPS_COUNT);
	
	World::createTile(Math::MutableVec2{200, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{400, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{600, 420}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{600, 540}, Math::MutableVec2{200, 20}, BLACK);

	Game::Camera camera;

	World::createPlayer(Math::MutableVec2{250, 250}, 15);
	
	
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);

		World::step(25);

		BeginMode2D(camera.cam);
			World::render();
		EndMode2D();

		auto selectedTile = World::getSelectedTile(camera.cam);
		if(selectedTile != nullptr) {
			std::cout << selectedTile->position.toString() << std::endl;
		}

		camera.follow(World::activePlayer->pos);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}