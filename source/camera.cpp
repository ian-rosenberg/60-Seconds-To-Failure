#include "camera.h"

#include <cmath>
#include <iostream>
#include <algorithm>

Camera::Camera(Vector2 screenDim, Vector2 bounds ){
	cameraRect = SDL_Rect(0, 0, screenDim.x, screenDim.y);
	this->bounds = SDL_Rect(0, 0, bounds.x, bounds.y);
	zRot = 0;
	offset = Vector2(screenDim.x / 2, screenDim.y / 2);
}

void Camera::Move(Vector2 target, float alpha)
{
	cameraRect.x = cameraRect.x + alpha * (target.x - (cameraRect.x + cameraRect.w/2));
	cameraRect.y = cameraRect.y + alpha * (target.y - (cameraRect.y + cameraRect.h/2));

	cameraRect.x = std::clamp((int)cameraRect.x, 0, (int)(bounds.w - cameraRect.w));
	cameraRect.y = std::clamp((int)cameraRect.y, 0, (int)(bounds.h - cameraRect.h));
	//std::cout << cameraPosition.x << "," << cameraPosition.y << std::endl;
}
