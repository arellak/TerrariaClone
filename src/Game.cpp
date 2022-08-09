#include "Game.h"

namespace Math {
	ImmutableVec2::ImmutableVec2(const float newX, const float newY) : x(newX), y(newY) {}

	float ImmutableVec2::getX() const {
		return x;
	}

	float ImmutableVec2::getY() const {
		return y;
	}

	float ImmutableVec2::getLength() const {
		return std::abs(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
	}

	ImmutableVec2 ImmutableVec2::normalize() {
		float length = getLength() == 0 ? 1 : getLength();
		return ImmutableVec2{(getX() / length), (getY() / length)};
	}

	ImmutableVec2 ImmutableVec2::add(const ImmutableVec2 vec) const {
		return ImmutableVec2{(getX() + vec.getX()), (getY() + vec.getY())};
	}

	ImmutableVec2 ImmutableVec2::sub(const ImmutableVec2 vec) const {
		return ImmutableVec2{(getX() - vec.getX()), (getY() - vec.getY())};
	}

	float ImmutableVec2::mult(const ImmutableVec2 vec) const {
		return (getX() * vec.getX()) + (getY() * vec.getY());
	}

	ImmutableVec2 ImmutableVec2::div(const ImmutableVec2 vec) const {
		return ImmutableVec2{(getX() / vec.getX()), (getY() / vec.getY())};
	}

	// ==========

	MutableVec2::MutableVec2() {
		setX(0);
		setY(0);
		limit = 0;
	}

	MutableVec2::MutableVec2(const float newX, const float newY) {
		setX(newX);
		setY(newY);
		limit = 0;
	}

	MutableVec2::MutableVec2(const float newX, const float newY, const float newLimit) {
		limit = newLimit;
		setX(newX);
		setY(newY);
	}

	void MutableVec2::setX(const float newX) {
		x = newX;
	}

	float MutableVec2::getX() const {
		return x;
	}

	void MutableVec2::setY(const float newY) {
		y = newY;
	}

	float MutableVec2::getY() const {
		return y;
	}

	bool MutableVec2::hasLimit() const {
		return limit != 0;
	}

	float MutableVec2::getLength() const {
		return std::abs(std::sqrt(pow(getX(), 2) + pow(getY(), 2)));
	}

	void MutableVec2::normalize() {
		auto length = getLength() == 0 ? 1 : getLength();
		setX(getX() / length);
		setY(getY() / length);
	}

	void MutableVec2::add(MutableVec2 vec) {
		setX(getX() + vec.getX());
		setY(getY() + vec.getY());
	}

	void MutableVec2::sub(MutableVec2 vec) {
		setX(getX() - vec.getX());
		setY(getY() - vec.getY());
	}

	void MutableVec2::mult(float value) {
		setX(getX() * value);
		setY(getY() * value);
	}

	float MutableVec2::mult(MutableVec2 vec) {
		return (getX() * vec.getX()) + (getY() * vec.getY());
	}

	void MutableVec2::div(MutableVec2 vec) {
		setX(getX() / vec.getX());
		setY(getY() / vec.getY());
	}

	std::string MutableVec2::toString() const {
		std::string str;
		str.append("[x:{").append(std::to_string(getX()))
		.append("}, y:{").append(std::to_string(getY())).append("}, length:{")
		.append(std::to_string(getLength())).append("}]");
		return str;
	}


}

namespace Game {
	Camera::Camera() {
		cam = {0};
		cam.zoom = 1.0f;
	}

	void Camera::follow(const Math::MutableVec2 pos) {
		cam.target = Vector2 {pos.getX() + 20, pos.getY() + 20};
		cam.offset = Vector2 {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
		cam.rotation = 0.0f;
		cam.zoom += ((float) GetMouseWheelMove() * 0.1f);
		if(cam.zoom > 3.0f) cam.zoom = 3.0f;
		if(cam.zoom < 0.5f) cam.zoom = 0.5f;
	}
}

// ==========
namespace Entity {
	BaseEntity::BaseEntity() {
		pos = Math::MutableVec2(0, 0);
		movementSpeed = 0;
		health = 0;
	}

	BaseEntity::BaseEntity(const Math::MutableVec2 posParam) {
		pos = posParam;
		movementSpeed = 0;
		health = 0;
	}

	BaseEntity::BaseEntity(const Math::MutableVec2 posParam, float movementSpeedParam, float healthParam) {
		pos = posParam;
		movementSpeed = movementSpeedParam;
		health = healthParam;    
	}

	void BaseEntity::updateMovementSpeed(const float newMovementSpeed) {
		movementSpeed = newMovementSpeed;
	}

	float BaseEntity::getMovementSpeed() {
		return movementSpeed;
	}

	void BaseEntity::updateHealth(const float newHealth) {
		health = newHealth;
	}

	float BaseEntity::getHealth() const {
		return health < 0 ? 0 : health;
	}

	void BaseEntity::follow(const Math::MutableVec2 position) {
		// std::cout << position.toString() << std::endl;
		Math::MutableVec2 way(position.getX(), position.getY());
		way.sub(pos);
		
		way.normalize();
		way.mult(movementSpeed);

		Math::MutableVec2 push(way.getX(), way.getY(), movementSpeed);
		pos.add(push);
	}

	void BaseEntity::update() {
		move();
	}

	void BaseEntity::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

	// ==========

	Player::Player() : BaseEntity() {}
	Player::Player(Math::MutableVec2 posParam) {
		pos = posParam;
		updateMovementSpeed(0);
		updateHealth(0);
	}
	Player::Player(Math::MutableVec2 posParam, float movementSpeedParam, float healthParam) {
		pos = posParam;
		updateMovementSpeed(movementSpeedParam);
		updateHealth(healthParam);
	}

	void Player::move() {
		if(IsKeyDown(KEY_A)) {
			Math::MutableVec2 position{pos.getX()-getMovementSpeed()*5, pos.getY()};
			follow(position);
		}
		if(IsKeyDown(KEY_D)) {
			Math::MutableVec2 position{pos.getX()+getMovementSpeed()*5, pos.getY()};
			follow(position);
		}

		if(IsKeyDown(KEY_W)) {
			Math::MutableVec2 position{pos.getX(), pos.getY()-getMovementSpeed()*5};
			follow(position);
		}
		if(IsKeyDown(KEY_S)) {
			Math::MutableVec2 position{pos.getX(), pos.getY()+getMovementSpeed()*5};
			follow(position);
		}
	}

	void Player::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

}
// ==========