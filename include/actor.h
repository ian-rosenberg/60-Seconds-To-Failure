#pragma once

#include <memory>
#include <string>
#include <SDL.h>
#include "sprite.h"
#include "animation.h"

class Actor {
protected:
	std::string					name;

	Sprite*						currentSprite;

	Vector4						color;

	Vector2						scale;
	Vector2						avgDim;

	std::vector<Animation*>*	animations;

	Animation*					currentAnimation;

	Uint32						numAnimations;

	State						animState;

	std::shared_ptr<Graphics>	graphics;

public:
	/**
	* @brief Get a new actor list
	* @param numAnim The number of animations to allocate
	* @returns an unused/unreferenced actor list
	*/
	Actor();

	~Actor();

	SDL_Renderer* GetRenderer() { return graphics->GetRenderer(); }

	/**
	* @brief Load an actor from file
	* @param actor The actor to load into
	* @param file The file to load
	* @returns The actor loaded with animation data
	*/
	Actor* LoadActor(const char* file);

	/**
	* @brief Calculate the average width and height
	* of the sprites across all animations
	* @param actor The actor whose attributes we get
	*/
	void CalculateAverageActorDimensions();

	/**
	* @brief Load .anim file for a given animation, retrieving relevant Animation data
	* @param filename The file to parse
	*/
	Animation* ParseAnimation(const char* filename);

	std::vector<Animation*>* GetAnimations();

	void SetAnimation(Animation* anim);

	void SetActorName(const char* n) { name = n; }

	void SetAnimationState(State s) { animState = s; }
};