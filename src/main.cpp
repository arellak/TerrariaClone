#include "Game.h"


int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(WindowUtils::FPS_COUNT);
	
	World::createTile(Math::MutableVec2{200, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{400, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{600, 420}, Math::MutableVec2{200, 20}, BLACK);


	World::createTile(Math::MutableVec2{600, 540}, Math::MutableVec2{200, 20}, BLACK);

	World::camera = Game::Camera{};

	World::createPlayer(Math::MutableVec2{250, 250}, 15);
	
	Game::Inventory inventory;
	auto firstItem = Items::BaseItem{"../res/penisItem.png"};
	firstItem.label = "Penis Item 1";
	auto secondItem = Items::BaseItem{"../res/penisItem2.png"};
	secondItem.label = "Penis Item 2";

	auto thirdItem = Items::BaseItem{"../res/japaneseItem.png"};
	thirdItem.label = "Japanese Item";

	inventory.addItem(3, Items::InventoryItem{firstItem, 5});
	inventory.addItem(22, Items::InventoryItem{secondItem, 2});
	inventory.addItem(44, Items::InventoryItem{thirdItem, 2});
	
	Game::ItemBar itemBar;

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		World::step(25);
		inventory.update();
		itemBar.update();
		World::camera.follow(World::activePlayer->pos);

		BeginMode2D(World::camera.cam);
			World::render();
		EndMode2D();
		inventory.renderInventory();
		itemBar.render();

		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			auto selectedTile = World::getSelectedTile(Math::MutableVec2{mousePos.x, mousePos.y});
			if(selectedTile != nullptr) {		
				World::activePlayer->selectedTile = selectedTile;
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}