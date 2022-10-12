#include "Game.h"

int main() {
	InitWindow(WindowUtils::WINDOW_WIDTH, WindowUtils::WINDOW_HEIGHT, "Terraria Clone");
	SetTargetFPS(WindowUtils::FPS_COUNT);

	World::camera = Game::Camera{};
	Game::MainMenu mainMenu;
	Game::mainMenuIsOpen = false;
	
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

	Game::MenuComponent newGame;
	newGame.label = "New Game";
	newGame.click = [] {
		Game::mainMenuIsOpen = false;
	};

	Game::MenuComponent loadGame;
	loadGame.label = "Load Game";
	loadGame.click = [] {
		std::cout << "Load Game!" << std::endl;
	};

	
	mainMenu.addComponent(newGame);
	mainMenu.addComponent(loadGame);
	
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		if(IsKeyPressed(KEY_Q)) {
			Game::mainMenuIsOpen = !Game::mainMenuIsOpen;
		}

		if(!Game::mainMenuIsOpen) {
			World::step(25);
			player.inventory.update();
			World::camera.follow(player.pos);

			BeginMode2D(World::camera.cam);
				World::render();
			EndMode2D();
			player.inventory.renderInventory();

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
					auto selectedTile = World::getSelectedTile(
						Math::MutableVec2{mousePos.x, mousePos.y}
					);
					if(selectedTile != nullptr) {		
						player.selectedTile = selectedTile;
					}
				}
			}
		} else {
			mainMenu.update();
			mainMenu.render();
		}

		EndDrawing();
	}

	CloseWindow();


	getchar();

	return 0;
}