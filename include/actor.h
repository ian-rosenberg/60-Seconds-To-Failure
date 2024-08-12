#pragma once

#include <memory>
#include <string>
#include <SDL.h>
#include "sprite.h"
#include "animation.h"
#include <unordered_map>

class Actor {
private:
	Animation* GetAnimationByName(const char* name);

protected:
	std::string					name;

	Vector4						color;

	Vector2						scale;
	Vector2						avgDim;

	std::vector<Animation*>*	animations;

	Animation*					currentAnimation;

	Uint32						numAnimations;

	State						animState;
	AnimationReturnType			artStatus;

	std::shared_ptr<Graphics>   graphics;

public:
	/**
	* @brief Get a new actor list
	* @param numAnim The number of animations to allocate
	* @returns an unused/unreferenced actor list
	*/
	Actor();

	~Actor();


	/**
	* @brief Calculate the average width and height
	* of the sprites across all animations
	* @param actor The actor whose attributes we get
	*/
	Vector2 CalculateAverageActorDimensions();


	std::vector<Animation*>* GetAnimations();

	void SetAnimation(Animation* anim);

	void SetActorName(const char* n) { name = n; }

	void SetAnimationState(State s);

	AnimationReturnType AnimationProceed();

	State GetAnimationState() { return animState; }

	const char* GetActorName() { return name.c_str(); }

	Vector2 GetAvgDimensions() { return avgDim; }

	void Draw(Vector2 position, SDL_RendererFlip flipFlags);
	
	void AddAnimation(Animation* anim);

	Animation* GetAnimationByType(State s);

	AnimationReturnType GetAnimationStatus() { return artStatus; }

	AnimationType GetCurrentLoopingType() { return currentAnimation->GetLoopType(); }

	void InitState(State s);

	bool MustCurrentAnimationFinish() { return currentAnimation->MustComplete(); }

	Graphics* GetGraphicsReference() { return graphics.get(); }
};

	/**
	* @brief Load .anim file for a given animation, retrieving relevant Animation data
	* @param filename The file to parse
	*/
Animation* ParseAnimation(const char* filename, const std::shared_ptr<Graphics> graphics);

	/**
	* @brief Load an actor from file
	* @param actor The actor to load into
	* @param file The file to load
	* @returns The actor loaded with animation data
	*/
Actor* LoadActor(const char* file, const std::shared_ptr<Graphics>& graphics);

