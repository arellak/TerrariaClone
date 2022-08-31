#include "Game.h"


int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(WindowUtils::FPS_COUNT);

	World::camera = Game::Camera{};

	auto player = World::createPlayer(Math::MutableVec2{400, 250}, 15);

	auto firstItem = Items::BaseItem{"../res/penisItem.png"};
	firstItem.label = "Penis Item 1";
	auto secondItem = Items::BaseItem{"../res/penisItem2.png"};
	secondItem.label = "Penis Item 2";

	auto thirdItem = Items::BaseItem{"../res/japaneseItem.png"};
	thirdItem.label = "Japanese Item";

	player.inventory.addItem(0, Items::InventoryItem{secondItem, 2});
	player.inventory.addItem(10, Items::InventoryItem{thirdItem, 2});
	player.inventory.addItem(3, Items::InventoryItem{firstItem, 5});

	for(int i = 0; i < 40; i++) {
		World::createTile(Math::MutableVec2{350.0f + (i * WindowUtils::TILE_SIZE), 350.0f}, Math::MutableVec2{WindowUtils::TILE_SIZE, WindowUtils::TILE_SIZE}, BLACK);
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

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				auto ms = Math::MutableVec2{mousePos.x, mousePos.y};
				ms.setX(ms.getX() - ((int) ms.getX() % WindowUtils::TILE_SIZE));
				ms.setY(ms.getY() - ((int) ms.getY() % WindowUtils::TILE_SIZE));
				World::createTile(ms, Math::MutableVec2{WindowUtils::TILE_SIZE, WindowUtils::TILE_SIZE}, BLACK);
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