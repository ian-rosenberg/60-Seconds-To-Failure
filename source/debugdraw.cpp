#include <debugdraw.h>
#include <graphics.h>

DebugDraw::DebugDraw(SDL_Renderer* r) {
	ren = r;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	SDL_Vertex* verts = new SDL_Vertex[vertexCount];
	SDL_Vertex v;

	for (int i = 0; i < vertexCount; i++) {
		v.position.x = (*(vertices + i)).x;
		v.position.y = (*(vertices + i)).y;

		verts[i] = v;
	}
	SDL_SetRenderDrawColor(ren, color.r, 255, color.b, 255);
	SDL_RenderGeometry(ren, NULL, verts, vertexCount, NULL, 0);
	//SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
}

void DebugDraw::DrawRect(const SDL_Rect* srcRect)
{
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
	SDL_RenderDrawRect(ren, srcRect);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	SDL_Vertex v1, v2;

	v1.position.x = p1.x;
	v1.position.y = p1.y;

	v2.position.x = p1.x;
	v2.position.y = p1.y;

	SDL_SetRenderDrawColor(ren, color.r, 255, color.b, 255);
	SDL_RenderDrawLineF(ren, v1.position.x, v1.position.y, v2.position.x, v2.position.y);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}