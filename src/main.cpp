#include "Game.h"


int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(30);

	Game::Camera camera;

	WorldObjects::Tile tile{Math::MutableVec2{200, 400}, Math::MutableVec2{200, 20}, BLACK};

    Entity::Player player{Math::MutableVec2{250, 250}, 15, 0};
	player.mass = 80;
	player.tex = LoadTexture("../res/Player.png");

	World::players.push_back(&player);
	World::tiles.push_back(&tile);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);

		World::step(25);
		camera.follow(player.pos);

		BeginMode2D(camera.cam);
		World::render();
		EndMode2D();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}