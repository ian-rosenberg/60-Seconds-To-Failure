#pragma once

#include <algorithm>
#include <vector>
#include "tile.h"

/*
Using OneLoneCoder's implementation
described @ https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PerlinNoise.cpp
*/
class PerlinNoise {
private:
	//2D Noise
	int									outputWidth;
	int									outputHeight;
	std::vector<std::vector<float>>		noiseSeed2D;
	std::vector<std::vector<float>>		perlin2D;

	//1D Noise
	std::vector<float>					noiseSeed1D;
	std::vector<float>					perlin1D;
	int									outputSize;

	int									octaves = 12;
	float								scalingBias = 2.f;

	std::shared_ptr<Graphics>			graphicsRef;

public:
	PerlinNoise(std::shared_ptr<Graphics> graphics);

	~PerlinNoise();

	std::vector<float> PerlinNoise1D(SDL_Color color);

	std::vector<std::vector<float>> PerlinNoise2D(SDL_Color color);
};