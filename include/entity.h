#pragma once

#include <vector>
#include <box2d/box2d.h>
#include <actor.h>
#include <debugdraw.h>

class Entity: public Actor
{
protected:
	int						id;

	b2Body*					body;
	b2Fixture*				jumpTrigger;

	b2Vec2					worldDimensions;

	State					logicalState; // Same enum as used for animations, used here for logic of those states

	Vector2					screenPosition;

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

	Uint16					energy;
	Uint16					maxEnergy;

	float					maxSpeed;

	//Debug Drawing, null if not enabled
	DebugDraw				*debugDraw;
	SDL_Rect			    debugRect;

public:
	Entity();

	~Entity();

	virtual void Draw() = 0;											/**<called after system entity drawing for custom effects*/
	virtual void Think() = 0;											/**<called before system updates to make decisions / hand input*/
	virtual void Update() = 0;											/**<called after system entity update*/
	virtual int Touch(Entity* other) = 0;								/**<when this entity touches another entity*/
	virtual void Activate(Entity* activator) = 0;						/**<some entities can be activated by others, doors opened, levels, etc*/
	virtual int Damage(int amount, Entity* source) = 0;					/**<when this entity takes damage*/
	virtual void Die() = 0;
	virtual void UpdateScreenPosition() = 0;
	
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

	void SetJumpTrigger(b2Fixture* f);

	void SetVelocity(float x, float y);

	void SetLogicalState(State state);

	void SetId(int newId);

	inline int GetId() { return id; }

	void SetBody(b2Body* b);

	inline b2Body* GetBody() { return body; }

	void RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle);

	inline b2Vec2 GetWorldDimensions() { return worldDimensions; }

	void SetWorldDimensions(b2Vec2 dim);

	inline Vector2 GetDrawPosition() { return position; }

	inline void EnableDebugDraw(DebugDraw* ddPtr) { debugDraw = ddPtr; }

	inline Uint8 GetDebugDrawEnabled() { return debugDraw!=NULL;}

	inline DebugDraw* GetDebugDraw() { return debugDraw;}
	
	inline SDL_Rect* GetDebugRect() { return &debugRect; }

	inline Vector2 GetScreenPosition() { return screenPosition; }

	inline const char* GetActorName() { return name.c_str(); }
};

class EntityManager {
private:
	std::vector<Entity*>	*entities; 
	Uint8					debugDraw;

public:
	EntityManager();

	EntityManager(Uint8 debug);

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
