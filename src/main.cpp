#include <iostream>
#include <raylib.h>


int main() {

	InitWindow(800, 400, "Test");

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawRectangleLines(300, 300, 50, 50, WHITE);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
