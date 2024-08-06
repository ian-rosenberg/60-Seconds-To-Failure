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
	colorSpecial = old.colorSpecial;
	animType = old.animType;
	graphics = nullptr;
	cFrame = 0.f;
	pFrame = 0.f;
	stripDirection = old.stripDirection;
	srcRect = old.srcRect;
	paused = 0;
	frameRate = old.frameRate;
	animStateType = old.animStateType;
}

Animation::Animation(std::string n, std::string fp, int len, int width, int height, Vector4 color, float fr, float current, AnimationType type, const std::shared_ptr<Graphics>& graphics, unsigned short animDir, State aState)
{
	name = n;
	filepath = fp;
	sprite = new Sprite(fp.c_str(), vector2( 0,0 ), vector2( 1,1 ), vector2(0,0), vector3( 0,0,0 ), vector2( 0,0 ), color, 0, 0, width, height, graphics);
	length = len;
	cellWidth = width;
	cellHeight = height;
	colorSpecial = color;
	animType = type;
	this->graphics = (const std::shared_ptr<Graphics>&)graphics;
	cFrame = 0.f;
	pFrame = 0.f;
	srcRect = { 0,0,cellWidth,cellHeight };
	paused = 0;
	frameRate = fr;
	animStateType = aState;

	if (animDir == 0) {
		stripDirection = AnimationDirection::X;
	}
	else if (animDir == 1){		
		stripDirection = AnimationDirection::Y;
	}
	else {
		stripDirection = AnimationDirection::BOTH;
	}
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

AnimationReturnType Animation::AnimationNextFrame()
{
	if (paused)
		return AnimationReturnType::ART_HOLD;
	
	if (!sprite)
	{
		std::cout << "No animation found by name!" << std::endl;
		return AnimationReturnType::ART_ERROR;
	}
	float dT = graphics->GetFrameDeltaTime() / 1000.0f;
	float framesToUpdate = dT / frameRate;
	float alpha = graphics->GetAccumulatorTime();

	pFrame = cFrame;

	if (framesToUpdate > 0) {
		cFrame += framesToUpdate;
	}
	else {
		cFrame += frameRate;
	}

	cFrame = pFrame + (cFrame-pFrame) * alpha;


	if ((int)cFrame >= length - 1)
	{
		switch (animType)
		{
		case AnimationType::AT_LOOP:
			cFrame = 0;
			return AnimationReturnType::ART_LOOPING;
		case AnimationType::AT_ONCE:
			cFrame = 0;
			return AnimationReturnType::ART_END;
		case AnimationType::AT_HOLD:
			paused = true;
			break;
		}
	}
	if(stripDirection == AnimationDirection::BOTH)
		srcRect = { int(cFrame) * cellWidth,
		int(cFrame) * cellHeight, 
		cellWidth, 
		cellHeight 
		};
	else if(stripDirection == AnimationDirection::Y)
		srcRect = { 0,
			int(cFrame) * cellHeight,
			cellWidth,
			cellHeight
	};
	else {
		srcRect = { int(cFrame) * cellWidth,
		0,
		cellWidth,
		cellHeight
		};
	}

	return AnimationReturnType::ART_INPROGRESS;
}

void Animation::DrawAnimationCell(Vector2 pixelDrawPosition, SDL_RendererFlip flipFlags)
{
	sprite->Draw(pixelDrawPosition, srcRect, flipFlags);
}

bool Animation::PauseAnimation()
{
	if (paused > 0)
		return false;
	else
		paused = 1;

	return true;
}

bool Animation::ResumeAnimation()
{
	if (paused < 1)
		return false;
	else
		paused = 0;

	return true;
}


