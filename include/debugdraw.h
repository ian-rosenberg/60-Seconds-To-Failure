#pragma once

#include <SDL.h>
#include <box2d/b2_body.h>
#include <graphics.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <entity.h>
#include <camera.h>
#include <tile.h>

class DebugDraw {
private:
	std::shared_ptr<Graphics>			graphicsRef;
	std::unordered_map<int, Entity*>	entityRefs;
	std::vector<Tile*>					tileRefs;
	Camera*								camera;
	int32								worldWidth, worldHeight;
		
	// Inherited via b2Draw
	void DrawRect(b2Body* bodyRef, const SDL_Rect* rect, const SDL_Color& color);
	void DrawRect(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color);
	void DrawTriggerPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color);
	void DrawSolidPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color);
	void DrawChainShape(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, b2Vec2 gPrev, b2Vec2 gNext, const SDL_Color& color);

	void DrawPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color);
	void DrawCircle(b2Body* bodyRef, const b2Vec2& center, float radius, const SDL_Color& color);
	void DrawSolidCircle(b2Body* bodyRef, const b2Vec2& center, float radius, const b2Vec2& axis, const SDL_Color& color);
	void DrawSegment(b2Body* bodyRef, const b2Vec2& p1, const b2Vec2& p2, const SDL_Color& color);
	void DrawTransform(b2Body* bodyRef, const b2Transform& xf);
	void DrawPoint(b2Body* bodyRef, const b2Vec2& p, float size, const SDL_Color& color);

public:

	DebugDraw(const std::shared_ptr<Graphics>& graphicsr, Camera* cam);
	~DebugDraw();

	void SetWorldDimensions(b2Vec2 dim);
	void AddEntityRef(Entity* entityRef);
	void AddTileRef(Tile* tile) { tileRefs.push_back(tile); }
	void AddTileMapRef(std::vector<std::vector<Tile*>>* tilemapRef);
	void DrawAll(float &accum, SDL_Rect camRect);
};
