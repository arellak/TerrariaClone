#include "Game.h"

std::vector<Texture2D> textures;
std::vector<WorldObjects::Tile> tiles;
// Entity::Player* activePlayer;
// Game::Camera camera;

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

namespace Items {
	BaseItem::BaseItem(int id) {
		_id = id;
		icon = textures.at(id);
		icon.width *= SCALE_FACTOR;
		icon.height *= SCALE_FACTOR;
	}
}

namespace Game {
	Camera::Camera() {
		cam = {0};
		cam.zoom = 1.0f;
	}

	void Camera::follow(const Math::MutableVec2 pos) {
		cam.target = Vector2 {pos.getX()+20, pos.getY()-30};
		cam.offset = Vector2 {WindowUtils::WINDOW_WIDTH/2, WindowUtils::WINDOW_HEIGHT/2};
		// cam.rotation = 0.0f;
		cam.zoom = 1.25f;
		// cam.zoom += ((float) GetMouseWheelMove() * 0.1f);
		// if(cam.zoom > 3.0f) cam.zoom = 3.0f;
		// if(cam.zoom < 0.5f) cam.zoom = 0.5f;
	}
}

namespace Entity {
	Inventory::Inventory() {
		float width = WindowUtils::WINDOW_WIDTH;
		float height = WindowUtils::WINDOW_HEIGHT;

		float inventoryWidth = width/2 - PADDING;
		float inventoryHeight = height/2;

		if(inventoryWindowSize.getX() == 0 || inventoryWindowSize.getY() == 0) {
			inventoryWindowSize = Math::MutableVec2{inventoryWidth, inventoryHeight};
		}

		if(inventoryWindowPosition.getX() == 0 && inventoryWindowPosition.getY() == 0) {
			inventoryWindowPosition = Math::MutableVec2{(width/2)-(inventoryWidth/2), (height/2)-(inventoryHeight/2)};
		}

		initSlotPositions();	
	}

	void Inventory::initSlotPositions() {
		int counter = 0;
		int rows = 1;
		int currentSlot = 0;

		// init positions for the normal storage slots
		for(int i = 1; i < MAX_SLOTS+1; i++) {
			counter++;
			float xPos = inventoryWindowPosition.getX() + ((SLOT_SIZE + PADDING)*counter);
			float yPos = inventoryWindowPosition.getY() + ((SLOT_SIZE + PADDING)*rows);

			if(xPos > ((inventoryWindowPosition.getX() + inventoryWindowSize.getX())) - (SLOT_SIZE + PADDING)) {
				xPos = inventoryWindowPosition.getX() + ((SLOT_SIZE + PADDING));
				counter = 1;
				rows++;
				yPos = inventoryWindowPosition.getY() + ((SLOT_SIZE + PADDING)*rows);
			}

			if(yPos > (inventoryWindowPosition.getY()+inventoryWindowSize.getY()) - (SLOT_SIZE + SLOT_SIZE + PADDING)) {
				break;
			}

			slotPositions.insert_or_assign(currentSlot, Math::MutableVec2{xPos, yPos});
			currentSlot++;
		}

		// init the positions of the "itembar" in the normal inventory
		float yPos = slotPositions.at(34).getY() + (SLOT_SIZE+PADDING*2);
		for(int i = 1; i < MAX_SLOTS/4; i++) {
			float xPos = inventoryWindowPosition.getX() + ((SLOT_SIZE + PADDING)*i);

			if(xPos > ((inventoryWindowPosition.getX() + inventoryWindowSize.getX())) - (SLOT_SIZE*2)) { 
				break;
			}
			
			slotPositions.insert_or_assign(currentSlot, Math::MutableVec2{xPos, yPos});
			currentSlot++;
		}
	}

	void Inventory::addItem(const int slot, Items::InventoryItem item) {
		if((slot < 0 || slot > MAX_SLOTS) || content.count(slot)) {
			// show some error message
			std::cout << "Slot is already used or slot is not in range\n";
			return;
		}
		content.insert_or_assign(slot, &item);
	}

	Items::InventoryItem* Inventory::getItemInHand() {
		if(!content.count(itemInHand)) return nullptr;
		return content.at(itemInHand);
	}

	void Inventory::update() {
		if(IsKeyPressed(KEY_E)) {
			if(isOpen) {
				isOpen = false;
				hoveredSlot = -1;
				selectedItemSlot = -1;
				hasItemOnCursor = false;
			} else {
				isOpen = true;
			}
		}

		int startSlot = 36;
		// update item on wheelmovement
		if(GetMouseWheelMove() == 1) {
			if(itemInHand - 1 < startSlot) {
				itemInHand = slotPositions.size()-2;
			} else {
				itemInHand--;
			}
		} else if(GetMouseWheelMove() == -1) {
			if(itemInHand + 1 > slotPositions.size()-2) {
				itemInHand = startSlot;
			} else {
				itemInHand++;
			}
		}

		if(!isOpen) return;

		hoverItem();
		dragItem();
	}

	void Inventory::hoverItem() {
		if(!mouseInInventory() || !isOpen) return;
		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		hoveredSlot = getSlot(Math::MutableVec2{mousePos.x, mousePos.y});
	}

	void Inventory::dragItem() {
		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		int slotAtMousePos = getSlot(Math::MutableVec2{mousePos.x, mousePos.y});
		
		// pick item up
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && content.count(slotAtMousePos)) {
			
		}

		// let the item "fall down" to its new place
		if(mouseInInventory() && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && hoveredSlot != -1) {
			if(slotAtMousePos != -1) {
				// check if there even is an item to drag at the selected slot
				// if not => do nothing, otherwise drag the item
				if(content.count(selectedItemSlot)) {
					if(content.count(slotAtMousePos)) return;
					content.insert_or_assign(slotAtMousePos, content.at(selectedItemSlot));
					content.erase(selectedItemSlot);
				}
			}

			hoveredSlot = -1;
			selectedItemSlot = -1;
		}
	}

	void Inventory::renderInventory() {
		renderItemBar();

		if(!isOpen) return;
		DrawRectangle(inventoryWindowPosition.getX(), inventoryWindowPosition.getY(), inventoryWindowSize.getX(), inventoryWindowSize.getY() , ColorAlpha(RAYWHITE, 1.0f));

		renderSlots();
		renderSlotContent();
		renderTooltip();
	}

	void Inventory::renderSlots() {
		for(int i = 0; i < slotPositions.size(); i++) {
			if(hoveredSlot == i) {
				renderSlot(i, INACTIVE_SLOT_COLOR, ACTIVE_SLOT_COLOR);
			} else {
				renderSlot(i, INACTIVE_SLOT_COLOR, ColorAlpha(BLACK, 0.3f));
			}
		}

		if(label.empty()) return;

		int fontSize = 25;
		int textWidth = MeasureText(label.c_str(), fontSize) + (fontSize/2);
		Math::MutableVec2 labelPosition{
			inventoryWindowPosition.getX() + (inventoryWindowSize.getX()/2) - (textWidth/2),
			inventoryWindowPosition.getY() + PADDING
		};

		DrawRectangle(labelPosition.getX()-5, labelPosition.getY(), textWidth, fontSize, BLACK);
		DrawText(label.c_str(), labelPosition.getX(), labelPosition.getY(), fontSize, LIGHTGRAY);
	}

	void Inventory::renderSlotContent() {
		for(auto item : content) { // loops over each item in the contents list
			auto currentItemSlot = item.first;
			auto pos = slotPositions.at(currentItemSlot);

			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				// set some values so other methods know if the player helds an item on the cursor
				if(hoveredSlot == currentItemSlot) {
					if(!hasItemOnCursor) {
						hasItemOnCursor = true;
						selectedItemSlot = currentItemSlot;
					}
				}

				if(currentItemSlot == hoveredSlot || hasItemOnCursor) {
					auto itemOnCursor = content.at(selectedItemSlot); // the if can also be true if the current iterations item isn't the same as the hovered item, so I get the item that is currently on the cursor and render that
					auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
					DrawTexture(itemOnCursor->content.icon, mousePos.x, mousePos.y, BLACK);
					
				} else {
					// if there is no item on the cursor and the current iterations isn't hovered over, i'll just render the item at its slot position
					DrawTexture(item.second->content.icon, pos.getX(), pos.getY(), BLACK);
					hasItemOnCursor = false;
					selectedItemSlot = -1;
				}
				
				// sadly i don't know a better way to handle this 'bug' yet
				// i have to iterate over each item a second time(except the outer loops item) so I can render all other items in case the player picks up an item
				// if i dont do this, all the other items in the inventory won't render as long as I have an item on the cursor
				for(auto otherItem : content) {
					auto otherPos = slotPositions.at(otherItem.first);
					if((pos.getX() == otherPos.getX() && pos.getY() == otherPos.getY() ) || selectedItemSlot == otherItem.first) continue;

					DrawTexture(otherItem.second->content.icon, otherPos.getX(), otherPos.getY(), BLACK);
				}

			} else {
				DrawTexture(item.second->content.icon, pos.getX(), pos.getY(), BLACK);
				hasItemOnCursor = false;
				selectedItemSlot = -1;
			}
		}
	}

	void Inventory::renderTooltip() {
		if(hoveredSlot == -1) return;
		if(!content.count(hoveredSlot)) return;
		auto hoveredItem = content.at(hoveredSlot);
		auto itemPos = slotPositions.at(hoveredSlot);

		if(hoveredItem->content.label.empty()) return;
		int fontSize = 20;
		int lines = 5;
		Color fontColor = LIGHTGRAY;

		float centerSlotPos = itemPos.getX() + (SLOT_SIZE/2 + PADDING/2);
		int textWidth = MeasureText(hoveredItem->content.label.c_str(), fontSize) + (fontSize/2);
		Math::MutableVec2 labelPosition{centerSlotPos - (textWidth/2), itemPos.getY() - fontSize*lines};

		DrawRectangle(labelPosition.getX()-5, labelPosition.getY(), textWidth, fontSize*5, DARKGRAY);
		DrawRectangleLinesEx(Rectangle{labelPosition.getX()-5, labelPosition.getY(), (float)textWidth, (float) fontSize*5}, 2, BLACK);

		// show label/name
		DrawText(hoveredItem->content.label.c_str(), labelPosition.getX(), labelPosition.getY(), fontSize, fontColor);

		// show item count
		DrawText(std::to_string(hoveredItem->amount).c_str(), labelPosition.getX(), labelPosition.getY()+fontSize, fontSize, fontColor);
	}

	void Inventory::renderItemBar() {
		int startSlot = 36;
		float startX = WindowUtils::WINDOW_WIDTH/2 - ((MAX_SLOTS/2) * (SLOT_SIZE+PADDING));
		float posY = WindowUtils::WINDOW_HEIGHT - (SLOT_SIZE+(PADDING*2));

		auto itemBarPos = Math::MutableVec2{startX, posY};
		auto itemBarSize = {(float) ((SLOT_SIZE+PADDING)*MAX_SLOTS)+PADDING, (float) SLOT_SIZE+(PADDING*2)};
		
		for(int i = startSlot; i < slotPositions.size()-1; i++) {
			float posX = itemBarPos.getX() + ((SLOT_SIZE + PADDING) * i);
			Rectangle slotRec{posX, posY, (float) SLOT_SIZE, (float) SLOT_SIZE};

			if(i == itemInHand) {
				DrawRectangleRec(slotRec, ColorAlpha(INACTIVE_SLOT_COLOR, 0.8f));
				DrawRectangleLinesEx(slotRec, 2, ACTIVE_SLOT_COLOR);
			} else {
				DrawRectangleRec(slotRec, ColorAlpha(INACTIVE_SLOT_COLOR, 0.8f));
				DrawRectangleLinesEx(slotRec, 2, BLACK);
			}
			
			if(content.count(i)) {
				DrawTexture(content.at(i)->content.icon, posX, posY, BLACK);
			}
		}
	}

	void Inventory::renderSlot(int slot, Color firstColor, Color secondColor) {
		auto position = slotPositions.at(slot);

		Rectangle slotRec{position.getX(), position.getY(), (float) SLOT_SIZE, (float) SLOT_SIZE};
		
		DrawRectangleRec(slotRec, firstColor);
		DrawRectangleLinesEx(slotRec, 2, secondColor);
	}

	bool Inventory::mouseInInventory() {
		if(!isOpen) return false; // double check just in case "mouseInInventory" gets called on its own
		auto mousePos = GetScreenToWorld2D(GetMousePosition(), World::camera.cam);
		
		bool inX = (mousePos.x > inventoryWindowPosition.getX() && mousePos.x < inventoryWindowPosition.getX() + inventoryWindowSize.getX());
		bool inY = (mousePos.y > inventoryWindowPosition.getY() && mousePos.y < inventoryWindowPosition.getY() + inventoryWindowSize.getY());

		return inX && inY;
	}

	int Inventory::getSlot(Math::MutableVec2 position) {
		for(auto slot : slotPositions) {
			auto slotPos = slot.second;
			bool inX = (position.getX() > slotPos.getX() && position.getX() < slotPos.getX() + SLOT_SIZE);
			bool inY = (position.getY() > slotPos.getY() && position.getY() < slotPos.getY() + SLOT_SIZE);

			if(inX && inY) {
				return slot.first;
			}
		}
		return -1;
	}
}

// ================================================

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

	void BaseEntity::moveTo(const Math::MutableVec2 position, const float stepSize) {
		Math::MutableVec2 difference(position.getX(), position.getY());
		difference.sub(pos);

		difference.normalize();
		difference.mult(stepSize);

		pos.add(difference);
	}

	void BaseEntity::update() {
		move();
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

	bool BaseEntity::collidesLeft(const WorldObjects::Tile tile) {
		float textureWidth = tile.texture.width != 32 ? tile.texture.width * 0.32f : tile.texture.width;
		float textureHeight = tile.texture.height != 32 ? tile.texture.height * 0.32f : tile.texture.height;

		bool inY = (pos.getY() <= tile.position.getY() + textureHeight) 
			&& (pos.getY() + tex.height >= tile.position.getY());
		
		float offset = getMovementSpeed() / 2;
		float posX = pos.getX() + offset;

		bool inX = (posX <= tile.position.getX() + textureWidth) 
			&& (posX >= (tile.position.getX() - textureWidth));
		return inY && inX;
	}

	bool BaseEntity::collidesRight(const WorldObjects::Tile tile) {
		float textureWidth = tile.texture.width != 32 ? tile.texture.width * 0.32f : tile.texture.width;
		float textureHeight = tile.texture.height != 32 ? tile.texture.height * 0.32f : tile.texture.height;

		bool inY = (pos.getY() <= tile.position.getY() + textureHeight) 
			&& (pos.getY() + tex.height >= tile.position.getY());
		
		float offset = getMovementSpeed() / 2;
		float posX = pos.getX() - offset*2;

		bool inX = (posX >= tile.position.getX()
			&&  posX <= tile.position.getX() + textureWidth);

		return inY && inX;
	}

	void BaseEntity::jump() {}

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

	void Player::move() {
		float startX = pos.getX();

		if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown((KEY_R))) {
			// reset pos if i get stuck
			pos.setY(pos.getY()+40);
		}

		if(onGround && IsKeyPressed(KEY_SPACE)) {
			jumping = true;
			onGround = false;
		}

		jump();

		if(IsKeyDown(KEY_A)) {
			Math::MutableVec2 position{pos.getX()-getMovementSpeed()*5, pos.getY()};
			for(auto &tile : tiles) {
				if(collidesRight(tile)) return;
			}

			if(direction != Game::Direction::WEST) {
				direction = Game::Direction::WEST;
			}
			moveTo(position, getMovementSpeed());
		}
		if(IsKeyDown(KEY_D)) {
			Math::MutableVec2 position{pos.getX()+getMovementSpeed()*5, pos.getY()};
			for(auto &tile : tiles) {
				if(collidesLeft(tile)) return;
			}

			if(direction != Game::Direction::EAST) {
				direction = Game::Direction::EAST;
			}
			moveTo(position, getMovementSpeed());
		}

		if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_W)) {
			const float MAX_HEIGHT = pos.getY()-(getMovementSpeed() * 5);
			
			Math::MutableVec2 position{pos.getX(), MAX_HEIGHT};
			for(auto &tile : tiles) {
				if(collidesBottom(tile)) {
					pos.setY(tile.position.getY()+getMovementSpeed() + 5);
					return;
				}
			}

			if(direction != Game::Direction::NORTH) {
				direction = Game::Direction::NORTH;
			}
			moveTo(position, getMovementSpeed());
		}

		if(pos.getX() == startX) {
			direction = Game::Direction::NONE;
		}
	}

	void Player::render() {
		DrawTexture(tex, pos.getX(), pos.getY(), WHITE);
	}

	void Player::jump() {
		static int jumpPixelCount = 0;
		if(jumping) {
			jumpPixelCount++;

			if(jumpPixelCount >= MAX_JUMP_HEIGHT) {
				if(direction == Game::Direction::EAST) {
					pos.updateX(WindowUtils::FPS_COUNT/2);
				} else if(direction == Game::Direction::WEST) {
					pos.updateX(-WindowUtils::FPS_COUNT/2);
				}

				pos.updateY(-WindowUtils::FPS_COUNT/4);
				
				jumpPixelCount = 0;
				jumping = false;
				return;
			}

			for(auto tile : tiles) {
				if(collidesBottom(tile)) {
					pos.setY(tile.position.getY()+getMovementSpeed() + 5);
					return;
				}
			}
			pos.updateY(-WindowUtils::FPS_COUNT/2);
		}
	}

	void Player::update() {
		move();
	}

}
// ==========