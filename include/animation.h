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
	std::shared_ptr<Graphics> graphics;

protected:
	Sprite*			sprite;

	int				length;
	int				cellWidth, cellHeight, yOffset, xOffset;
	float           cFrame;
	float           pFrame;


	Vector4			colorSpecial;	

	AnimationType	animType;

public:
	Animation(const Animation & old);
	Animation(std::string n, std::string fp, int len, int width, int height, int xOffset, int yOffset, Vector4 color, float fr, float current, AnimationType type, const std::shared_ptr<Graphics>& graphics);
	Animation(std::string n, Sprite* s, int width, int height, int xOffset, int yOffset, Vector4 color, const std::shared_ptr<Graphics>& graphics);
	~Animation();

	/**
	* @brief Proceed to the next frame
	* @param animList The animation list to pull an animation from
	* @param frame For timing purposes
	* @returns A flag determining the status of the animation
	*/
	AnimationReturnType AnimationNextFrame();

	int GetCurrentFrame() {
		return floor(cFrame);
	}

	int GetYOffset() {
		return yOffset;
	}

	int GetXOffset() {
		return xOffset;
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