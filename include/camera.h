#pragma once

#include <vectortypes.h>
#include <graphics.h>

class Camera {
private:
	SDL_Rect cameraRect;
	Vector4 bounds;
	float zRot;

public:
	Camera(SDL_Rect r, Vector4 bounds);

	void Move(Vector2 target, float lerp);

	inline SDL_Rect GetRect() { return cameraRect; }
	inline float GetRotationZ() { return zRot; }

	inline void SetBounds(Vector4 bounds) { this->bounds = bounds; }
};