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
		ImmutableVec2(float new_x, float new_y);

		float limit;

		float getX() const;
		float getY() const;

		bool hasLimit() const;

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
		MutableVec2(float new_x, float new_y);

		void setX(float new_y);
		float getX() const;
		void setY(float new_y);
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
		float movement_speed;
		float health;

	public:
		BaseEntity();
		explicit BaseEntity(Math::MutableVec2 pos_param);
		BaseEntity(Math::MutableVec2 pos_param, float movement_speed_param, float health_param);
		Math::MutableVec2 pos;

		void updateHealth(float new_health);
		float get_health() const;

		virtual void move() = 0;

	};

	class Player : public BaseEntity {
	public:
		Game::Inventory inventory;
		void move() override;
	};
}


#endif