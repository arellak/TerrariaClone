#include "Game.h"


int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(Game::FPS_COUNT);
	
	World::createTile(Math::MutableVec2{200, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{400, 400}, Math::MutableVec2{200, 20}, BLACK);
	World::createTile(Math::MutableVec2{600, 420}, Math::MutableVec2{200, 20}, BLACK);


	World::createTile(Math::MutableVec2{600, 540}, Math::MutableVec2{200, 20}, BLACK);

	World::camera = Game::Camera{};

	World::createPlayer(Math::MutableVec2{250, 250}, 15);
	
	Game::Inventory inventory;
	inventory.addItem(0, Item::BaseItem{"../res/penisItem.png", 0.35f});
	inventory.addItem(5, Item::BaseItem{"../res/penisItem2.png", 0.35f});
	
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);

		World::step(25);
		inventory.update();
		World::camera.follow(World::activePlayer->pos);

		BeginMode2D(World::camera.cam);
			World::render();
		EndMode2D();
		inventory.render();

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