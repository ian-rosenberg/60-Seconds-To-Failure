#include "perlinnoise.h"

PerlinNoise::PerlinNoise(std::shared_ptr<Graphics> graphics)
{
	graphicsRef = graphics;
	outputWidth = graphicsRef->GetScreenDimensions().x;
	outputHeight = graphicsRef->GetScreenDimensions().y;

	outputSize = outputWidth;

	noiseSeed1D.resize(outputSize);
	perlin1D.resize(outputSize);

	srand(time(nullptr));
}

PerlinNoise::~PerlinNoise()
{
	graphicsRef = nullptr;
}

std::vector<float> PerlinNoise::PerlinNoise1D(SDL_Color color)
{
	noiseSeed1D.clear();
	perlin1D.clear();

	noiseSeed1D.resize(outputSize);
	perlin1D.resize(outputSize);

	for (int col = 0; col < outputSize; col++) {
		noiseSeed1D[col] = abs(gf2d_crandom());
		perlin1D[col] = 0.f;
	}
	
	for (int x = 0; x < noiseSeed1D.size(); x++){
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octaves-1; o++)
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

	for (int x = 0; x < outputWidth; x++) {
		
			//std::cout << perlin2D[y][x] << ", ";
			SDL_Rect r = { x, floor(outputHeight * perlin1D[x]), 1, 1};
			SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
		//std::cout << std::endl;
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	return perlin1D;
}

std::vector<std::vector<float>> PerlinNoise::PerlinNoise2D(SDL_Color color)
{
	std::vector<float> noiseSeed2DRow;
	std::vector<float> perlin2DRow; 
	int larger = (outputWidth > outputHeight) ? outputWidth : outputHeight;
	float avg = 0.f;

	noiseSeed2D.clear();
	perlin2D.clear();

	for (int row = 0; row < outputHeight; row++) {
		noiseSeed2DRow.clear();
		perlin2DRow.clear();
		for (int col = 0; col < outputWidth; col++) {
			noiseSeed2DRow.push_back(abs(gf2d_crandom()));
			perlin2DRow.push_back(0.f);
		}
		perlin2D.push_back(perlin2DRow);
		noiseSeed2D.push_back(noiseSeed2DRow);
	}

	// Used 1D Perlin Noise
	for (int x = 0; x < outputWidth; x++) {
		for (int y = 0; y < outputHeight; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = scalingBias - 1.0 / (pow(scalingBias, octaves - 1) * (scalingBias - 1.0));
			float fScale = 1.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = outputWidth >> o;

				if (nPitch == 0)
					break;

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

			SDL_Rect r = { x,  y, 1, 1 };
			SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), color.r, color.g, color.b, perlin2D[y][x] * 255);
			SDL_RenderFillRect(graphicsRef->GetRenderer(), &r);
		}
		//std::cout << std::endl;
	}

	SDL_RenderPresent(graphicsRef->GetRenderer());

	return perlin2D;
}
