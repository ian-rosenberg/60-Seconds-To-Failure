#include "staticentity.h"

StaticEntity::StaticEntity() {
	boundingVolume = nullptr;
	graphics = nullptr;
	body = nullptr;
}

StaticEntity::StaticEntity(std::shared_ptr<Graphics> g) {
	graphics = g;
	boundingVolume = nullptr;
	body = nullptr;
}

StaticEntity::StaticEntity(std::shared_ptr<Graphics> g, float w, float h)
{
	graphics = g;
	boundingVolume = nullptr;
	SetWorldDimensions(b2Vec2(w, h));
	body = nullptr;
}

StaticEntity::~StaticEntity()
{
	body = nullptr;
	boundingVolume = nullptr;
	graphics = nullptr;
}

void StaticEntity::Draw() {
	if (!currentAnimation)
		return;

	scaleCenter = vector2(currentAnimation->GetCellWidth() / 2.0f,
		currentAnimation->GetCellHeight() / 2.0f);

	currentSprite->Draw(currentSprite,
		newDrawPosition,
		&scale,
		&scaleCenter,
		&rotation,
		flip,
		&color,
		currentAnimation->GetCurrentFrame(),
		currentAnimation->GetYOffset(),
		currentAnimation->GetCellWidth(),
		currentAnimation->GetCellHeight());
}

void StaticEntity::Think()
{
}

void StaticEntity::Update()
{
	worldPosition = body->GetPosition();
}

int StaticEntity::Touch(Entity* other)
{
	return 0;
}

void StaticEntity::Activate(Entity* activator)
{
}

int StaticEntity::Damage(int amount, Entity* source)
{
	return 0;
}

void StaticEntity::Die()
{

}

void StaticEntity::UpdateScreenPosition(double alpha)
{
	newDrawPosition.x = ((SCALED_WIDTH / 2.0f) + worldPosition.x) * MET_TO_PIX;
	newDrawPosition.y = ((SCALED_HEIGHT / 2.0f) + worldPosition.y) * MET_TO_PIX;
	debugRect.x = newDrawPosition.x - worldDimensions.x / 2 * MET_TO_PIX;
	debugRect.y = newDrawPosition.y - worldDimensions.y / 2 * MET_TO_PIX;
	/*std::cout << "Object: " << name << std::endl;
	std::cout << "World Pos: " << worldPosition.x << "," << worldPosition.y << std::endl;
	std::cout << "Screen Pos: " << newDrawPosition.x << "," << newDrawPosition.y << "\t\tDEBUG RECT: " << debugCircle.x << "," << debugCircle.y << std::endl;*/
}
