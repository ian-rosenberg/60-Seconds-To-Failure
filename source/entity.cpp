#include "entity.h"

Entity::Entity()
{
	id = -1;
	logicalState = State::State_Idle;
	srcRect = { 0,0,0,0 };
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
	jumpCool = 0;
	energy = 0;
	maxEnergy = 0;
	dead = 0;
}

Entity::~Entity()
{
	if (animations)
	{
		delete[] animations;
	}
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
}

void Entity::SetLogicalState(State state)
{
	logicalState = state;
}

void Entity::SetId(int newId)
{
	id = newId;
}

EntityManager::EntityManager()
{
	entities = new std::vector<Entity*>();
}

EntityManager::~EntityManager()
{
	for (int i = entities->size()-1; i >= 0; i--) {
		delete entities->at(i);
	}

	entities->clear();
}

void EntityManager::AddEntity(Entity* ent)
{
	entities->push_back(ent);
	ent->SetId(entities->size());
}

void EntityManager::DeleteEntity(Entity* ent)
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		if (*it == ent) {
			entities->erase(it);
			return;
		}
	}
}

void EntityManager::EntityDrawAll()
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->Draw();
	}
}

void EntityManager::EntityUpdateAll()
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->Update();
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

