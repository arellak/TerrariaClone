#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>

#include "raylib.h"

namespace WindowUtils {
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 400;
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

			std::string toString() const;
	};
}

namespace Item {
	enum class ItemType {
		WEAPON,
		TOOL,
		USABLE,
	};

	class BaseItem {
		public:
			int id;
			ItemType type;
			std::string label;
			Texture2D icon;

			bool repairable;
			bool destructable;

			BaseItem(std::string iconPath, float scaleFactor);
			
	};
}

namespace Game {
	static const int FPS_COUNT = 30;

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

	class Inventory {
		public:
			bool isOpen = false;
			std::string label{"Inventory"};
			Math::MutableVec2 position;
			Math::MutableVec2 windowSize;
			std::map<int, Math::MutableVec2> slots;
			std::map<int, Item::BaseItem*> content;

			const Color inactiveSlotColor = BLACK;
			const Color selectedSlotColor = RED;

			bool hasItemOnCursor = false;

			int activeSlot = -1;
			int selectedItemSlot = -1;
			
			const int slotSize = 35;
			const int padding = 8;
			
			Inventory();

			void open();
			void close();
			void update();
			void render();
			void renderSlots();
			void selectItem();
			void renderSlotAt(int pos);
			void renderSlotContent();


			bool mouseInInventory();
			int getSlot(Math::MutableVec2 position);
			void addItem(int slot, Item::BaseItem);
	};
};

namespace WorldObjects {
	struct Tile {
			Math::MutableVec2 position;
			Math::MutableVec2 size;
			Color color;
			// Texture2D texture;
	};
}

namespace Entity {
	enum class CollisionDirection {
		LEFT,
		RIGHT,
		BOTTOM,
		TOP
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

			void follow(Math::MutableVec2 position, float stepSize);
			virtual void move(std::vector<WorldObjects::Tile*>& tiles) = 0;
			virtual void update(std::vector<WorldObjects::Tile*>& tiles);
			virtual void render();
			virtual void jump(std::vector<WorldObjects::Tile*>& tiles);

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
			
			Game::Inventory inventory;
			void move(std::vector<WorldObjects::Tile*>& tiles) override;
			void render() override;
			void jump(std::vector<WorldObjects::Tile*>& tiles) override;
	};
}

namespace World {
	static std::vector<WorldObjects::Tile*> tiles;
	static Entity::Player* activePlayer;
	static Game::Camera camera;

	static Entity::Player createPlayer(const Math::MutableVec2 startPos, const float movementSpeed) {
		Entity::Player player{startPos, movementSpeed};
		activePlayer = &player;
		player.mass = 80;
		player.tex = LoadTexture("../res/Player.png");
		return player;
	}

	static WorldObjects::Tile createTile(Math::MutableVec2 position, Math::MutableVec2 size, Color color) {
		WorldObjects::Tile tile{position, size, color};
		tiles.push_back(&tile);
		return tile;
	}

	static void step(const float dt) {
		activePlayer->update(tiles);

		// GRAVITY STUFF
		float force = (activePlayer->mass * 9.81f) / dt;
			
		Math::MutableVec2 gravityVec{0, force};
		float length = gravityVec.getLengthIfOneIsZero(dt) == 0 ? 1 : gravityVec.getLengthIfOneIsZero(dt);
		gravityVec.setX(gravityVec.getX() / length);
		gravityVec.setY(gravityVec.getY() / length);

		if(activePlayer->getMovementSpeed() > 0) {
			for(auto &tile : tiles) {
				if(activePlayer->collidesTop(*tile)) {
					activePlayer->onGround = true;
					return;
				}
			}
			activePlayer->pos.add(gravityVec);
		}
	}

	static void render() {
		for(auto &tile : tiles) {
			DrawRectangle(tile->position.getX(), tile->position.getY(), tile->size.getX(), tile->size.getY(), tile->color);
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
			if(vec.getX() > tile->position.getX()) {
				xVal = tile->position.getX() - vec.getX();
			} else {
				xVal = vec.getX() - tile->position.getX();
			}

			if(vec.getY() > tile->position.getY()) {
				yVal = tile->position.getY() - vec.getY();
			} else {
				yVal = vec.getY() - tile->position.getY();
			}

			temp.setX(xVal);
			temp.setY(yVal);

			if(currentTile == nullptr) {
				currentLength = temp.getLength();
				currentTile = tile;
				continue;
			}

			if(temp.getLength() < currentLength) {
				currentLength = temp.getLength();
				currentTile = tile;
			}
		}
		return currentTile;
	}

	static WorldObjects::Tile* getSelectedTile(const Math::MutableVec2 mousePos) {
		for(auto tile : tiles) {
			bool inX = (mousePos.getX() < tile->position.getX() + tile->size.getX()) && (mousePos.getX() > tile->position.getX());
			bool inY = (mousePos.getY() < tile->position.getY() + tile->size.getY()) && (mousePos.getY() > tile->position.getY());
			if(inX && inY) {
				return tile;
			}
		}
		return nullptr;
	}
};


#endif