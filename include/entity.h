#pragma once

#include <vector>
#include <box2d/box2d.h>
#include <actor.h>
#include <debugdraw.h>
#include <queue>
#include <functional>

typedef enum {
	MENU,
	WALK_UP,
	WALK_RIGHT,
	WALK_DOWN,
	WALK_LEFT,
	INTERACT,
	DECLINE,
	JUMP,
	PUNCH,
	KICK,
	SHOOT,
	NONE
}InputType;

class Entity: public Actor
{
protected:
	int												id;

	Uint8											gravityEnabled;

	b2Body*											body;
	b2Fixture*										jumpTrigger;

	b2Vec2											worldDimensions;

	State											logicalState; // Same enum as used for animations, used here for logic of those states
		
	Entity*											parentEntity;

	Vector2											velocity;
	Vector2											prevDrawPosition;
	Vector2											newDrawPosition;
	b2Vec2											prevBodyPosition;
	b2Vec2											newBodyPosition;
	Vector2											scale;												/**<scale to draw sprite at*/
	Vector2											scaleCenter;										/**<where to scale sprite from*/
	Vector3											rotation;											/**<how to rotate the sprite*/
	Vector2											flip;												/**<if to flip the sprite*/
	Vector2											facing;												/**<direction the entity is facing*/

	Uint8											dead;												/**<when true, the entity system will delete the entity on the next update*/

	Uint16											health;
	Uint16											maxHealth;

	const double									jumpCooldown = 2550.0;	//1.25 second jump cooldown
	double											jumpTimer;
	float											jumpForce;
	float											dampening;
	bool											grounded;
		
	Uint16											energy;
	Uint16											maxEnergy;

	float											maxSpeed;

	//Debug Drawing, null if not enabled
	DebugDraw										*debugDraw;
	SDL_Rect										debugRect;

public:
	struct InputEvent {
		InputEvent*				prevEvent;
		Uint8					keyCount;
		Uint8					gravity;
		Uint8					repeat;
		SDL_Event*				e;
		InputType				inputType;
		Uint8					keyDown;
		Uint32					msSinceLastInput;
		void*					data;

		std::function<void(Entity::InputEvent*)> onPress;
		std::function<void(Entity::InputEvent*)> onHold;
		std::function<void(Entity::InputEvent*)> onRelease;

		/*
		void			(Entity::* onPress)(InputEvent* data);        //callback for press event
		void			(Entity::*onHold)(InputEvent* data);         //callback for hold event
		void			(Entity::*onRelease)(InputEvent* data);      //callback for release event
		*/

		InputEvent() {
			prevEvent = nullptr;
			keyCount = 0;
			gravity = 0;
			e = nullptr;
			keyDown = 0;
			msSinceLastInput = 0;
			inputType = NONE;
			data = nullptr;
			onPress = nullptr;
			onHold = nullptr;
			onRelease = nullptr;
		}

		~InputEvent() {
			prevEvent = nullptr;
			keyCount = 0;
			gravity = 0;
			e = nullptr;
			keyDown = 0;
			msSinceLastInput = 0;
			inputType = NONE;
			data = nullptr;
			onPress = nullptr;
			onHold = nullptr;
			onRelease = nullptr;
		}
	};

	Entity();

	~Entity();

	virtual void Draw() = 0;											/**<called after system entity drawing for custom effects*/
	virtual void Think() = 0;											/**<called before system updates to make decisions / hand input*/
	virtual void Update() = 0;											/**<called after system entity update*/
	virtual int Touch(Entity* other) = 0;								/**<when this entity touches another entity*/
	virtual void Activate(Entity* activator) = 0;						/**<some entities can be activated by others, doors opened, levels, etc*/
	virtual int Damage(int amount, Entity* source) = 0;					/**<when this entity takes damage*/
	virtual void Die() = 0;
	virtual void UpdateScreenPosition(double alpha) = 0;
	
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

	void SetLogicalState(State state);

	void SetId(int newId);

	inline int GetId() { return id; }

	void SetBody(b2Body* b);

	inline b2Body* GetBody() { return body; }

	inline b2Fixture* GetJumpTrigger() { return jumpTrigger; }

	void RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle);

	inline b2Vec2 GetWorldDimensions() { return worldDimensions; }

	void SetWorldDimensions(b2Vec2 dim);
	
	/** Gameplay -------------------------------------------------------------**/
	void Jump(InputEvent* e);

	void ToggleGrounded(int flag);

	void SetVelocity(InputEvent* e);
	inline void SetPixelVelocity(Vector2 v) { velocity = v; }

	inline void SetGravityEnabled(Uint8 flag) { gravityEnabled = flag; }

	/** Inline Fcns -------------------------------------------------------------**/
	inline Vector2 GetDrawPosition() { return newDrawPosition; }

	inline void EnableDebugDraw(DebugDraw* ddPtr) { debugDraw = ddPtr; }

	inline Uint8 GetDebugDrawEnabled() { return debugDraw!=NULL;}

	inline DebugDraw* GetDebugDraw() { return debugDraw;}
	
	inline Vector2 GetScreenPosition() { return newDrawPosition; }

	inline const char* GetActorName() { return name.c_str(); }

	inline void DecrementJumpTimer(double ticks) { jumpTimer -= (jumpTimer > 0) ? ticks : 0; }

	inline void ResetJumpTimer() { jumpTimer = 0.0f; }

	inline bool IsJumpTimeReady() { return jumpTimer <= 0; }

	inline bool IsGrounded() { return grounded; }
};

class EntityManager {
private:
	std::vector<Entity*>*					entities; 
	Uint8									debugDraw;
	std::vector<Entity::InputEvent*>*		inputQueue;
	std::queue<Entity::InputEvent*>*		eventsToFire;
	std::shared_ptr<Graphics>				graphics;

public:
	EntityManager();

	EntityManager(Uint8 debug, std::shared_ptr<Graphics> g);

	~EntityManager();

	void AddEntity(Entity* ent);

	Entity* DeleteEntity(Entity* ent);

	void ClearInputEventQueue();

	void ClearEventsToFireQueue();

	Entity::InputEvent* PeekInputEventAtIndex(Uint16 index);

	Entity::InputEvent* GetNextInputEvent();

	/**
	* Input driver integrated per entity manager, one active at a time
	*/
	inline void PushBackInputEvent(Entity::InputEvent* newEvent) { inputQueue->push_back(newEvent); }

	inline void PushBackEventToFire(Entity::InputEvent* newEvent) { eventsToFire->push(newEvent); }

	inline std::vector<Entity::InputEvent*>* GetInputQueue() { return inputQueue; }

	inline std::queue<Entity::InputEvent*>* GetEventsToFire() { return eventsToFire; }

	/**
	* @brief Render all entites to screen
	*/
	void EntityDrawAll(double alpha);

	void DebugDrawing(Entity* it);

	/**
	* @brief Update all entites
	*/
	void EntityUpdateAll(double ticks);

	/**
	* @brief Let all managed entities think
	*/
	void InputUpdate();

	void EntityThinkAll();

	void SetAlpha(double a);
};
