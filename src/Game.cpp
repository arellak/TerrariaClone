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
		return ImmutableVec2{(x / length), (y / length)};
	}

	ImmutableVec2 ImmutableVec2::add(const ImmutableVec2 vec) const {
		return ImmutableVec2{(x + vec.x), (y + vec.y)};
	}

	ImmutableVec2 ImmutableVec2::sub(const ImmutableVec2 vec) const {
		return ImmutableVec2{(x - vec.x), (y - vec.y)};
	}

	float ImmutableVec2::mult(const ImmutableVec2 vec) const {
		return (x * vec.x) + (y * vec.y);
	}

	// ==========

	MutableVec2::MutableVec2() {
		x = 0;
		y = 0;
		limit = 0;
	}

	MutableVec2::MutableVec2(const float newX, const float newY) {
		x = newX;
		y = newY;
		limit = 0;
	}

	MutableVec2::MutableVec2(const float newX, const float newY, const float newLimit) {
		limit = newLimit;
		x = newX;
		y = newY;
	}

	bool MutableVec2::hasLimit() const {
		return limit != 0;
	}

	float MutableVec2::getLength() const {
		return std::sqrt(pow(x, 2) + pow(y, 2));
	}

	float MutableVec2::getLengthIfOneIsZero(const float divisor) const {
		float x = 0;
		float y = 0;

		if(x == 0) {
			x = 5;
			y /= divisor;
		} else if(y == 0) {
			x /= divisor;
			y = 5;
		}

		return std::sqrt(pow(x, 2) + pow(y, 2));	
	}

	void MutableVec2::normalize() {		
		auto length = getLength() == 0 ? 1 : getLength();
		x /= length;
		y /= length;
	}

	void MutableVec2::add(MutableVec2 vec) {
		x += vec.x;
		y += vec.y;
	}

	void MutableVec2::sub(MutableVec2 vec) {
		x -= vec.x;
		y -= vec.y;
	}

	void MutableVec2::mult(float value) {
		x *= value;
		y *= value;
	}

	float MutableVec2::mult(MutableVec2 vec) {
		return (x * vec.x) + (y * vec.y);
	}

	std::string MutableVec2::toString() const {
		std::string str;
		str.append("[x:{").append(std::to_string(x))
		.append("}, y:{").append(std::to_string(y)).append("}, length:{")
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
		cam.target = Vector2 {pos.x + 20, pos.y + 20};
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

	void BaseEntity::updateHealth(const float newHealth) {
		health = newHealth;
	}

	void BaseEntity::follow(const Math::MutableVec2 position, const float stepSize) {
		Math::MutableVec2 difference(position.x, position.y);
		difference.sub(pos);

		difference.normalize();
		difference.mult(stepSize);

		pos.add(difference);
	}

	void BaseEntity::update(std::vector<WorldObjects::Tile*>& tiles) {
		move(tiles);
	}

	void BaseEntity::render() {
		DrawTexture(tex, pos.x, pos.y, WHITE);
	}

	bool BaseEntity::collidesBottom(const WorldObjects::Tile compare) {
		bool inX = (pos.x <= compare.position.x + compare.size.x) 
			&& (pos.x + tex.width >= compare.position.x);

		float offset = movementSpeed / 2;
		float posY = pos.y - offset;
		return inX && (posY > compare.position.y) && (posY < compare.position.y + compare.size.y);
	}

	bool BaseEntity::collidesTop(const WorldObjects::Tile compare) {
		bool inX = (pos.x <= compare.position.x + compare.size.x) 
			&& (pos.x + tex.width >= compare.position.x);

		float offset = movementSpeed / 2;
		float posY = pos.y + tex.height + offset;		
		return inX && (posY < compare.position.y + compare.size.y) && (posY > compare.position.y);
	}

	bool BaseEntity::collidesLeft(const WorldObjects::Tile compare) {
		bool inY = (pos.y <= compare.position.y + compare.size.y) 
			&& (pos.y + tex.height >= compare.position.y);
		
		float offset = movementSpeed / 2;
		float posX = pos.x + tex.width + offset;
		return inY && (posX < compare.position.x + compare.size.x) && (posX > compare.position.x);
	}

	bool BaseEntity::collidesRight(const WorldObjects::Tile compare) {
		bool inY = (pos.y <= compare.position.y + compare.size.y) 
			&& (pos.y + tex.height >= compare.position.y);
		
		float offset = movementSpeed / 2;
		float posX = pos.x - offset;
		return inY && (posX > compare.position.x) && (posX < compare.position.x + compare.size.x);
	}

	void BaseEntity::jump(std::vector<WorldObjects::Tile*>& tiles) {

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

	void Player::move(std::vector<WorldObjects::Tile*>& tiles) {
		float startX = pos.x;

		if(onGround && IsKeyPressed(KEY_SPACE)) {
			jumping = true;
			onGround = false;
		}

		jump(tiles);

		if(IsKeyDown(KEY_A)) {
			Math::MutableVec2 position{pos.x-movementSpeed*5, pos.y};
			for(auto &tile : tiles) {
				if(collidesRight(*tile)) return;
			}

			if(direction != Game::Direction::WEST) {
				direction = Game::Direction::WEST;
			}
			follow(position, movementSpeed);
		}
		if(IsKeyDown(KEY_D)) {
			Math::MutableVec2 position{pos.x+movementSpeed*5, pos.y};
			for(auto &tile : tiles) {
				if(collidesLeft(*tile)) return;
			}

			if(direction != Game::Direction::EAST) {
				direction = Game::Direction::EAST;
			}
			follow(position, movementSpeed);
		}

		if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_W)) {
			const float MAX_HEIGHT = pos.y-(movementSpeed * 5);
			
			Math::MutableVec2 position{pos.x, MAX_HEIGHT};
			for(auto &tile : tiles) {
				if(collidesBottom(*tile)) {
					pos.y = tile->position.y + movementSpeed * 5;
					return;
				}
			}

			if(direction != Game::Direction::NORTH) {
				direction = Game::Direction::NORTH;
			}
			follow(position, movementSpeed);
		}

		if(pos.x == startX) {
			direction = Game::Direction::NONE;
		}
	}

	void Player::render() {
		DrawTexture(tex, pos.x, pos.y, WHITE);
	}

	void Player::jump(std::vector<WorldObjects::Tile*>& tiles) {
		static int jumpPixelCount = 0;
		if(jumping) {
			jumpPixelCount++;

			if(jumpPixelCount >= MAX_JUMP_HEIGHT) {
				if(direction == Game::Direction::EAST) {
					pos.x += Game::FPS_COUNT / 3;
				} else if(direction == Game::Direction::WEST) {
					pos.x -= Game::FPS_COUNT / 3;
				}

				pos.y -= Game::FPS_COUNT/4;
				
				jumpPixelCount = 0;
				jumping = false;
				return;
			}

			for(auto tile : tiles) {
				if(collidesBottom(*tile)) {
					pos.y = tile->position.y+movementSpeed + 5;
					return;
				}
			}
			pos.y -= Game::FPS_COUNT/2;
		}
	}

}
// ==========