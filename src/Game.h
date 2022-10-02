#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>

#include "raylib.h"

extern std::vector<Texture2D> textures;

namespace WindowUtils {
	static constexpr int WINDOW_WIDTH = 1280;
	static constexpr int WINDOW_HEIGHT = 720;
	static constexpr int FPS_COUNT = 30;
	static constexpr int TILE_SIZE = 32;
}

namespace Math {
	class ImmutableVec2 {
		private:
			const float x = 0;
			const float y = 0;
		public:
			ImmutableVec2(float mewX, float newY);

			float getX() const;
			float getY() const;

			float getLength() const;
			ImmutableVec2 normalize();

			ImmutableVec2 add(ImmutableVec2 vec) const;
			ImmutableVec2 sub(ImmutableVec2 vec) const;
			float mult(ImmutableVec2 vec) const;
	};

	class MutableVec2 {
		private:
			float x;
			float y;
		public:
			float limit;
			MutableVec2();
			MutableVec2(float newX, float newY);
			MutableVec2(float newX, float newY, float limit);

			void setX(float newY);
			float getX() const;
			void setY(float newY);
			float getY() const;

			void updateX(float value);
			void updateY(float value);

			bool hasLimit() const;

			float getLength() const;
			float getLengthIfOneIsZero(float divisor) const;
			void normalize();

			void add(MutableVec2 vec);
			void sub(MutableVec2 vec);
			float mult(MutableVec2 vec);
			void mult(float value);

			MutableVec2 operator+(const MutableVec2& value) const {
				return Math::MutableVec2{x + value.x, y + value.y};
			}
			
			MutableVec2 operator-(const MutableVec2& value) const {
				return Math::MutableVec2{x - value.x, y - value.y};
			}

			MutableVec2 operator*(const size_t value) const {
				return Math::MutableVec2{x * value, y * value};
			}

			size_t operator*(const MutableVec2& value) const {
				return x * value.x + y * value.y;
			}

			MutableVec2 operator%(const size_t value) const {
				return MutableVec2{(float) ((int) x % value), (float) ((int) y %value)};
			}

			bool operator==(const MutableVec2& value) {
				return ((int) x == (int) value.x) && ((int) y == (int) value.y);
			}

			std::string toString() const;
	};
}

namespace Items {

	class BaseItem {
		public:
			static constexpr float SCALE_FACTOR = 0.5f;

			int _id;
			std::string label;
			std::string _iconPath;
			Texture2D icon;

			bool repairable;
			bool destructable;

			BaseItem(int id);
	};

	class BlockItem : public BaseItem {
		
	};

	struct InventoryItem {
		BaseItem content;
		int amount;
	};

	static InventoryItem createItem(int texId, const std::string label, int amount) {
		BaseItem baseItem{texId};
		baseItem.label = label;
		return InventoryItem{baseItem, amount};
	}
}

namespace Game {
	static bool mainMenuIsOpen;
	enum class Direction {
		WEST,
		EAST,
		SOUTH,
		NORTH,
		NONE,
	};

	class Camera {
		public:
			Camera2D cam;
			Camera();

			void follow(Math::MutableVec2 pos);
	};

	class MenuComponent {
		public:
			Math::MutableVec2 dimensions;
			Math::MutableVec2 position;
			std::string label;

			void (*click)();

			void render();
			void onClick();
	};
	class MainMenu {
		public:
			MainMenu();
			MainMenu(Math::MutableVec2 dimensions, Math::MutableVec2 position);

			Math::MutableVec2 dimensions;
			Math::MutableVec2 position;
			std::vector<MenuComponent> components;
			void render();
			void update();
			void addComponent(MenuComponent component);
	};
};

namespace WorldObjects {
	struct Tile {
			Math::MutableVec2 position;
			Math::MutableVec2 size;
			Color color;
			Texture2D texture;
	};
}

namespace Entity {
	enum class CollisionDirection {
		LEFT,
		RIGHT,
		BOTTOM,
		TOP
	};

	class Inventory {
		public:
			const Color INACTIVE_SLOT_COLOR = ColorAlpha(Color{151, 151, 151}, 1.0f);
			const Color ACTIVE_SLOT_COLOR = ColorAlpha(RED, 0.7f);
			const int SLOT_SIZE = Items::BaseItem::SCALE_FACTOR * 100;
			const int PADDING = 8;
			const int MAX_SLOTS = 80;

			bool hasItemOnCursor = false;
			bool isOpen = false;

			int hoveredSlot = -1;
			int selectedItemSlot = -1;

			int itemInHand = 36;

			std::string label{"Inventory"};
			
			Math::MutableVec2 inventoryWindowPosition;
			Math::MutableVec2 inventoryWindowSize;

			std::map<int, Math::MutableVec2> slotPositions;
			std::map<int, Items::InventoryItem*> content;

			Inventory();

			void initSlotPositions();

			void update();
			void hoverItem();
			void dragItem();

			void renderInventory();
			void renderSlots();
			void renderSlotContent();
			void renderTooltip();
			void renderItemBar();

			void renderSlot(int slot, Color firstColor, Color secondColor);

			bool mouseInInventory();
			int getSlot(const Math::MutableVec2 position);
			void addItem(int slot, Items::InventoryItem item);

			Items::InventoryItem* getItemInHand();
	};

	class BaseEntity {
		private:
			float movementSpeed;
			float health;
		public:
			const float MAX_JUMP_HEIGHT = 10; //

			float mass;
			bool jumping;
			bool onGround;

			Texture2D tex;
			CollisionDirection collisionDirection;
			Game::Direction direction;

			BaseEntity();
			explicit BaseEntity(Math::MutableVec2 posParam);
			BaseEntity(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
			Math::MutableVec2 pos;

			void updateMovementSpeed(float newMovementSpeed);
			float getMovementSpeed();

			void updateHealth(float newHealth);
			float getHealth() const;

			void moveTo(Math::MutableVec2 position, float stepSize);
			virtual void move() = 0;
			virtual void update();
			virtual void render();
			virtual void jump();

			bool collidesBottom(WorldObjects::Tile compare);
			bool collidesTop(WorldObjects::Tile compare);
			bool collidesLeft(WorldObjects::Tile compare);
			bool collidesRight(WorldObjects::Tile compare);
	};

	class Player : public BaseEntity {
		public:
			WorldObjects::Tile* selectedTile;

			Player();
			explicit Player(Math::MutableVec2 posParam);
			Player(Math::MutableVec2 posParam, float movementSpeedParam);
			Player(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
			
			Inventory inventory;
			void move() override;
			void render() override;
			void jump() override;
			void update() override;
	};
}
extern std::vector<WorldObjects::Tile> tiles;
namespace World {
	static Entity::Player* activePlayer;
	static Game::Camera camera;
	
	static bool tileAlreadyExists(Math::MutableVec2 &pos) {
		for(auto &tile : tiles) {
			if(pos == tile.position) {
				return true;
			}
		}
		return false;
	}

	static Entity::Player createPlayer(const Math::MutableVec2 startPos, const float movementSpeed) {
		Entity::Player player{startPos, movementSpeed};
		activePlayer = &player;
		player.mass = 100;
		player.tex = textures.at(0);
		return player;
	}

	static WorldObjects::Tile* createTile(Math::MutableVec2 position, Math::MutableVec2 size, Color color) {
		WorldObjects::Tile tile;
		if(tileAlreadyExists(position)) return nullptr;
		tile = {position, size, color};
		tiles.push_back(tile);
		return &tiles.at(tiles.size()-1);
	}

	static WorldObjects::Tile* createTile(Math::MutableVec2 ms, int textureID) {
		WorldObjects::Tile tile;
		if(tileAlreadyExists(ms)) return nullptr;
		if(activePlayer->inventory.getItemInHand()->amount-1 < 0) return nullptr;
		activePlayer->inventory.getItemInHand()->amount--;

		auto texture = textures.at(textureID);
		tile.color = BLACK;
		tile.position = ms;
		tile.size = {(float) texture.width, (float) texture.height};
		tile.texture = texture;

		tiles.push_back(tile);

		return &tiles.at(tiles.size()-1);
	}

	static void removeTile(Math::MutableVec2 position) {
		int pos = -1;
		for(int i = 0; i < tiles.size(); i++) {
			auto tile = tiles.at(i);
			if(tile.position == position) {
				pos = i;
				break;
			}
		}
		if(pos == -1) return;
		tiles.erase(std::next(tiles.begin(), pos));
		activePlayer->selectedTile = nullptr;
	}

	static void step(const float dt) {
		activePlayer->update();

		// GRAVITY STUFF
		float force = (activePlayer->mass * 9.81f) / dt;
			
		Math::MutableVec2 gravityVec{0, force};
		float length = gravityVec.getLengthIfOneIsZero(dt) == 0 ? 1 : gravityVec.getLengthIfOneIsZero(dt);
		gravityVec.setX(gravityVec.getX() / length);
		gravityVec.setY(gravityVec.getY() / length);

		if(activePlayer->getMovementSpeed() > 0) {
			for(auto &tile : tiles) {
				if(activePlayer->collidesTop(tile)) {
					activePlayer->onGround = true;
					return;
				}
			}
			activePlayer->pos.add(gravityVec);
		}
	}

	static void render() {
		for(auto &tile : tiles) {
			if(tile.texture.width != 0) {
				DrawTextureEx(tile.texture, Vector2{tile.position.getX(), tile.position.getY()}, 0.0f, 0.32f, BLACK);
			} else {
				DrawRectangle(tile.position.getX(), tile.position.getY(), tile.size.getX(), tile.size.getY(), tile.color);
			}
		}
		activePlayer->render();
	}

	static WorldObjects::Tile* getNearestTile(Math::MutableVec2 vec) {
		WorldObjects::Tile* currentTile = nullptr;
		float currentLength = 0;

		for(auto tile : tiles) {
			Math::MutableVec2 temp;
			float xVal = 0;
			float yVal = 0;
			if(vec.getX() > tile.position.getX()) {
				xVal = tile.position.getX() - vec.getX();
			} else {
				xVal = vec.getX() - tile.position.getX();
			}

			if(vec.getY() > tile.position.getY()) {
				yVal = tile.position.getY() - vec.getY();
			} else {
				yVal = vec.getY() - tile.position.getY();
			}

			temp.setX(xVal);
			temp.setY(yVal);

			if(currentTile == nullptr) {
				currentLength = temp.getLength();
				currentTile = &tile;
				continue;
			}

			if(temp.getLength() < currentLength) {
				currentLength = temp.getLength();
				currentTile = &tile;
			}
		}
		return currentTile;
	}

	static WorldObjects::Tile* getSelectedTile(const Math::MutableVec2 mousePos) {
		for(auto &tile : tiles) {
			bool inX = (mousePos.getX() < tile.position.getX() + tile.size.getX()) && (mousePos.getX() > tile.position.getX());
			bool inY = (mousePos.getY() < tile.position.getY() + tile.size.getY()) && (mousePos.getY() > tile.position.getY());
			if(inX && inY) {
				return &tile;
			}
		}
		return nullptr;
	}
};

#endif