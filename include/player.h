#pragma once

#include <string>
#include <entity.h>

class Player : public Entity
{
private:
	const std::string		actorFilePath = "data/Player.actor";
	std::string 			name;

	const Uint8*			keys;
	SDL_GameController*		controller;
	float					sensitivity;

	Vector3					dimensions;

	Vector2					enteredFrom;

	int32_t					axisLeftXLock;
	int32_t					axisLeftYLock;
	int32_t					axisRightXLock;
	int32_t					axisRightYLock;

	Bool					punching;

public:
	Player(SDL_Renderer* renderer);

	/**
	* @brief Handle input
	*/
	void Think();

	/**
	* @brief Draw the player to screen
	*/
	void Draw();

	/**
	* @brief Update the player's state
	*/
	void Update();

	int Touch(Entity* other);									/**<when this entity touches another entity*/
	
	void Activate(Entity* activator);							/**<some entities can be activated by others, doors opened, levels, etc*/
	
	int Damage(int amount, Entity* source);						/**<when this entity takes damage*/

	void Die();													/**<when this entity dies*/
};

