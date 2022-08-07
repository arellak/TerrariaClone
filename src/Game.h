#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <iostream>
#include <cmath>

#include "raylib.h"

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
		void normalize();

		void add(MutableVec2 vec);
		void sub(MutableVec2 vec);
		float mult(MutableVec2 vec);
		void mult(float value);
		void div(MutableVec2 vec);
	};
}

namespace Item {

}

namespace Game {
	class Inventory {
	public:
		int size;
	};
};

namespace Entity {
	class BaseEntity {
	private:
		float movementSpeed;
		float health;

	public:
		Texture2D tex;

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
	};

	class Player : public BaseEntity {
	public:
		Player();
		Player(Math::MutableVec2 posParam);
		Player(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam);
		
		Game::Inventory inventory;
		void move() override;
		void render() override;
	};
}


#endif