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

	int			length;
	int			cellWidth, cellHeight, yOffset, xOffset;

	Vector4			colorSpecial;

	float			frameRate;
	float			frameTimer;
	float			currentFrame;

	AnimationType	animType;

public:
	Animation(Animation* old);
	Animation(std::string n, std::string fp, int len, int width, int height, int xOffset, int yOffset, Vector4 color, float fr, float current, AnimationType type, std::shared_ptr<Graphics> g);
	Animation(std::string n, Sprite* s, int width, int height, int xOffset, int yOffset, Vector4 color);
	~Animation();

	/**
	* @brief Proceed to the next frame
	* @param animList The animation list to pull an animation from
	* @param frame For timing purposes
	* @returns A flag determining the status of the animation
	*/
	AnimationReturnType AnimationNextFrame(Animation* animList);

	inline float GetCurrentFrame() {
		return currentFrame;
	}

	inline int GetYOffset() {
		return yOffset;
	}

	inline int GetXOffset() {
		return xOffset;
	}

	inline int GetCellHeight() {
		return cellHeight;
	}

	inline int GetCellWidth() {
		return cellWidth;
	}

	inline std::string GetName() {
		return name;
	}

	inline Sprite* GetSprite() {
		return sprite;
	}
};