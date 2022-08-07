#include "Game.h"


int main() {
	InitWindow(800, 400, "Terraria Clone");
	SetTargetFPS(30);


	Entity::Player player(Math::MutableVec2(400, 200), 40, 20);
	player.tex = LoadTexture("");

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		player.render();
		player.update();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}