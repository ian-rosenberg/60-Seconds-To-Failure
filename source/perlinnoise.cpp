#include "perlinnoise.h"

PerlinNoise::PerlinNoise(std::shared_ptr<Graphics> graphics)
{
	std::vector<float> noiseSeed2DRow;
	std::vector<float> perlin2DRow;
	graphicsRef = graphics;
	outputWidth = graphicsRef->GetScreenDimensions().x/64;
	outputHeight = graphicsRef->GetScreenDimensions().y/64;

	srand(time(nullptr));
	for (int row = 0; row < outputHeight; row++) {
		noiseSeed2DRow.clear();
		perlin2DRow.clear();
		for (int col = 0; col < outputWidth; col++) {
			noiseSeed2DRow.push_back((float)rand() / (float)RAND_MAX);
			perlin2DRow.push_back(0.f);
		}
		perlin2D.push_back(perlin2DRow);
		noiseSeed2D.push_back(noiseSeed2DRow);
	}


	outputSize = outputWidth;
	noiseSeed1D.resize(outputSize);
	perlin1D.resize(outputSize);

	for(float val : noiseSeed1D)
		val = (float)rand() / (float)RAND_MAX;
}

PerlinNoise::~PerlinNoise()
{
	noiseSeed1D.clear();
	noiseSeed2D.clear();

	perlin1D.clear();
	perlin2D.clear();

	graphicsRef = nullptr;
}

std::vector<float> PerlinNoise::PerlinNoise1D()
{
	for (int x = 0; x < noiseSeed1D.size(); x++){
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = noiseSeed1D.size() >> o;
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % noiseSeed1D.size();

				float fBlend = (float)(x - nSample1) / (float)nPitch;

				float fSample = (1.0f - fBlend) * noiseSeed1D[nSample1] + fBlend * noiseSeed1D[nSample2];

				fScaleAcc += fScale;
				fNoise += fSample * fScale;
				fScale = fScale / scalingBias;
			}

			// Scale to seed range
			perlin1D[x] = fNoise / fScaleAcc;
	}

	return perlin1D;
}

std::vector<std::vector<float>> PerlinNoise::PerlinNoise2D()
{
	int larger = (outputWidth > outputHeight) ? outputWidth : outputHeight;
	float avg = 0.f;

	// Used 1D Perlin Noise
	for (int x = 0; x < outputWidth; x++) {
		for (int y = 0; y < outputHeight; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = outputWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;
				
				int nSampleX2 = (nSampleX1 + nPitch) % outputWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % outputHeight;
				
				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;
				
				float fSampleT = (1.0f - fBlendX) * noiseSeed2D[nSampleY1][nSampleX1] + fBlendX * noiseSeed2D[nSampleY1][nSampleX2];
				float fSampleB = (1.0f - fBlendX) * noiseSeed2D[nSampleY2][nSampleX1] + fBlendX * noiseSeed2D[nSampleY2][nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / scalingBias;
			}

			perlin2D[y][x] = fNoise / fScaleAcc;
			avg += fNoise / fScaleAcc;
		}
	}

	avg /= (outputWidth * outputHeight);

	for (int x = 0; x < outputWidth; x++) {
		for (int y = 0; y < outputHeight; y++)
		{
			if (perlin2D[y][x] < avg)
				continue;
			SDL_Rect r = { x * 64, y*64, 1*64, 1*64 };
			SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, perlin2D[y][x] * 255, 0, perlin2D[y][x]*255);
			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
		}
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	return perlin2D;
}
