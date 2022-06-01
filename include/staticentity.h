#pragma once
#include "entity.h"
#include <vector>
#include <box2d/box2d.h>
#include <actor.h>
#include <debugdraw.h>

class StaticEntity : public Entity {
private:
	int id;

	Uint8 isTrigger;
	b2Fixture* boundingVolume;
	
	b2Vec2 worldPosition;

	SDL_Rect srcRect;

public:
	StaticEntity();

	StaticEntity(std::shared_ptr<Graphics> g);

	StaticEntity(std::shared_ptr<Graphics> g, float w, float h);

	~StaticEntity();

	void Draw();

	void Think();

	void Update();

	int Touch(Entity* other);									/**<when this entity touches another entity*/

	void Activate(Entity* activator);							/**<some entities can be activated by others, doors opened, levels, etc*/

	int Damage(int amount, Entity* source);						/**<when this entity takes damage*/

	void Die();													/**<when this entity dies*/

	void UpdateScreenPosition(double alpha);

	inline void SetStaticTriggerFixture(b2Fixture* f) { boundingVolume = f; }
};