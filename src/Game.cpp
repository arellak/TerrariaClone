#include "Game.h"

Math::ImmutableVec2::ImmutableVec2(const float new_x, const float new_y) : x(new_x), y(new_y), limit(0) {}

float Math::ImmutableVec2::getX() const {
	return x;
}

float Math::ImmutableVec2::getY() const {
	return y;
}

bool Math::ImmutableVec2::hasLimit() const {
	return limit != 0;
}

float Math::ImmutableVec2::getLength() const {
	return std::abs(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
}

Math::ImmutableVec2 Math::ImmutableVec2::normalize() {
	float length = getLength() == 0 ? 1 : getLength();
	return Math::ImmutableVec2{(getX() / length), (getY() / length)};
}

Math::ImmutableVec2 Math::ImmutableVec2::add(const Math::ImmutableVec2 vec) const {
	return Math::ImmutableVec2{(getX() + vec.getX()), (getY() + vec.getY())};
}

Math::ImmutableVec2 Math::ImmutableVec2::sub(const Math::ImmutableVec2 vec) const {
	return Math::ImmutableVec2{(getX() - vec.getX()), (getY() - vec.getY())};
}

float Math::ImmutableVec2::mult(const Math::ImmutableVec2 vec) const {
	return (getX() * vec.getX()) + (getY() * vec.getY());
}

Math::ImmutableVec2 Math::ImmutableVec2::div(const Math::ImmutableVec2 vec) const {
	return Math::ImmutableVec2{(getX() / vec.getX()), (getY() / vec.getY())};
}

Math::MutableVec2::MutableVec2() : x(0), y(0), limit(0) {}

Math::MutableVec2::MutableVec2(const float new_x, const float new_y) : x(new_x), y(new_y), limit(0) {}

void Math::MutableVec2::setX(const float new_x) {
	x = new_x;
}

float Math::MutableVec2::getX() const {
	return x;
}

void Math::MutableVec2::setY(const float new_y) {
	y = new_y;
}

float Math::MutableVec2::getY() const {
	return y;
}

bool Math::MutableVec2::hasLimit() const {
	return limit != 0;
}

float Math::MutableVec2::getLength() const {
	return std::abs(std::sqrt(pow(getX(), 2) + pow(getY(), 2)));
}

void Math::MutableVec2::normalize() {
	auto length = getLength() == 0 ? 1 : getLength();
	setX(getX() / length);
	setY(getY() / length);
}

void Math::MutableVec2::add(MutableVec2 vec) {
	setX(getX() + vec.getX());
	setY(getY() + vec.getY());
}

void Math::MutableVec2::sub(MutableVec2 vec) {
	setX(getX() - vec.getX());
	setY(getY() - vec.getY());
}

void Math::MutableVec2::mult(float value) {
	setX(getX() * value);
	setY(getY() * value);
}

float Math::MutableVec2::mult(MutableVec2 vec) {
	return (getX() * vec.getX()) + (getY() * vec.getY());
}

void Math::MutableVec2::div(MutableVec2 vec) {
	setX(getX() / vec.getX());
	setY(getY() / vec.getY());
}

// ==============

Entity::BaseEntity::BaseEntity() 
	: movement_speed(0), health(10) {}

Entity::BaseEntity::BaseEntity(const Math::MutableVec2 pos_param) 
	: pos(pos_param), movement_speed(0), health(10) {}

Entity::BaseEntity::BaseEntity(const Math::MutableVec2 pos_param, float movement_speed_param, float health_param) 
	: pos(pos_param), movement_speed(movement_speed_param), health(health_param) {}

void Entity::BaseEntity::updateHealth(const float new_health) {
	health = new_health;
}

float Entity::BaseEntity::get_health() const {
	return health;
}

// ==============

void Entity::Player::move() {
	if(IsKeyPressed(KEY_A)) {

	}
	if(IsKeyPressed(KEY_D)) {

	}

	if(IsKeyPressed(KEY_W)) {

	}
	if(IsKeyPressed(KEY_S)) {

	}
}