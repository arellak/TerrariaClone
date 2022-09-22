#include "Game.h"

int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(WindowUtils::FPS_COUNT);

	World::camera = Game::Camera{};
	
	textures.push_back(LoadTexture("../res/Player.png"));
	textures.push_back(LoadTexture("../res/penisItem.png"));
	textures.push_back(LoadTexture("../res/blockItem.png"));
	textures.push_back(LoadTexture("../res/japaneseItem.png"));

	auto player = World::createPlayer(Math::MutableVec2{400, 250}, 15);

	player.inventory.addItem(0, Items::createItem(2, "Test Block Item", 25));
	player.inventory.addItem(10, Items::createItem(3, "Japanese Item", 3));
	player.inventory.addItem(3, Items::createItem(1, "Penis Item 1", 25));

	for(int i = 0; i < 100; i++) {
		World::createTile(Math::MutableVec2{(float) (i * WindowUtils::TILE_SIZE), 320.0f}, Math::MutableVec2{(float) WindowUtils::TILE_SIZE, (float) WindowUtils::TILE_SIZE}, BLACK);
	}

	static int radius = 0;
	
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		World::step(25);
		player.inventory.update();
		World::camera.follow(player.pos);

		BeginMode2D(World::camera.cam);
			World::render();
		EndMode2D();
		player.inventory.renderInventory();


		if(radius > 0 && radius < 25) {
			DrawCircleLines(GetMousePosition().x, GetMousePosition().y, radius, RED);
			radius+=3;
		} else if(radius >= 25) {
			radius = 0;
		}

		if(!player.inventory.isOpen) {
			auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
			auto ms = Math::MutableVec2{mousePos.x, mousePos.y};
			ms = ms - (ms % WindowUtils::TILE_SIZE);

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) 
				&& player.inventory.getItemInHand() != nullptr) {
				World::createTile(ms, player.inventory.getItemInHand()->content._id);
			}

			if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
				World::removeTile(ms);
			}

			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				radius++;
				auto selectedTile = World::getSelectedTile(
					Math::MutableVec2{mousePos.x, mousePos.y}
				);
				if(selectedTile != nullptr) {		
					player.selectedTile = selectedTile;
				}
			}
		}

		EndDrawing();
	}
	/*
	Collision error:
	collision isn't layed out to work with the grid in the background, where all tiles are aligned to
	so if i check for collision, i dont check with the real position of the tile but an offset
	*/

	CloseWindow();

	return 0;
}