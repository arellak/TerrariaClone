#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

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
			ImmutableVec2(float newX, float newY);

			float getX() const;
			float getY() const;

			float getLength() const;
			ImmutableVec2 normalize();

			ImmutableVec2 add(ImmutableVec2 vec) const;
			ImmutableVec2 sub(ImmutableVec2 vec) const;
			float mult(ImmutableVec2 vec) const;
	};

	class MutableVec2 {
		public:
			float x;
			float y;
			float limit;

			MutableVec2();
			MutableVec2(float newX, float newY);
			MutableVec2(float newX, float newY, float limit);

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
			int size;
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

namespace Item {

}

namespace Entity {
	enum class CollisionDirection {
		LEFT,
		RIGHT,
		BOTTOM,
		TOP
	};

	class BaseEntity {
		public:
			const float MAX_JUMP_HEIGHT = 10; // i don't even know how to explain it

			float movementSpeed;
			float health;
			float mass;

			bool jumping;
			bool onGround;

			Game::Direction direction;
			Texture2D tex;
			CollisionDirection collisionDirection;
			Math::MutableVec2 pos;

			BaseEntity();
			explicit BaseEntity(Math::MutableVec2 posParam);
			BaseEntity(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
			
			void updateMovementSpeed(float newMovementSpeed);
			void updateHealth(float newHealth);

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
		gravityVec.x /= length; //(gravityVec.getX() / length);
		gravityVec.y /= length; //(gravityVec.getY() / length);

		if(activePlayer->movementSpeed > 0) {
			for(auto &tile : tiles) {
				if(activePlayer->collidesTop(*tile)) {
					activePlayer->onGround = true;
					return;
				}
			}

			if(activePlayer->onGround) activePlayer->onGround = false;
			activePlayer->pos.add(gravityVec);
		}
	}

	static void render() {
		for(auto &tile : tiles) {
			DrawRectangle(tile->position.x, tile->position.y, tile->size.x, tile->size.y, tile->color);
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
			if(vec.x > tile->position.x) {
				xVal = tile->position.x - vec.x;
			} else {
				xVal = vec.x - tile->position.x;
			}

			if(vec.y > tile->position.y) {
				yVal = tile->position.y - vec.y;
			} else {
				yVal = vec.y - tile->position.y;
			}

			temp.x = xVal;
			temp.y = yVal;

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

	static WorldObjects::Tile* getSelectedTile(const Camera2D cam) {
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			auto mousePos = GetScreenToWorld2D(GetMousePosition(), cam);
			for(auto tile : tiles) {
				bool inX = (mousePos.x < tile->position.x + tile->size.x) && (mousePos.x > tile->position.x);
				bool inY = (mousePos.y < tile->position.y + tile->size.y) && (mousePos.y > tile->position.y);
				if(inX && inY) {
					return tile;
				}
			}
		}
		return nullptr;
	}
};


#endif