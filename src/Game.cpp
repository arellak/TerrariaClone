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

namespace Item {
	BaseItem::BaseItem(std::string iconPath, float scaleFactor) {
		icon = LoadTexture(iconPath.c_str());
		icon.width *= scaleFactor;
		icon.height *= scaleFactor;
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

	Inventory::Inventory() {
		
	}

	void Inventory::addItem(int slot, Item::BaseItem item) {
		content.insert(std::pair{slot, &item});
	}

	void Inventory::open() {
		isOpen = true;
	}

	void Inventory::close() {
		isOpen = false;
	}

	void Inventory::update() {
		if(IsKeyPressed(KEY_E)) {
			if(isOpen) {
				close();
			} else {
				open();
			}
		}
	}

	void Inventory::render() {
		if(!isOpen) return;
		float width = WindowUtils::WINDOW_WIDTH;
		float height = WindowUtils::WINDOW_HEIGHT;

		float inventoryWidth = width-((width/10)*2);
		float inventoryHeight = height-((height/5)*2);

		if(windowSize.getX() == 0 || windowSize.getY() == 0) {
			windowSize = Math::MutableVec2{inventoryWidth, inventoryHeight};
		}

		if(position.getX() == 0 && position.getY() == 0) {
			position = Math::MutableVec2{width/10, height/5};
		}

		DrawRectangle(position.getX(), position.getY(), windowSize.getX(), windowSize.getY() , WHITE);

		selectItem();
		if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && activeSlot != -1) {
			auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
			auto newSlot = getSlot(Math::MutableVec2{mousePos.x, mousePos.y});

			if(newSlot != -1) {
				// check if there even is an item to drag at the selected slot
				// if not => do nothing, otherwise drag the item
				if(content.count(selectedItemSlot)) {
					if(content.count(newSlot)) return;
					content.insert_or_assign(newSlot, content.at(selectedItemSlot));
					content.erase(selectedItemSlot);	
				}
			}

			activeSlot = -1;
			selectedItemSlot = -1;
		}

		renderSlots();
		renderSlotContent();
	}

	// TODO picked up items dont necessarily have the texture in "selected" mode compared to when they are in non-selected mode
	void Inventory::renderSlotContent() {
		for(auto item : content) {
			auto pos = slots.at(item.first);

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);

				// set some values so other methods know if the player helds an item on the cursor
				if(getSlot(Math::MutableVec2{mousePos.x, mousePos.y}) == item.first) {
					if(!hasItemOnCursor) {
						hasItemOnCursor = true;
						selectedItemSlot = activeSlot;
					}
				}

				if(item.first == activeSlot || hasItemOnCursor) {
					DrawTexture(item.second->icon, mousePos.x, mousePos.y, BLACK);
				} else {
					DrawTexture(item.second->icon, pos.getX(), pos.getY(), BLACK);
					hasItemOnCursor = false;
					selectedItemSlot = -1;
				}
				
				for(auto otherItem : content) {
					auto otherPos = slots.at(otherItem.first);
					if((pos.getX() == otherPos.getX() && pos.getY() == otherPos.getY() ) || selectedItemSlot == otherItem.first) continue;

					DrawTexture(otherItem.second->icon, otherPos.getX(), otherPos.getY(), BLACK);
				}

			} else {
				DrawTexture(item.second->icon, pos.getX(), pos.getY(), BLACK);
				hasItemOnCursor = false;
				selectedItemSlot = -1;
			}
		}
	}

	void Inventory::renderSlots() {
		int rows = windowSize.getX() / (slotSize+padding);
		int cols = windowSize.getY() / (slotSize+padding);
		int currentSlot = -1;

		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				float xPos = position.getX() + ((slotSize + padding) * i);
				float yPos = position.getY() + ((slotSize + padding) * j);

				if(xPos == position.getX() || yPos == position.getY()) continue;
				currentSlot++;
				Rectangle slot{xPos, yPos, (float) slotSize, (float) slotSize};
				if(activeSlot == currentSlot) {
					DrawRectangleLinesEx(slot, 2, selectedSlotColor);
				} else {
					DrawRectangleLinesEx(slot, 2, inactiveSlotColor);
				}
				slots.insert(std::pair{currentSlot, Math::MutableVec2{xPos, yPos}});
			}
		}

		if(label.empty()) return;
		DrawText(label.c_str(), position.getX() + (windowSize.getX()/3), position.getY() + padding, 20, BLACK);
	}

	void Inventory::selectItem() {
		if(!mouseInInventory() || !isOpen) return;
		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		activeSlot = getSlot(Math::MutableVec2{mousePos.x, mousePos.y});
		// for(auto slot : slots) {
		// 	auto slotPos = slot.second;
		// 	auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		// 	bool inX = (mousePos.x > slotPos.getX() && mousePos.x < slotPos.getX() + slotSize);
		// 	bool inY = (mousePos.y > slotPos.getY() && mousePos.y < slotPos.getY() + slotSize);

		// 	if(inX && inY) {
		// 		activeSlot = slot.first;
		// 	}
		// }
	}

	bool Inventory::mouseInInventory() {
		if(!isOpen) return false; // double check just in case "mouseInInventory" gets called on its own
		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		
		bool inX = (mousePos.x > position.getX() && mousePos.x < position.getX() + windowSize.getX());
		bool inY = (mousePos.y > position.getY() && mousePos.y < position.getY() + windowSize.getY());

		return inX && inY;
	}

	void Inventory::renderSlotAt(const int pos) {
		auto slot = slots.at(pos);
		//DrawRectangle(slot.getX(), slot.getY(), slotSize, slotSize, BLACK);
	}

	int Inventory::getSlot(Math::MutableVec2 position) {
		for(auto slot : slots) {
			auto slotPos = slot.second;
			bool inX = (position.getX() > slotPos.getX() && position.getX() < slotPos.getX() + slotSize);
			bool inY = (position.getY() > slotPos.getY() && position.getY() < slotPos.getY() + slotSize);

			if(inX && inY) {
				return slot.first;
			}
		}

		return -1;
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

	void BaseEntity::update(std::vector<WorldObjects::Tile*>& tiles) {
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
		float startX = pos.getX();

		if(onGround && IsKeyPressed(KEY_SPACE)) {
			jumping = true;
			onGround = false;
		}

		jump(tiles);

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

		if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_W)) {
			const float MAX_HEIGHT = pos.getY()-(getMovementSpeed() * 5);
			
			Math::MutableVec2 position{pos.getX(), MAX_HEIGHT};
			for(auto &tile : tiles) {
				if(collidesBottom(*tile)) {
					pos.setY(tile->position.getY()+getMovementSpeed() + 5);
					return;
				}
			}

			if(direction != Game::Direction::NORTH) {
				direction = Game::Direction::NORTH;
			}
			follow(position, getMovementSpeed());
		}

		if(pos.getX() == startX) {
			direction = Game::Direction::NONE;
		}
	}

	void Player::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

	void Player::jump(std::vector<WorldObjects::Tile*>& tiles) {
		static int jumpPixelCount = 0;
		if(jumping) {
			jumpPixelCount++;

			if(jumpPixelCount >= MAX_JUMP_HEIGHT) {
				if(direction == Game::Direction::EAST) {
					pos.updateX(Game::FPS_COUNT/3);
				} else if(direction == Game::Direction::WEST) {
					pos.updateX(-Game::FPS_COUNT/3);
				}

				pos.updateY(-Game::FPS_COUNT/4);
				
				jumpPixelCount = 0;
				jumping = false;
				return;
			}

			for(auto tile : tiles) {
				if(collidesBottom(*tile)) {
					pos.setY(tile->position.getY()+getMovementSpeed() + 5);
					return;
				}
			}
			pos.updateY(-Game::FPS_COUNT/2);
		}
	}

}
// ==========