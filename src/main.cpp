#include "Game.h"


int main() {
	InitWindow(800, 400, "Terraria Clone");
	SetTargetFPS(30);


    Math::MutableVec2 playerPos{250, 250, 5};
    std::cout << playerPos.getX();
	auto player = new Entity::Player{playerPos, 20, 20};
	player->tex = LoadTexture("../res/Player.png");
    
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		player->update();
        player->render();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}