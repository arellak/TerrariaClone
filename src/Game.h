#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "raylib.h"

extern constexpr int WINDOW_WIDTH = 800;
extern constexpr int WINDOW_HEIGHT = 400;

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
			ImmutableVec2 div(ImmutableVec2 vec) const;
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

			bool hasLimit() const;

			float getLength() const;
			float getLengthIfOneIsZero(float divisor) const;
			void normalize();

			void add(MutableVec2 vec);
			void sub(MutableVec2 vec);
			float mult(MutableVec2 vec);
			void mult(float value);
			void div(MutableVec2 vec);

			std::string toString() const;
	};
}

namespace Game {
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
		private:
			float movementSpeed;
			float health;
		public:
			Texture2D tex;
			float mass;
			CollisionDirection collisionDirection;

			BaseEntity();
			explicit BaseEntity(Math::MutableVec2 posParam);
			BaseEntity(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
			Math::MutableVec2 pos;

			void updateMovementSpeed(float newMovementSpeed);
			float getMovementSpeed();

			void updateHealth(float newHealth);
			float getHealth() const;

			void follow(Math::MutableVec2 position);
			virtual void move() = 0;
			virtual void update();
			virtual void render();
			bool collides(WorldObjects::Tile toCompare);
	};

	class Player : public BaseEntity {
		public:
			Player();
			explicit Player(Math::MutableVec2 posParam);
			Player(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
			
			Game::Inventory inventory;
			void move() override;
			void render() override;
	};
}

namespace World {
	static std::vector<WorldObjects::Tile*> tiles;
	static std::vector<Entity::Player*> players;

	static void step(const float dt) {
		for(auto &entity : players) {
			float force = (entity->mass * 9.81f) / dt;
				
			Math::MutableVec2 updated{0, force};
			float length = updated.getLengthIfOneIsZero(dt) == 0 ? 1 : updated.getLengthIfOneIsZero(dt);
			updated.setX(updated.getX() / length);
			updated.setY(updated.getY() / length);

			for(auto &tile : tiles) {
				if(entity->collides(*tile)) {
					entity->updateMovementSpeed(0);
				}
			}

			entity->update();
			if(entity->getMovementSpeed() > 0) {
				entity->pos.add(updated);
			}
		}
	}

	static void render() {
		for(auto &tile : tiles) {
			DrawRectangle(tile->position.getX(), tile->position.getY(), tile->size.getX(), tile->size.getY(), tile->color);
		}
		for(auto &entity : players) {
			entity->render();
		}
	}
};


#endif