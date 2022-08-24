#include "Game.h"


int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(Game::FPS_COUNT);
	
	WorldObjects::Tile tile{Math::MutableVec2{200, 400}, Math::MutableVec2{200, 20}, BLACK};
	WorldObjects::Tile tile2{Math::MutableVec2{400, 400}, Math::MutableVec2{200, 20}, BLACK};
	WorldObjects::Tile tile3{Math::MutableVec2{600, 420}, Math::MutableVec2{200, 20}, BLACK};

	World::tiles.push_back(&tile);
	World::tiles.push_back(&tile2);
	World::tiles.push_back(&tile3);

	Game::Camera camera;

    Entity::Player player{Math::MutableVec2{250, 250}, 15};
	player.mass = 80;
	player.tex = LoadTexture("../res/Player.png");

	World::player = &player;
	
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