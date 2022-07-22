#pragma once

#include <string>
#include <SDL.h>
#include "sprite.h"

enum class State
{
	State_Idle,
	State_Walking,
	State_Attacking,
	State_Hurt,
	State_Death,
	State_Dead
};

enum class AnimationType
{
	AT_ONCE,
	AT_LOOP,
};

enum class AnimationReturnType
{
	ART_ERROR,
	ART_LOOPING,
	ART_INPROGRESS,
	ART_END
};

/**
* @brief Animation data
*/
class Animation
{
private:
	std::string		name;
	std::string		filepath;

protected:
	Sprite*			sprite;

	Uint32			length;
	Uint32			cellWidth, cellHeight, yOffset, xOffset;

	Vector4			colorSpecial;

	float			frameRate;
	float			frameTimer;
	float			currentFrame;

	AnimationType	animType;

public:
	Animation(std::string n, std::string fp, Uint32 len, Uint32 width, Uint32 height, Uint32 xOffset, Uint32 yOffset, Vector4 color, float fr, float current, AnimationType type, std::shared_ptr<Graphics> g);
	Animation(std::string n, Sprite* s, Uint32 width, Uint32 height, Uint32 xOffset, Uint32 yOffset, Vector4 color);
	~Animation();

	/**
	* @brief Proceed to the next frame
	* @param animList The animation list to pull an animation from
	* @param frame For timing purposes
	* @returns A flag determining the status of the animation
	*/
	AnimationReturnType AnimationNextFrame(Animation* animList);

	float GetCurrentFrame() {
		return currentFrame;
	}

	Uint32 GetYOffset() {
		return yOffset;
	}

	int GetCellHeight() {
		return cellHeight;
	}

	int GetCellWidth() {
		return cellWidth;
	}

	std::string GetName() {
		return name;
	}

	Sprite* GetSprite() {
		return sprite;
	}
};