#pragma once
#include <vector>

class GaussianBlur{
private:
	double kernelSum;
	double coverage;
	int kernelSize;
	std::vector<std::vector<double>> gKernel;

	void FilterCreate();

public:
	GaussianBlur(int kernelSize);
	~GaussianBlur();

	void BlurTileMap(std::vector<std::vector<int>>& tileMap);
};

