#include "Game.h"


int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(30);

	Game::Camera camera;
    Math::MutableVec2 playerPos{250, 250, 5};
	Entity::Player player{playerPos, 20, 20};
	player.tex = LoadTexture("../res/Player.png");

	Math::MutableVec2 obstaclePos {200, 300};
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		player.update();
		camera.follow(player.pos);
		BeginMode2D(camera.cam);
		DrawLine(obstaclePos.getX(), obstaclePos.getY(), obstaclePos.getX() + 200, obstaclePos.getY(), BLACK);
        player.render();
		EndMode2D();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}