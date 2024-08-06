#include "staticentity.h"

StaticEntity::StaticEntity() {
	boundingVolume = nullptr;
	body = nullptr;
	debugColor = SDL_Color(0, 255, 255, 255);
}

StaticEntity::StaticEntity(float w, float h, Vector2 startPos)
{
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
}

void StaticEntity::Draw(Vector2 cameraPosition) {
	Vector2 resultPosition;

	if (!animActor)
		return;	

	resultPosition = { newDrawPosition.x - cameraPosition.x, 
		newDrawPosition.y - cameraPosition.y };

	animActor->Draw(resultPosition, SDL_FLIP_NONE);
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
