#include "animation.h"
#include <fstream>
#include <string>

Animation::Animation(const Animation & old)
{
	name = old.name;
	filepath = old.filepath;
	sprite = new Sprite(*old.sprite);
	length = old.length;
	cellWidth = old.cellWidth;
	cellHeight = old.cellHeight;
	yOffset = old.yOffset;
	xOffset = old.xOffset;
	colorSpecial = old.colorSpecial;
	animType = old.animType;
	graphics = nullptr;
	cFrame = 0.f;
	pFrame = 0.f;
}

Animation::Animation(std::string n, std::string fp, int len, int width, int height, int xOffset, int yOffset, Vector4 color, float fr, float current, AnimationType type, const std::shared_ptr<Graphics>& graphics)
{
	name = n;
	filepath = fp;
	sprite = new Sprite(fp.c_str(), vector2( 0,0 ), vector2( 1,1 ), vector2(0,0), vector3( 0,0,0 ), vector2( 0,0 ), color, 0, yOffset, width, height, graphics);
	length = len;
	cellWidth = width;
	cellHeight = height;
	this->yOffset = yOffset;
	xOffset = 0;
	colorSpecial = color;
	animType = type;
	this->graphics = (const std::shared_ptr<Graphics>&)graphics;
	cFrame = 0.f;
	pFrame = 0.f;
}

Animation::Animation(std::string n, Sprite* s, int width, int height, int xOffset, int yOffset, Vector4 color, const std::shared_ptr<Graphics>& graphics)
{
	name = n;
	filepath = s->GetFilePath();
	sprite = s;
	length = 1;
	cellWidth = width;
	cellHeight = height;
	this->yOffset = yOffset;
	xOffset = 0;
	colorSpecial = color;
	animType = AnimationType::AT_LOOP;
	this->graphics = graphics;
	cFrame = 0.f;
	pFrame = 0.f;
}

Animation::~Animation()
{
	if (graphics)
		graphics.reset();

	if (sprite != nullptr)
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
	float dT = graphics->GetFrameDeltaTime() / 1000.0f;
	float framesToUpdate = dT / (1.0f / SPRITE_ANIMATION_RATE);
	float alpha = graphics->GetAccumulatorTime();

	pFrame = cFrame;

	if (framesToUpdate > 0) {
		cFrame += framesToUpdate;
	}

	cFrame = cFrame * alpha + pFrame * (1.0 - alpha);

	if (cFrame >= anim->length - 1)
	{
		switch (anim->animType)
		{
		case AnimationType::AT_LOOP:
			cFrame = 0;
			return AnimationReturnType::ART_LOOPING;
		case AnimationType::AT_ONCE:
			cFrame = 0;//fix this loop
			return AnimationReturnType::ART_END;
		}
	}

	return AnimationReturnType::ART_INPROGRESS;
}
