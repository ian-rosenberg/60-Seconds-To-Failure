#include "camera.h"

#include <cmath>
#include <iostream>
#include <algorithm>

Camera::Camera(SDL_Rect r, Vector4 bounds ){
	cameraRect = r;
	this->bounds = bounds;
	zRot = 0;
}

void Camera::Move(Vector2 target, float alpha)
{
	cameraRect.x = (target.x - cameraRect.w / 2) * alpha + cameraRect.x * (1.0 - alpha);
	cameraRect.y = (target.y - cameraRect.h / 2) * alpha + cameraRect.y * (1.0 - alpha);


	cameraRect.x = std::clamp(cameraRect.x * 1.0, bounds.x, bounds.z - cameraRect.w);
	cameraRect.y = std::clamp(cameraRect.y * 1.0, bounds.y, bounds.w - cameraRect.h);
	//std::cout << cameraPosition.x << "," << cameraPosition.y << std::endl;
}
