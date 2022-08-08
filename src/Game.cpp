#include "Game.h"

Math::ImmutableVec2::ImmutableVec2(const float newX, const float newY) : x(newX), y(newY) {}

float Math::ImmutableVec2::getX() const {
	return x;
}

float Math::ImmutableVec2::getY() const {
	return y;
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

// ==========

Math::MutableVec2::MutableVec2() {
	setX(0);
	setY(0);
	limit = 0;
}

Math::MutableVec2::MutableVec2(const float newX, const float newY) {
	setX(x);
	setY(y);
    limit = 0;
}

Math::MutableVec2::MutableVec2(const float newX, const float newY, const float newLimit) {
	limit = newLimit;
	setX(newX);
	setY(newY);
}

void Math::MutableVec2::setX(const float newX) {
	// if(hasLimit()) {
	// 	x = std::min(x, limit);
	// 	x = std::max(x, -limit);
	// } else {
		x = newX;
	// }
}

float Math::MutableVec2::getX() const {
	return x;
}

void Math::MutableVec2::setY(const float newY) {
	// if(hasLimit()) {
		// y = std::min(y, limit);
		// y = std::max(y, -limit);
	// } else {
		y = newY;
	// }
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

// ==========

Entity::BaseEntity::BaseEntity() {
    pos = Math::MutableVec2(0, 0);
    movementSpeed = 0;
    health = 0;
}

Entity::BaseEntity::BaseEntity(const Math::MutableVec2 posParam) {
    std::cout << "without24" << std::endl;
    pos = posParam;
    movementSpeed = 0;
    health = 0;
}

Entity::BaseEntity::BaseEntity(const Math::MutableVec2 posParam, float movementSpeedParam, float healthParam) {
    pos = posParam;
    movementSpeed = movementSpeedParam;
    health = healthParam;    
    std::cout << "yes" << std::endl;
}

void Entity::BaseEntity::updateMovementSpeed(const float newMovementSpeed) {
	movementSpeed = newMovementSpeed;
}

float Entity::BaseEntity::getMovementSpeed() {
	return movementSpeed;
}

void Entity::BaseEntity::updateHealth(const float newHealth) {
	health = newHealth;
}

float Entity::BaseEntity::getHealth() const {
	return health < 0 ? 0 : health;
}

void Entity::BaseEntity::follow(const Math::MutableVec2 position) {
	Math::MutableVec2 way(position.getX(), position.getY());
	way.sub(pos);
	way.normalize();
	way.mult(movementSpeed);

	Math::MutableVec2 push(way.getX(), way.getY(), movementSpeed);
	pos.add(push);
}

void Entity::BaseEntity::update() {
	move();
}

void Entity::BaseEntity::render() {
	DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
}

// ==========

Entity::Player::Player() : BaseEntity() {}
Entity::Player::Player(Math::MutableVec2 posParam) {
    std::cout << "without1" << std::endl;
    pos = posParam;
    updateMovementSpeed(0);
    updateHealth(0);
}
Entity::Player::Player(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam) {
    std::cout << "without2" << std::endl;
    std::cout << posParam.getX() << std::endl;
    pos = posParam;
    updateMovementSpeed(movementSpeedParam);
    updateHealth(healthParam);
}

void Entity::Player::move() {
	if(IsKeyDown(KEY_A)) {
		follow(Math::MutableVec2(pos.getX()-getMovementSpeed()*5, pos.getY()));
	}
	if(IsKeyDown(KEY_D)) {
		follow(Math::MutableVec2(pos.getX()+getMovementSpeed()*5, pos.getY()));
	}

	if(IsKeyDown(KEY_W)) {
		follow(Math::MutableVec2(pos.getX(), pos.getY()-getMovementSpeed()*5));
	}
	if(IsKeyDown(KEY_S)) {
		follow(Math::MutableVec2(pos.getX(), pos.getY()+getMovementSpeed()*5));
	}
}

void Entity::Player::render() {
    DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
    // DrawRectangle(pos.getX(), pos.getY(), 30, 30, WHITE);
}

// ==========