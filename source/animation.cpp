#include "animation.h"
#include <fstream>
#include <string>

Animation::Animation(std::string n, std::string fp, Uint32 len, Uint32 width, Uint32 height, Uint32 offset, Vector4 color, float fr, float current, AnimationType type, SDL_Renderer* renderer)
{
	name = n;
	filepath = fp;
	sprite = new Sprite(fp.c_str(), vector2( 0,0 ), &vector2( 1,1 ), &vector2(0,0), &vector3( 0,0,0 ), vector2( 0,0 ), &color, 0, offset, width, height, renderer);
	length = len;
	currentFrame = current;
	cellWidth = width;
	cellHeight = height;
	yOffset = offset;
	colorSpecial = color;
	animType = type;
	frameRate = fr;
}

Animation::~Animation()
{
	if (sprite != NULL)
		delete sprite;
	else
		return;
}

AnimationReturnType Animation::AnimationNextFrame(Animation* anim)
{
	if (!anim)
	{
		std::cout << "No animation found by name!" << std::endl;
		return AnimationReturnType::ART_ERROR;
	}

	currentFrame += anim->frameRate;
	if ((int)(currentFrame) >= anim->length - 1)
	{
		switch (anim->animType)
		{
		case AnimationType::AT_LOOP:
			currentFrame = 0;
			return AnimationReturnType::ART_LOOPING;
		case AnimationType::AT_ONCE:
			currentFrame = 0;//fix this loop
			return AnimationReturnType::ART_END;
		}
	}
	return AnimationReturnType::ART_INPROGRESS;
}
