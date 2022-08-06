#include "Game.h"


int main() {
	InitWindow(800, 400, "Terraria Clone");

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawRectangleLines(300, 300, 50, 50, BLACK);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}