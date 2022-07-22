#include <debugdraw.h>
#include <graphics.h>
#include <iostream>
#include <vector>

DebugDraw::DebugDraw(std::shared_ptr<Graphics> gr, const char* name, Vector2 pDim) {
	graphicsRef = std::shared_ptr<Graphics>(gr);
	bodyPosition = b2Vec2(0, 0);
	bodyRef = nullptr;
	objName = name;
	isColliding = 0;
	dR = dB = 0;
	dG = 255;
	pixelDimensions = pDim;
}

void DebugDraw::UpdateBodyPosition(b2Vec2 p)
{
	bodyPosition = p;
	if (isColliding) {
		dR = 255;
		dG = 0;
	}
	else {
		dR = 0;
		dG = 255;
	}
}

void DebugDraw::SetWorldDimensions(b2Vec2 dim) {
	worldHeight = dim.y;
	worldWidth = dim.x;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	//std::cout << "Object: " << objName << std::endl;

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 255, 0, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(a);

		//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x1 << "," << y1 << std::endl;

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		b = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(b);
		//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x2 << "," << y2 << std::endl;

		SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}

	a = b;


	t = bodyRef->GetWorldPoint(*vertices);
	b = Vector2(t.x, t.y);
	graphicsRef.get()->Vector2MetersToPixels(b);
	//std::cout << "Transform vertex " << t.x << "," << t.y << "\tto\t" << x2 << "," << y2 << std::endl;


	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTriggerPolygon(const b2Vec2* vertices, int32 vertexCount)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), dR, dG, dB, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(a);

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		b = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(b);

		SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}

	a = b;


	t = bodyRef->GetWorldPoint(*vertices);
	b = Vector2(t.x, t.y);
	graphicsRef.get()->Vector2MetersToPixels(b);

	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawRect(const SDL_Rect* srcRect)
{
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawRect(graphicsRef.get()->GetRenderer(), srcRect);
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void DebugDraw::DrawChainShape(const b2Vec2* vertices, int32 vertexCount, b2Vec2 gPrev, b2Vec2 gNext)
{
	Vector2 a = {},
		b = {};
	b2Vec2 p = bodyRef->GetWorldPoint(gPrev);
	b2Vec2 bp = bodyRef->GetPosition();

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), dR, dG, dB, 255);

	a = { p.x, p.y};
	p -= bp;
	graphicsRef.get()->Vector2MetersToPixels(a);
	p = bodyRef->GetWorldPoint(*vertices);
	p -= bp;
	b = { p.x, p.y};
	graphicsRef.get()->Vector2MetersToPixels(b);

	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);

	for (int i = 0; i < vertexCount-1; i++) {
		p = bodyRef->GetWorldPoint( *(vertices + i));
		p -= bp;
		a = Vector2(p.x, p.y);
		graphicsRef.get()->Vector2MetersToPixels(a);
		p = bodyRef->GetWorldPoint(*(vertices + i + 1));
		p -= bp;
		b = Vector2(p.x, p.y);
		graphicsRef.get()->Vector2MetersToPixels(b);

		SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}


	p = bodyRef->GetWorldPoint(gNext);
	p -= bp;
	a = { p.x, p.y };
	graphicsRef.get()->Vector2MetersToPixels(a);
	b = { vertices[vertexCount-1].x , vertices[vertexCount-1].y };
	graphicsRef.get()->Vector2MetersToPixels(b);

	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawCircle(const b2Vec2& c, float radius, const b2Color& color)
{
	float pih = M_PI / 2.0; //half of pi
	uint8_t sides = 16;
	float step = 2 * M_PI / sides;
	b2Vec2 center = bodyRef->GetWorldPoint(c);
	Vector2 transformCenter = { center.x, center.y };
	std::vector<SDL_Point> circle;
	SDL_Point b, pt;


	graphicsRef.get()->Vector2MetersToPixels(transformCenter);
	
	//16 sides
	float d_a = 2 * M_PI / 16,
		angle = d_a;
	
	b.x = transformCenter.x + radius * cos(0);
	b.y = transformCenter.y - radius * sin(0);

	for (float theta = 0; theta < 2 * M_PI; theta += step)
	{
		pt.x = transformCenter.x + radius * cos(theta);
		pt.y = transformCenter.y - radius * sin(theta);
		circle.push_back(pt);
	}

	circle.push_back(b);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 255, 0, 0);
	SDL_RenderDrawLines(graphicsRef.get()->GetRenderer(), circle.data(), circle.size());

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{

}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	SDL_Vertex v1,
		v2;
	Vector2 a = {},
		b = {};

	a = Vector2(p1.x, p1.y);
	graphicsRef.get()->Vector2MetersToPixels(a);
	b = Vector2(p2.x, p2.y);
	graphicsRef.get()->Vector2MetersToPixels(b);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), dR, dG, dB, 255);
	SDL_RenderDrawLineF(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}