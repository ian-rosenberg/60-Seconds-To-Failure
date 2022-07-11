#pragma once

#include <box2d/b2_draw.h>
#include <SDL.h>
#include <box2d/b2_body.h>
#include <string>
#include <memory>
#include <graphics.h>

class DebugDraw : public b2Draw {
private:
	std::shared_ptr<Graphics> graphicsRef;
	b2Vec2 bodyPosition;
	b2Body* bodyRef;
	b2Fixture* triggerFix;
	int32 worldWidth, worldHeight;
	std::string objName;
	Uint8 isColliding;
	Uint8 dR, dG, dB;

public:

	DebugDraw(std::shared_ptr<Graphics> gr, const char* name);

	void SetWorldDimensions(b2Vec2 dim);
	void UpdateBodyPosition(b2Vec2 p);
	void SetCollisionColor(Uint8 on) { isColliding = on; }

	inline void SetBodyReference(b2Body* ref) { bodyRef = ref; }
	inline void SetTriggerFixture(b2Fixture* ref) { triggerFix = ref; }
	inline b2Fixture* GetTrigger() { return triggerFix; }
	
	// Inherited via b2Draw
	void DrawRect(const SDL_Rect* rect); 
	void DrawRect(const b2Vec2* vertices, int32 vertexCount);
	void DrawTriggerPolygon(const b2Vec2* vertices, int32 vertexCount);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);


	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	virtual void DrawTransform(const b2Transform& xf) override;
	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
};
