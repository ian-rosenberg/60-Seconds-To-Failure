#include "staticentity.h"

StaticEntity::StaticEntity() {
	boundingVolume = nullptr;
	renderer = nullptr;
	body = nullptr;
}

StaticEntity::StaticEntity(SDL_Renderer* ren) {
	renderer = ren;
	boundingVolume = nullptr;
	body = nullptr;
}

StaticEntity::StaticEntity(SDL_Renderer* ren, float w, float h)
{
	renderer = ren;
	boundingVolume = nullptr;
	SetWorldDimensions(b2Vec2(w, h));
	body = nullptr;
}

StaticEntity::~StaticEntity()
{
	body = nullptr;
	boundingVolume = nullptr;
	renderer = nullptr;
}

void StaticEntity::Draw() {
	if (!currentAnimation)
		return;

	scaleCenter = vector2(currentAnimation->GetCellWidth() / 2.0f,
		currentAnimation->GetCellHeight() / 2.0f);

	currentSprite->Draw(currentSprite,
		screenPosition,
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

void StaticEntity::UpdateScreenPosition()
{
	screenPosition.x = ((SCALED_WIDTH / 2.0f) + worldPosition.x) * MET_TO_PIX;
	screenPosition.y = ((SCALED_HEIGHT / 2.0f) + worldPosition.y) * MET_TO_PIX;
	debugRect.x = screenPosition.x - worldDimensions.x / 2 * MET_TO_PIX;
	debugRect.y = screenPosition.y - worldDimensions.y / 2 * MET_TO_PIX;
	/*std::cout << "Object: " << name << std::endl;
	std::cout << "World Pos: " << worldPosition.x << "," << worldPosition.y << std::endl;
	std::cout << "Screen Pos: " << screenPosition.x << "," << screenPosition.y << "\t\tDEBUG RECT: " << debugRect.x << "," << debugRect.y << std::endl;*/
}
