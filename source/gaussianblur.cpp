#include "gaussianblur.h"

#include <vectortypes.h>
#include <iostream>

void GaussianBlur::FilterCreate()
{
    int halfKernelSize = gKernel.size() / 2;
    double sigma = 1.0;
    double s = 2.0 * sigma * sigma;
    double r = 0.0;

    for (int y = -halfKernelSize; y <= halfKernelSize; y++) {
        for (int x = -halfKernelSize; x <= halfKernelSize; x++) {
            double r = sqrt(x * x + y * y);
            gKernel[y + halfKernelSize][x + halfKernelSize] = (exp(-(r * r) / s)) / (M_PI * s);
            kernelSum += gKernel[y + halfKernelSize][x + halfKernelSize];
        }
    }

    // normalising the Kernel 
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            gKernel[i][j] /= kernelSum;

            std::cout << gKernel[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

GaussianBlur::GaussianBlur(int kernelSize)
{
    
	gKernel.resize(kernelSize);

	for (int i = 0; i < kernelSize; i++)
		gKernel[i].resize(kernelSize);

    this->kernelSize = kernelSize;

    coverage = .667;//gf2d_random() * (kernelSize * kernelSize);

    kernelSum = 0;
}


GaussianBlur::~GaussianBlur()
{
    gKernel.clear();
}

void GaussianBlur::BlurTileMap(std::vector<std::vector<int>>& tileMap)
{
    std::vector<std::vector<int>> blurred;
    int tileMapWidth, tileMapHeight;
    int halfKernelSize = kernelSize / 2;
    tileMapHeight = tileMap.size();
    tileMapWidth = tileMap[0].size(); 

    FilterCreate();


    for (int gy = 0; gy < tileMapHeight; gy++) {
        std::vector<int> blurRow;
        for (int gx = 0; gx < tileMapWidth; gx++) {   
            bool edge = false;
            double sum = 0;
            for (int ty = -halfKernelSize; ty < halfKernelSize; ty++) {
                for (int tx = -halfKernelSize; tx < halfKernelSize; tx++) {
                    if(tx + gx >= 0 && tx + gx < tileMapWidth && ty + gy >=0 && ty + gy < tileMapHeight)
                        sum += tileMap[ty + gy][tx + gx] * gKernel[ty + halfKernelSize][tx + halfKernelSize];
                    else {
                        edge = true;
                        break;
                    }

                }
                if (edge)
                    break;
            }

            if (edge)
                sum = 1.0;
            else
                sum /= kernelSum;

            if (sum > coverage)
                blurRow.push_back(1);
            else
                blurRow.push_back(0);
        }
        blurred.push_back(blurRow);
    }

    for (int y = 0; y < tileMap.size(); y++)
        for (int x = 0; x < tileMap[y].size(); x++)
            tileMap[y][x] = blurred[y][x];
}
