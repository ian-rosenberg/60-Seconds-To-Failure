#include "animation.h"
#include <fstream>
#include <string>

Animation::Animation(Animation* old)
{
	name = old->name;
	filepath = old->filepath;
	sprite = new Sprite(old->sprite);
	length = old->length;
	currentFrame = old->currentFrame;
	cellWidth = old->cellWidth;
	cellHeight = old->cellHeight;
	yOffset = old->yOffset;
	xOffset = old->xOffset;
	colorSpecial = old->colorSpecial;
	animType = old->animType;
	frameRate = old->frameRate;
}

Animation::Animation(std::string n, std::string fp, int len, int width, int height, int xOffset, int yOffset, Vector4 color, float fr, float current, AnimationType type, std::shared_ptr<Graphics> g)
{
	name = n;
	filepath = fp;
	sprite = new Sprite(fp.c_str(), vector2( 0,0 ), vector2( 1,1 ), vector2(0,0), vector3( 0,0,0 ), vector2( 0,0 ), color, 0, yOffset, width, height, g);
	length = len;
	currentFrame = current;
	cellWidth = width;
	cellHeight = height;
	this->yOffset = yOffset;
	xOffset = 0;
	colorSpecial = color;
	animType = type;
	frameRate = fr;
}

Animation::Animation(std::string n, Sprite* s, int width, int height, int xOffset, int yOffset, Vector4 color)
{
	name = n;
	filepath = s->GetFilePath();
	sprite = s;
	length = 1;
	currentFrame = 0;
	cellWidth = width;
	cellHeight = height;
	this->yOffset = yOffset;
	xOffset = 0;
	colorSpecial = color;
	animType = AnimationType::AT_LOOP;
	frameRate = 0;
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

	currentFrame += anim->length * frameRate;
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
