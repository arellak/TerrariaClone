//
// Created by nimbl on 06/08/2022.
//

#ifndef TERRARIAKLON_GAME_H
#define TERRARIAKLON_GAME_H

#include <hash_map>

namespace Game {

	class Inventory {
	public:
		int size;

	};

};

class BaseEntity {
public:
	float walkingSpeed;
	float health;

	void updateHealth(const float new_health);

};

class Player : public BaseEntity {
public:
	Inventory inventory;
};


#endif //TERRARIAKLON_GAME_H
