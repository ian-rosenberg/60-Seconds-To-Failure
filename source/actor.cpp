#include "actor.h"
#include "sprite.h"
#include <vector>
#include <fstream>
#include <sstream>

Actor::Actor()
{
	animState = State::State_Idle;
	color = vector4(0, 0, 0, 0);
	numAnimations = 0;
	scale = vector2(0, 0);
	avgDim = vector2(0, 0);

	name = "";

	animations = new std::vector<Animation*>();
	currentAnimation = nullptr;
	artStatus = AnimationReturnType::ART_END;
}

Actor::~Actor()
{
	Animation* a;
	while (!animations->empty()) {
		a = animations->front();
		animations->erase(animations->begin());
		delete a;
	}

	graphics.reset();
}

Vector2 Actor::CalculateAverageActorDimensions()
{
	int i, j;
	Vector2 dimTotal = {0,0};

	for (i = 0; i < animations->size(); ++i)
	{
		dimTotal.x += animations->at(i)->GetCellWidth();
		dimTotal.y += animations->at(i)->GetCellHeight();
	}

	avgDim.x = dimTotal.x / animations->size();
	avgDim.y = dimTotal.y / animations->size();

	return avgDim;
}

Animation* ParseAnimation(const char* filename, const std::shared_ptr<Graphics> graphics)
{
	Uint32 iteFlag = 0;
	Animation* anim = nullptr;
	std::ifstream in(filename, std::ios::out);
	std::string line = "";
	std::string value = "";
	std::string filepath = "";
	std::string name = "";
	State stt = State::State_Idle;
	Vector4 color = { 0,0,0,0 };
	int length = 0;
	int width = 0;
	int height = 0;
	float fr = 0.0f;
	int stripDir = 0;

	if (!in.is_open()) {
		std::cerr << "Error Unable to open settings file \"" << filename << "\" for reading!" << std::endl;

		return nullptr;
	}

	while (!std::getline(in, line, ':').eof())
	{
		std::getline(in, value, '\n');

		if (strcmp(line.c_str(), "name") == 0)
		{
			name = value;

			if (strcmp(name.c_str(), "idle") == 0)
				stt = State::State_Idle;
			else if (strcmp(name.c_str(), "walk") == 0)
				stt = State::State_Walking;
			else if (strcmp(name.c_str(), "jump") == 0)
				stt = State::State_Jumping;
			else if (strcmp(name.c_str(), "fall") == 0)
				stt = State::State_Falling;
			else if (strcmp(name.c_str(), "land") == 0)
				stt = State::State_Landing;

			continue;
		}
		if (strcmp(line.c_str(), "filename") == 0)
		{
			filepath = value;

			continue;
		}
		if (strcmp(line.c_str(), "colorSpecial") == 0)
		{
			sscanf_s(value.c_str(), "%lf,%lf,%lf,%lf", &color.x, &color.y, &color.z, &color.w);

			continue;
		}
		if (strcmp(line.c_str(), "stripDir") == 0)
		{
			stripDir = atoi(value.c_str());
			
			continue;
		}
		if (strcmp(line.c_str(), "length") == 0)
		{
			length = atoi(value.c_str());

			continue;
		}
		if (strcmp(line.c_str(), "frameWidth") == 0)
		{
			width = atoi(value.c_str());

			continue;
		}
		if (strcmp(line.c_str(), "frameHeight") == 0)
		{
			height = atoi(value.c_str());

			continue;
		}
		if (strcmp(line.c_str(), "frameRate") == 0)
		{
			fr = atof(value.c_str());

			continue;
		}
		if (strcmp(line.c_str(), "type") == 0)
		{
			if (strcmp(value.c_str(), "loop") == 0)
			{
				anim = new Animation(name, filepath, length, width, height, color, fr, 0, AnimationType::AT_LOOP, graphics, stripDir, stt);
			}
			else if (strcmp(value.c_str(), "once") == 0)
			{
				anim = new Animation(name, filepath, length, width, height, color, fr, 0, AnimationType::AT_ONCE, graphics, stripDir, stt);
			}
			else if (strcmp(value.c_str(), "hold") == 0)
			{
				anim = new Animation(name, filepath, length, width, height, color, fr, 0, AnimationType::AT_HOLD, graphics, stripDir, stt);
			}

			std::cout << "Animation " << name << " loaded!" << std::endl;

			break;
		}
	}

	in.close();

	return anim;
}

std::vector<Animation*>* Actor::GetAnimations()
{
	return animations;
}

Animation* Actor::GetAnimationByName(const char* name)
{
	std::vector<Animation*>* animations = GetAnimations();

	for (int i = 0; i < animations->size(); i++)
	{
		if (strcmp(animations->at(i)->GetName().c_str(), name) == 0) {
			return animations->at(i);
		}
	}

	return nullptr;
}

Animation* Actor::GetAnimationByType(State s)
{
	std::vector<Animation*>* animations = GetAnimations();

	for (int i = 0; i < animations->size(); i++)
	{
		if (animations->at(i)->GetAnimStateType() == s) {
			return animations->at(i);
		}
	}

	return nullptr;
}

void Actor::SetAnimation(Animation* anim)
{
	currentAnimation = anim;
	currentAnimation->ResetFrame();
}

void Actor::SetAnimationState(State s){
	if (s == animState)
		return;
	
	SetAnimation(GetAnimationByType(s));
  	animState = s;
}

AnimationReturnType Actor::AnimationProceed() {
	return artStatus = currentAnimation->AnimationNextFrame();
}

void Actor::Draw(Vector2 position, SDL_RendererFlip flipFlags)
{
	currentAnimation->DrawAnimationCell(position, flipFlags);
}

void Actor::AddAnimation(Animation* anim)
{
	animations->push_back(anim);
}

Actor* LoadActor(const char* filename, const std::shared_ptr<Graphics>& graphics)
{
	int numAnim = 0;
	std::vector<std::string> filenames = std::vector<std::string>();
	std::string name;
	Vector4 color;
	Vector2 scale;
	Vector2 avgDim;
	Actor* animActor;

	//NEW ACTOR CONSTRUCTOR
	std::ifstream in(filename);
	std::string line = "";
	std::string value = "";

	if (!in.is_open()) {
		std::cerr << "Error Unable to open settings file \"" << filename << "\" for reading!" << std::endl;

		return nullptr;
	}

	while (!std::getline(in, line, ':').eof())
	{
		if (strcmp(line.c_str(), "numAnimations") == 0)
		{
			std::getline(in, value, '\n');
			numAnim = atoi(value.c_str());

			if (numAnim)
			{
				break;
			}
		}
	}

	while (!std::getline(in, line, ':').eof())
	{
		if (strcmp(line.c_str(), "actorName") == 0)
		{
			std::getline(in, value, '\n');
			name = value;

			continue;
		}
		if (strcmp(line.c_str(), "color") == 0)
		{
			std::getline(in, value, '\n');
			sscanf_s(value.c_str(), " %lf,%lf,%lf,%lf", &color.x, &color.y, &color.z, &color.w);

			continue;
		}
		if (strcmp(line.c_str(), "scale") == 0)
		{
			std::getline(in, value, '\n');
			sscanf_s(value.c_str(), " %lf,%lf", &scale.x, &scale.y);

			if (scale.x >= 0 && scale.x <= INT_MAX)
			{
				break;
			}
			{
				scale = { 1,1 };
			}

		}
	}

	in.seekg(0);

	for (int i = 0; i < numAnim; ++i)
	{
		while (!std::getline(in, line, ':').eof())
		{
			std::getline(in, value, '\n');

			if (strcmp(line.c_str(), "Animation") == 0)
			{
				filenames.push_back(value);

				break;
			}
		}
	}

	in.close();


	animActor = new Actor();

	for (int i = 0; i < numAnim; ++i)
	{
		char* p_writable_data = (char*)filenames.at(i).data();

		animActor->AddAnimation(ParseAnimation(p_writable_data, graphics));
	}

	avgDim = animActor->CalculateAverageActorDimensions();

	std::cout << "Actor dimensions: {" << avgDim.x << " , " << avgDim.y << "}" << std::endl;

	std::cout << "Loaded actor " << name << std::endl;

	return animActor;
}

