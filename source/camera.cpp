#include "camera.h"

#include <cmath>
#include <iostream>

Camera::Camera(SDL_Rect r, Vector4 bounds) {
	cameraRect = r;
	this->bounds = bounds;
	zRot = 0;
}

void Camera::Move(Vector2 target, float l)
{
	cameraRect.x = std::lerp(cameraRect.x, target.x - cameraRect.w / 2, l);
	cameraRect.y = std::lerp(cameraRect.y, target.y - cameraRect.h / 2, l);


	//std::cout << cameraPosition.x << "," << cameraPosition.y << std::endl;
}
