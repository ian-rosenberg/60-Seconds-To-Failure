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
	Vector2 cameraMove{
		(target.x - cameraRect.w / 2.0) * alpha + cameraRect.x * (1.0 - alpha),
		(target.y - cameraRect.h / 2.0) * alpha + cameraRect.y * (1.0 - alpha)
	};

	if (bounds.z - cameraMove.x / 2.0
		|| bounds.w - cameraMove.y / 2.0)
		return;

	cameraRect.x = std::clamp(bounds.x, bounds.z, bounds.z - cameraMove.x/2.0);
	cameraRect.y = std::clamp(bounds.y, bounds.w, bounds.w - cameraMove.y/2.0);
	//std::cout << cameraPosition.x << "," << cameraPosition.y << std::endl;
}
