#include <debugdraw.h>
#include <iostream>

DebugDraw::DebugDraw(std::shared_ptr<Graphics> gr, Camera* cam) {
	graphicsRef = std::shared_ptr<Graphics>(gr);
	camera = cam;
}

void DebugDraw::SetWorldDimensions(b2Vec2 dim) {
	worldHeight = dim.y;
	worldWidth = dim.x;
}

void DebugDraw::DrawPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, camX = camera->GetRect().x, camY = camera->GetRect().y;
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 255, 0, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		b = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;

		SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}

	a = b;


	t = bodyRef->GetWorldPoint(*vertices);
	b = Vector2(t.x, t.y);
	graphicsRef.get()->Vector2MetersToPixels(b);

	b.x -= camX;
	b.y -= camY;
	
	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTriggerPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, camX = camera->GetRect().x, camY = camera->GetRect().y;
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), color.r, color.g, color.b, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		b = Vector2(t.x, t.y);
		graphicsRef.get()->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;

		SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}

	a = b;

	b.x -= camX;
	b.y -= camY;

	t = bodyRef->GetWorldPoint(*vertices);
	b = Vector2(t.x, t.y);
	graphicsRef.get()->Vector2MetersToPixels(b);

	SDL_RenderDrawLine(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawRect(b2Body* bodyRef, const SDL_Rect* srcRect, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawRect(graphicsRef.get()->GetRenderer(), srcRect);
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawSolidPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
}

void DebugDraw::DrawChainShape(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, b2Vec2 gPrev, b2Vec2 gNext, const SDL_Color& color)
{
	Vector2 a, b;
	b2Vec2 p1;
	b2Vec2 p2;
	int camX = camera->GetRect().x, camY = camera->GetRect().y;

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), color.r, color.g, color.b, 255);

	for (int i = 0; i < vertexCount-1; i++) {
		p1 = bodyRef->GetWorldPoint( *(vertices + i));
		a = Vector2(p1.x, p1.y);
		graphicsRef.get()->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;

		p2 = bodyRef->GetWorldPoint(*(vertices + i + 1));
		b = Vector2(p2.x, p2.y);
		graphicsRef.get()->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;

		SDL_RenderDrawLineF(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	}
	
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::AddEntityRef(Entity* entityRef)
{
	int id = entityRef->GetId();

	entityRefs.insert_or_assign(id, entityRef);
}

void DebugDraw::DrawRect(b2Body* body, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
}

DebugDraw::~DebugDraw()
{
	camera = nullptr;
	entityRefs.clear();
	tileRefs.clear();
}

void DebugDraw::DrawAll()
{
	for (auto tile : tileRefs) {
		Tile* thisTile = tile;
		b2Body* body = thisTile->GetBodyReference();
		SDL_Color debugColor = thisTile->GetDebugColor();
		if (!body)
			continue;


		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2Shape::Type shapeType = f->GetType();

			if (shapeType == b2Shape::e_polygon) {
				b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

				DrawPolygon(body, poly->m_vertices, poly->m_count, debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_edge) {
				b2EdgeShape* edge = (b2EdgeShape*)f->GetShape();

				DrawSegment(body, edge->m_vertex1, edge->m_vertex2, debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_chain) {
				b2ChainShape* chain = (b2ChainShape*)f->GetShape();

				DrawChainShape(body, chain->m_vertices, chain->m_count, *(chain->m_vertices), *(chain->m_vertices + chain->m_count - 1), debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_circle) {
				b2CircleShape* poly = (b2CircleShape*)f->GetShape();

				DrawCircle(body, poly->m_p, poly->m_radius * PIX_IN_MET, debugColor);
				continue;
			}
		}

	}

	for (auto entity : entityRefs) {
		Entity* thisEntity = entity.second;
		b2Body* body = thisEntity->GetBody();
		if (!body)
			continue;

		
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			SDL_Color debugColor = thisEntity->GetDebugColor();
			b2Shape::Type shapeType = f->GetType();

			if (shapeType == b2Shape::e_polygon) {
				b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

				DrawPolygon(body, poly->m_vertices, poly->m_count, debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_edge) {
				b2EdgeShape* edge = (b2EdgeShape*)f->GetShape();

				DrawSegment(body, edge->m_vertex1, edge->m_vertex2, debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_chain) {
				b2ChainShape* chain = (b2ChainShape*)f->GetShape();

				DrawChainShape(body, chain->m_vertices, chain->m_count, *(chain->m_vertices), *(chain->m_vertices + chain->m_count-1), debugColor);

				continue;
			}

			else if (shapeType == b2Shape::e_circle) {
				b2CircleShape* poly = (b2CircleShape*)f->GetShape();

				DrawCircle(body, poly->m_p, poly->m_radius * PIX_IN_MET, debugColor);
				continue;
			}
		}

	}
}

void DebugDraw::DrawCircle(b2Body* bodyRef, const b2Vec2& c, float radius, const SDL_Color& color)
{
	int camX = camera->GetRect().x, camY = camera->GetRect().y;
	float pih = M_PI / 2.0; //half of pi
	uint8_t sides = 16;
	float step = 2 * M_PI / sides;
	b2Vec2 center = bodyRef->GetWorldPoint(c);
	Vector2 transformCenter = { center.x, center.y };
	std::vector<SDL_Point> circle;
	SDL_Point b, pt;


	graphicsRef.get()->Vector2MetersToPixels(transformCenter);
	
	transformCenter.x -= camX;
	transformCenter.y -= camY;

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

void DebugDraw::DrawSolidCircle(b2Body* bodyRef, const b2Vec2& center, float radius, const b2Vec2& axis, const SDL_Color& color)
{

}

void DebugDraw::DrawSegment(b2Body* bodyRef, const b2Vec2& p1, const b2Vec2& p2, const SDL_Color& color)
{
	int camX = camera->GetRect().x, camY = camera->GetRect().y;
	Vector2 a = {},
		b = {};

	a = Vector2(p1.x, p1.y);
	graphicsRef.get()->Vector2MetersToPixels(a);

	a.x -= camX;
	a.y -= camY;

	b = Vector2(p2.x, p2.y);
	graphicsRef.get()->Vector2MetersToPixels(b);

	b.x -= camX;
	b.y -= camY;

	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), color.r, color.g, color.b, 255);
	SDL_RenderDrawLineF(graphicsRef.get()->GetRenderer(), a.x, a.y, b.x, b.y);
	SDL_SetRenderDrawColor(graphicsRef.get()->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTransform(b2Body* bodyRef, const b2Transform& xf)
{
}

void DebugDraw::DrawPoint(b2Body* bodyRef, const b2Vec2& p, float size, const SDL_Color& color)
{
}