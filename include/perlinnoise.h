#pragma once

#include <algorithm>
#include <vector>
#include <time.h>
#include <vectortypes.h>

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

	int									octaves = 5;
	float								scalingBias = 2.f;

public:
	PerlinNoise(Vector2 dimensions);

	~PerlinNoise();

	std::vector<float>					PerlinNoise1D();

	std::vector<std::vector<float>>		PerlinNoise2D();
};