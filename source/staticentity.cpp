#include "staticentity.h"

StaticEntity::StaticEntity() {
	boundingVolume = nullptr;
	body = nullptr;
	debugColor = SDL_Color(0, 255, 255, 255);
}

StaticEntity::StaticEntity(std::shared_ptr<Graphics> g) {
	graphics = g;
	boundingVolume = nullptr;
	body = nullptr;
	debugColor = SDL_Color(0, 255, 255, 255);
}

StaticEntity::StaticEntity(std::shared_ptr<Graphics> g, float w, float h, Vector2 startPos)
{
	graphics = g;
	boundingVolume = nullptr;
	SetWorldDimensions(b2Vec2(w, h));
	newDrawPosition = startPos;
	body = nullptr;
	debugColor = SDL_Color(0, 255, 255, 255);
}

StaticEntity::~StaticEntity()
{
	body = nullptr;
	boundingVolume = nullptr;

	std::cout << "Graphics pointer ref count: " << graphics.use_count() << std::endl;
	graphics = nullptr;
}

void StaticEntity::Draw(Vector2 cameraPosition) {
	Vector2 resultPosition;

	if (!currentAnimation)
		return;

	resultPosition = { newDrawPosition.x - cameraPosition.x, 
		newDrawPosition.y - cameraPosition.y };

	scaleCenter = vector2(currentAnimation->GetCellWidth() / 2.0f,
		currentAnimation->GetCellHeight() / 2.0f);

	currentSprite->Draw(currentSprite,
		resultPosition,
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
{/*
	worldPosition = body->GetPosition();
	newDrawPosition = { worldPosition.x, worldPosition.y };
	graphics->Vector2MetersToPixels(newDrawPosition);*/
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
