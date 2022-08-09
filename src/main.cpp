#include "Game.h"


int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(30);

	World world;

	Game::Camera camera;

    Entity::Player player{Math::MutableVec2{250, 250}, 20, 20};
	player.mass = 80;
	player.tex = LoadTexture("../res/Player.png");

	world.entities.push_back(&player);

	Math::MutableVec2 obstaclePos {200, 300};
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);

		world.step(25);

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