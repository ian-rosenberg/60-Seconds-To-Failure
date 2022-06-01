#include "actor.h"
#include "sprite.h"
#include <vector>
#include <fstream>
#include <sstream>

Actor::Actor()
{
	int j = 0;
	Actor* act = NULL;

	animState = State::State_Idle;
	currentSprite = NULL;
	color = vector4(0, 0, 0, 0);
	numAnimations = 0;
	scale = vector2(0, 0);
	avgDim = vector2(0, 0);

	name = "";

	animations = new std::vector<Animation*>();
}

Actor::~Actor()
{
}

Actor* Actor::LoadActor(const char* filename)
{
	int numAnim = 0;
	std::vector<std::string> filenames = std::vector<std::string>();
	std::ifstream in(filename);
	std::string line = "";
	std::string value = "";

	if (!in.is_open()) {
		std::cerr << "Error Unable to open settings file \"" << filename << "\" for reading!" << std::endl;

		return NULL;
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

	for (int i = 0; i < numAnim; ++i)
	{
		char* p_writable_data = (char*)filenames.at(i).data();
		
		animations->push_back(ParseAnimation(p_writable_data));
	}

	std::cout << "Actor dimensions: {" << avgDim.x << " , " << avgDim.y << "}" << std::endl;

	std::cout << "Loaded actor " << name << std::endl;

	return this;
}

void Actor::CalculateAverageActorDimensions()
{
	int i, j;
	Vector2 dimTotal = {0,0};
	float accum = 0.0f;

	for (i = 0; i < animations->size(); ++i)
	{
		dimTotal.x += animations->at(i)->GetCellWidth();
		dimTotal.y += animations->at(i)->GetCellHeight();
	}

	avgDim.x = dimTotal.x / animations->size();
	avgDim.y = dimTotal.y / animations->size();
}

Vector2 Actor::GetAverageActorDimensions()
{
	return avgDim;
}

Animation* Actor::ParseAnimation(const char* filename)
{
	Uint32 iteFlag = 0;
	Animation* anim = NULL;
	std::ifstream in(filename, std::ios::out);
	std::string line = "";
	std::string value = "";
	std::string filepath = "";
	std::string name = "";
	Vector4 color = { 0,0,0,0 };
	int offset = 0;
	int length = 0;
	int width = 0;
	int height = 0;
	float fr = 0.0f;

	if (!in.is_open()) {
		std::cerr << "Error Unable to open settings file \"" << filename << "\" for reading!" << std::endl;

		return NULL;
	}

	while (!std::getline(in, line, ':').eof())
	{
		std::getline(in, value, '\n');

		if (strcmp(line.c_str(), "name") == 0)
		{
			name = value;

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
		if (strcmp(line.c_str(), "yOffset") == 0)
		{
			offset = atoi(value.c_str());
			
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
				anim = new Animation(name, filepath, length, width, height, offset, color, fr, 0, AnimationType::AT_LOOP, graphics->GetRenderer());
			}
			if (strcmp(value.c_str(), "once") == 0)
			{
				anim = new Animation(name, filepath, length, width, height, offset, color, fr, 0, AnimationType::AT_ONCE, graphics->GetRenderer());
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

void Actor::SetAnimation(Animation* anim)
{
	currentAnimation = anim;
	currentSprite = anim->GetSprite();
}
