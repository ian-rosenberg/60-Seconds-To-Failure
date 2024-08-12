#pragma once

#include <string>
#include <SDL.h>
#include "sprite.h"

enum class State
{
	State_Idle,
	State_Walking,
	State_Jumping,
	State_Falling,
	State_Landing,
	State_Attacking,
	State_Hurt,
	State_Death,
	State_Dead,
	State_Invalid
};

enum class AnimationType
{
	AT_ONCE,
	AT_LOOP,
	AT_HOLD
};

enum class AnimationReturnType
{
	ART_ERROR,
	ART_LOOPING,
	ART_INPROGRESS,
	ART_HOLD,
	ART_END
};

enum class AnimationDirection : unsigned short
{
	X,
	Y,
	BOTH
};

/**
* @brief Animation data
*/
class Animation
{
private:
	std::string					name;
	std::string					filepath;
	std::shared_ptr<Graphics>	graphics;

	std::vector<State>			possibleStatesNext;

protected:
	Uint8						paused;
	bool						mustComplete;
	Sprite*						sprite;

	SDL_Rect					srcRect;

	int							length;
	int							cellWidth, cellHeight;
	float						cFrame;
	float						pFrame;
	float						frameRate;


	Vector4						colorSpecial;	

	AnimationType				animType;

	State						animStateType;

	AnimationDirection			stripDirection;

public:
	Animation(const Animation & old);
	Animation(std::string n, std::string fp, int len, int width, int height, Vector4 color, float fr, float current, AnimationType type, const std::shared_ptr<Graphics>& graphics, unsigned short animDir, State aState, bool finish, std::vector<State> tStates);
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

	int GetCellHeight() {
		return cellHeight;
	}

	int GetCellWidth() {
		return cellWidth;
	}

	std::string GetName() {
		return name;
	}

	int GetLength() {
		return length;
	}

	void DrawAnimationCell(Vector2 pixelDrawPosition, SDL_RendererFlip flipFlags);

	bool PauseAnimation();

	bool ResumeAnimation();

	bool IsLastFrame() { return (int)cFrame == (length - 1); }

	bool IsPaused() { return paused > 0; }

	bool InProgress() { return (int)cFrame < length - 1; }

	bool MustComplete() { return mustComplete; }

	State GetAnimStateType() { return animStateType; }

	AnimationType GetLoopType() { return animType; }

	void ResetFrame() { cFrame = paused = 0; }

	bool IsPossibleState(State s) { return std::find(possibleStatesNext.begin(), possibleStatesNext.end(), s) != possibleStatesNext.end(); }
};