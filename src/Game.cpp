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

	void MutableVec2::updateX(const float value) {
		x += value;
	}

	void MutableVec2::updateY(const float value) {
		y += value;
	}

	bool MutableVec2::hasLimit() const {
		return limit != 0;
	}

	float MutableVec2::getLength() const {
		return std::sqrt(pow(getX(), 2) + pow(getY(), 2));
	}

	float MutableVec2::getLengthIfOneIsZero(const float divisor) const {
		float x = 0;
		float y = 0;

		if(getX() == 0) {
			x = 5;
			y = getY() / divisor;
		} else if(getY() == 0) {
			x = getX() / divisor;
			y = 5;
		}

		return std::sqrt(pow(x, 2) + pow(y, 2));	
	}

	void MutableVec2::normalize() {		
		auto length = getLength() == 0 ? 1 : getLength();
		setX(getX() / length);
		setY(getY() / length);
	}

	void MutableVec2::add(MutableVec2 vec) {
		updateX(vec.getX());
		updateY(vec.getY());
	}

	void MutableVec2::sub(MutableVec2 vec) {
		updateX(-vec.getX());
		updateY(-vec.getY());
	}

	void MutableVec2::mult(float value) {
		setX(getX() * value);
		setY(getY() * value);
	}

	float MutableVec2::mult(MutableVec2 vec) {
		return (getX() * vec.getX()) + (getY() * vec.getY());
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
		cam.offset = Vector2 {WindowUtils::WINDOW_WIDTH/2, WindowUtils::WINDOW_HEIGHT/2};
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
		direction = Game::Direction::NONE;
	}

	BaseEntity::BaseEntity(const Math::MutableVec2 posParam) {
		pos = posParam;
		movementSpeed = 0;
		health = 0;
		direction = Game::Direction::NONE;
	}

	BaseEntity::BaseEntity(const Math::MutableVec2 posParam, float movementSpeedParam, float healthParam) {
		pos = posParam;
		movementSpeed = movementSpeedParam;
		health = healthParam;
		direction = Game::Direction::NONE;
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

	void BaseEntity::follow(const Math::MutableVec2 position, const float stepSize) {
		Math::MutableVec2 difference(position.getX(), position.getY());
		difference.sub(pos);

		difference.normalize();
		difference.mult(stepSize);

		pos.add(difference);
	}

	void BaseEntity::update(const std::vector<WorldObjects::Tile*> tiles) {
		move(tiles);
	}

	void BaseEntity::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

	bool BaseEntity::collidesBottom(const WorldObjects::Tile compare) {
		bool inX = (pos.getX() <= compare.position.getX() + compare.size.getX()) 
			&& (pos.getX() + tex.width >= compare.position.getX());

		float offset = getMovementSpeed() / 2;
		float posY = pos.getY() - offset;
		return inX && (posY > compare.position.getY()) && (posY < compare.position.getY() + compare.size.getY());
	}

	bool BaseEntity::collidesTop(const WorldObjects::Tile compare) {
		bool inX = (pos.getX() <= compare.position.getX() + compare.size.getX()) 
			&& (pos.getX() + tex.width >= compare.position.getX());

		float offset = getMovementSpeed() / 2;
		float posY = pos.getY() + tex.height + offset;		
		return inX && (posY < compare.position.getY() + compare.size.getY()) && (posY > compare.position.getY());
	}

	bool BaseEntity::collidesLeft(const WorldObjects::Tile compare) {
		bool inY = (pos.getY() <= compare.position.getY() + compare.size.getY()) 
			&& (pos.getY() + tex.height >= compare.position.getY());
		
		float offset = getMovementSpeed() / 2;
		float posX = pos.getX() + tex.width + offset;
		return inY && (posX < compare.position.getX() + compare.size.getX()) && (posX > compare.position.getX());
	}

	bool BaseEntity::collidesRight(const WorldObjects::Tile compare) {
		bool inY = (pos.getY() <= compare.position.getY() + compare.size.getY()) 
			&& (pos.getY() + tex.height >= compare.position.getY());
		
		float offset = getMovementSpeed() / 2;
		float posX = pos.getX() - offset;
		return inY && (posX > compare.position.getX()) && (posX < compare.position.getX() + compare.size.getX());
	}

	// ==========

	Player::Player() : BaseEntity() {
		direction = Game::Direction::NONE;
	}
	Player::Player(const Math::MutableVec2 posParam) {
		pos = posParam;
		updateMovementSpeed(0);
		updateHealth(0);
		direction = Game::Direction::NONE;
	}
	Player::Player(const Math::MutableVec2 posParam, const float movementSpeedParam) {
		pos = posParam;
		updateMovementSpeed(movementSpeedParam);
		direction = Game::Direction::NONE;
	}
	Player::Player(const Math::MutableVec2 posParam, const float movementSpeedParam, const float healthParam) {
		pos = posParam;
		updateMovementSpeed(movementSpeedParam);
		updateHealth(healthParam);
		direction = Game::Direction::NONE;
	}

	void Player::move(const std::vector<WorldObjects::Tile*> tiles) {
		static int jumpPixelCount = 0;

		float startX = pos.getX();

		if(onGround && IsKeyPressed(KEY_SPACE)) {
			jumping = true;
			onGround = false;
		}

		if(jumping) {
			jumpPixelCount++;

			if(jumpPixelCount >= MAX_JUMP_HEIGHT) {
				if(direction == Game::Direction::EAST) {
					pos.updateX(Game::FPS_COUNT/2);
				} else if(direction == Game::Direction::WEST) {
					pos.updateX(-Game::FPS_COUNT/2);
				}

				pos.updateY(-Game::FPS_COUNT/5);
				jumpPixelCount = 0;
				jumping = false;
				return;
			}

			pos.updateY(-Game::FPS_COUNT/2);
		}

		if(IsKeyDown(KEY_A)) {
			Math::MutableVec2 position{pos.getX()-getMovementSpeed()*5, pos.getY()};
			for(auto &tile : tiles) {
				if(collidesRight(*tile)) return;
			}

			if(direction != Game::Direction::WEST) {
				direction = Game::Direction::WEST;
			}
			follow(position, getMovementSpeed());
		}
		if(IsKeyDown(KEY_D)) {
			Math::MutableVec2 position{pos.getX()+getMovementSpeed()*5, pos.getY()};
			for(auto &tile : tiles) {
				if(collidesLeft(*tile)) return;
			}

			if(direction != Game::Direction::EAST) {
				direction = Game::Direction::EAST;
			}
			follow(position, getMovementSpeed());
		}

		if(IsKeyDown(KEY_W)) {
			const float MAX_HEIGHT = pos.getY()-(getMovementSpeed() * 5);
			
			Math::MutableVec2 position{pos.getX(), MAX_HEIGHT};
			for(auto &tile : tiles) {
				if(collidesBottom(*tile)) return;
			}

			if(direction != Game::Direction::NORTH) {
				direction = Game::Direction::NORTH;
			}
			follow(position, getMovementSpeed());
		}

		// if(IsKeyDown(KEY_S)) {
		// 	Math::MutableVec2 position{pos.getX(), pos.getY()+getMovementSpeed()*5};
		// 	for(auto &tile : tiles) {
		// 		if(collidesTop(*tile)) return;
				
		// 	}

		// 	follow(position);
		// }

		if(pos.getX() == startX) {
			direction = Game::Direction::NONE;
		}

	}

	void Player::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

}
// ==========