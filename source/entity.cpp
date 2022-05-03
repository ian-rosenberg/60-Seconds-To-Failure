#include "entity.h"

Entity::Entity()
{
	id = -1;
	logicalState = State::State_Idle;
	worldDimensions = {};
	srcRect = { 0,0,0,0 };
	screenPosition = vector2(0, 0);
	debugRect = { 0,0,0,0 };
	velocity = { 0,0 };
	position = { 0,0 };
	scale = { 0,0 };
	scaleCenter = { 0,0 };
	rotation = { 0,0,0 };
	flip = { 0,0 };
	facing = { 0,0 };
	maxHealth = 0;
	health = 0;
	cooldown = 0;
	grounded = 0;
	energy = 0;
	maxEnergy = 0;
	maxSpeed = 0;
	dead = 0;
	body = NULL;
	jumpTrigger = NULL;
}

Entity::~Entity()
{
	if (animations) {
		while (!animations->empty()) {
			Animation* anim = animations->back();
			animations->pop_back();
			delete anim;
		}
	}

	if (currentAnimation)
		currentAnimation = NULL;
	
	if (currentSprite)
		currentSprite = NULL;
	 
	renderer = NULL;
}
void Entity::SetAnimationByName(const char* name)
{
	std::vector<Animation*>* animations = GetAnimations();

	for (int i = 0; i < animations->size(); i++)
	{
		if (strcmp(animations->at(i)->GetName().c_str(), name) == 0) {
			SetAnimation(animations->at(i));
			return;
		}
	}
}

void Entity::SetJumpTrigger(b2Fixture* f) {
	jumpTrigger = f;
}

Animation* Entity::GetAnimationByName(const char* name)
{
	std::vector<Animation*>* animations = GetAnimations();
	
	for (int i = 0; i < animations->size(); i++)
	{
		if (strcmp(animations->at(i)->GetName().c_str(), name) == 0) {
			return animations->at(i);
		}
	}

	return NULL;
}

void Entity::SetVelocity(float x, float y)
{
	velocity = { x,y };
	b2Vec2 v = b2Vec2(velocity.x, velocity.y);

	if ((x != 0 || y != 0) && abs(body->GetLinearVelocity().x) < maxSpeed) {
		body->ApplyForceToCenter(v, true);
	}
}

void Entity::RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
	b2Vec2 tmp;
	tmp.x = vector.x * cos(angle) - vector.y * sin(angle);
	tmp.y = vector.x * sin(angle) + vector.y * cos(angle);
	vector = tmp + center;
}

void Entity::SetLogicalState(State state)
{
	logicalState = state;
}

void Entity::SetId(int newId)
{
	id = newId;
}

void Entity::SetBody(b2Body* b) {
	if (!b) {
		std::cout << "Body is NULL." << std::endl;

		return;
	}
	
	body = b;
}

void Entity::SetWorldDimensions(b2Vec2 dim) {
	worldDimensions.Set(dim.x, dim.y);
}

void Entity::UpdateScreenPosition()
{
	b2Vec2 worldPos;

	if (body)
		worldPos = body->GetPosition();
	else{
		screenPosition = GetDrawPosition();
		return;
	}

	debugRect.w = srcRect.w * PIX_TO_MET;
	debugRect.h = srcRect.h * PIX_TO_MET;

	screenPosition.x = debugRect.x = ((SCALED_WIDTH / 2.0f) + worldPos.x) * MET_TO_PIX - worldDimensions.x / 2;
	screenPosition.y = debugRect.y = ((SCALED_HEIGHT / 2.0f) + worldPos.y) * MET_TO_PIX - worldDimensions.y / 2;
}


EntityManager::EntityManager()
{
	entities = new std::vector<Entity*>();
}

EntityManager::~EntityManager()
{
	if (entities) {
		while (!entities->empty()) {
			Entity* e = entities->back();
			entities->pop_back();
			delete e;
		}
	}
}

void EntityManager::AddEntity(Entity* ent)
{
	entities->push_back(ent);
	ent->SetId(entities->size());
}

Entity* EntityManager::DeleteEntity(Entity* ent)
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		if (*it == ent) {
			Entity* ent = *it;
			entities->erase(it);

			return ent;
		}
	}
}

void EntityManager::EntityDrawAll()
{
	SDL_Rect r;
	Vector2 p, dim;

	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{

		(*it)->Draw();

		//Debug Drawing
		if ((*it)->GetDebugDrawEnabled()) {
			p = (*it)->GetScreenPosition();
			dim = (*it)->GetAverageActorDimensions();
			r = { (int)floor(p.x-(dim.x/2)), (int)floor(p.y-(dim.y/2)), (int)dim.x, (int)dim.y };
			(*it)->GetDebugDraw()->DrawRect(&r); 
		}
	}
}

void EntityManager::EntityUpdateAll()
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->Update();
		(*it)->UpdateScreenPosition();
	}
}

void EntityManager::EntityThinkAll()
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->Think();
	}
}

void EntityManager::SetDebugDrawActive()
{
	DebugDraw* dd = new DebugDraw((*entities->begin())->GetRenderer());

	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->EnableDebugDraw(dd);
	}
}

