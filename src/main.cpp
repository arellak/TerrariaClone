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

	auto firstItem = Items::BaseItem{1};
	firstItem.label = "Penis Item 1";

	auto secondItem = Items::BaseItem{2};
	secondItem.label = "Test Block Item";

	auto thirdItem = Items::BaseItem{3};
	thirdItem.label = "Japanese Item";

	player.inventory.addItem(0, Items::InventoryItem{secondItem, 25});
	player.inventory.addItem(10, Items::InventoryItem{thirdItem, 2});
	player.inventory.addItem(3, Items::InventoryItem{firstItem, 5});

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

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player.inventory.getItemInHand() != nullptr) {
				auto ms = Math::MutableVec2{mousePos.x, mousePos.y};
				ms = ms - (ms % WindowUtils::TILE_SIZE);
				World::createTile(ms, player.inventory.getItemInHand()->content._id);
			}

			if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
				auto ms = Math::MutableVec2{mousePos.x, mousePos.y};
				ms = ms - (ms % WindowUtils::TILE_SIZE);
				World::removeTile(ms);
			}

			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				radius++;
				auto selectedTile = World::getSelectedTile(Math::MutableVec2{mousePos.x, mousePos.y});
				if(selectedTile != nullptr) {		
					player.selectedTile = selectedTile;
				}
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}