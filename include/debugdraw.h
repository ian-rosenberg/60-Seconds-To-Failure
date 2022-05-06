#pragma once

#include <box2d/b2_draw.h>
#include <SDL.h>
#include <box2d/b2_body.h>
#include <string>

class DebugDraw : public b2Draw {
private:
	SDL_Renderer* ren;
	b2Vec2 bodyPosition;
	b2Body* bodyRef;
	int32 worldWidth, worldHeight;
	std::string objName;

public:

	DebugDraw(SDL_Renderer* r, const char* name);

	void SetWorldDimensions(b2Vec2 dim);
	void UpdateBodyPosition(b2Vec2 p);

	inline void SetBodyReference(b2Body* ref) { bodyRef = ref; }
	
	// Inherited via b2Draw
	void DrawRect(const SDL_Rect* rect); 
	void DrawRect(const b2Vec2* vertices, int32 vertexCount);

	
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	virtual void DrawTransform(const b2Transform& xf) override;
	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
};
