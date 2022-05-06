#include <debugdraw.h>
#include <graphics.h>
#include <iostream>

DebugDraw::DebugDraw(SDL_Renderer* r, const char* name) {
	ren = r;
	bodyPosition = b2Vec2(0, 0);
	bodyRef = nullptr;
	objName = name;
}

void DebugDraw::UpdateBodyPosition(b2Vec2 p)
{
	bodyPosition = p;
}

void DebugDraw::SetWorldDimensions(b2Vec2 dim) {
	worldHeight = dim.y;
	worldWidth = dim.x;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	b2Vec2 t;

	//std::cout << "Object: " << objName << std::endl;

	
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		x1 = ((SCALED_WIDTH / 2.0f) + t.x) * MET_TO_PIX;
		y1 = ((SCALED_HEIGHT / 2.0f) + t.y) * MET_TO_PIX;

		//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x1 << "," << y1 << std::endl;

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		x2 = ((SCALED_WIDTH / 2.0f) + t.x) * MET_TO_PIX;
		y2 = ((SCALED_HEIGHT / 2.0f) + t.y) * MET_TO_PIX;

		//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x2 << "," << y2 << std::endl;

		SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}

	x1 = x2;
	y1 = y2;


	t = bodyRef->GetWorldPoint(*vertices);
	x2 = ((SCALED_WIDTH / 2.0f) + t.x) * MET_TO_PIX;
	y2 = ((SCALED_HEIGHT / 2.0f) + t.y) * MET_TO_PIX;

	//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x2 << "," << y2 << std::endl;


	SDL_RenderDrawLine(ren, x1, y1, x2, y2);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
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


	v1.position.x = ((SCALED_WIDTH / 2.0f) + bodyRef->GetWorldPoint(p1).x) * MET_TO_PIX;
	v1.position.y = ((SCALED_HEIGHT / 2.0f) + bodyRef->GetWorldPoint(p1).y) * MET_TO_PIX;

	v2.position.x = ((SCALED_WIDTH / 2.0f) + bodyRef->GetWorldPoint(p2).x) * MET_TO_PIX;
	v2.position.y = ((SCALED_HEIGHT / 2.0f) + bodyRef->GetWorldPoint(p1).y) * MET_TO_PIX;

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