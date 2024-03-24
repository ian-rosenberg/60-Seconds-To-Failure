#pragma once

#include <vectortypes.h>
#include <graphics.h>

class Camera {
private:
	SDL_Rect cameraRect;
	SDL_Rect bounds;
	float zRot;
	Vector2 offset;

public:
	Camera(Vector2 screenDim, Vector2 bounds);

	void Move(Vector2 target, float lerp);

	inline SDL_Rect GetRect() { return cameraRect; }
	inline float GetRotationZ() { return zRot; }
};