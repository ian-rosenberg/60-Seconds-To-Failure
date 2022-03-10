#pragma once

#include <vector>
#include <box2d/b2_body.h>
#include <actor.h>

class Entity: public Actor
{
protected:
	int						id;

	b2Body*					body;
	b2Vec2					worldDimensions;

	State					logicalState; // Same enum as used for animations, used here for logic of those states

	SDL_Rect			    srcRect;

	Vector2					velocity;
	Vector2					position;
	Vector2					scale;												/**<scale to draw sprite at*/
	Vector2					scaleCenter;										/**<where to scale sprite from*/
	Vector3					rotation;											/**<how to rotate the sprite*/
	Vector2					flip;												/**<if to flip the sprite*/
	Vector2					facing;												/**<direction the entity is facing*/

	Uint8					dead;												/**<when true, the entity system will delete the entity on the next update*/

	Uint16					health;
	Uint16					maxHealth;

	float					cooldown;
	int						grounded;

	float					jumpCool;

	Uint16					energy;
	Uint16					maxEnergy;

	float					maxSpeed;

public:
	Entity();

	virtual ~Entity();

	virtual void Draw() = 0;											/**<called after system entity drawing for custom effects*/
	virtual void Think() = 0;											/**<called before system updates to make decisions / hand input*/
	virtual void Update() = 0;											/**<called after system entity update*/
	virtual int Touch(Entity* other) = 0;								/**<when this entity touches another entity*/
	virtual void Activate(Entity* activator) = 0;						/**<some entities can be activated by others, doors opened, levels, etc*/
	virtual int Damage(int amount, Entity* source) = 0;					/**<when this entity takes damage*/
	virtual void Die() = 0;

	/**
	* @brief Set this actor's animation by the name of the animation
	* @param name Animation to set for actor
	*/
	void SetAnimationByName(const char* name);

	/**
	* @brief Set this actor's animation by the name of the animation
	* @param name Animation to set for actor
	*/
	Animation* GetAnimationByName(const char* name);

	void SetVelocity(float x, float y);

	void SetLogicalState(State state);

	void SetId(int newId);

	inline int GetId() { return id; }

	void SetBody(b2Body* b);

	inline b2Body* GetBody() { return body; }

	void RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle);

	inline b2Vec2 GetWorldDimensions() { return worldDimensions; }

	void SetWorldDimensions(b2Vec2 dim);
};

class EntityManager {
private:
	std::vector<Entity*>* entities;

public:
	EntityManager();

	~EntityManager();

	void AddEntity(Entity* ent);

	Entity* DeleteEntity(Entity* ent);

	/**
	* @brief Render all entites to screen
	*/
	void EntityDrawAll();

	/**
	* @brief Update all entites
	*/
	void EntityUpdateAll();

	/**
	* @brief Let all managed entities think
	*/
	void EntityThinkAll();
};
