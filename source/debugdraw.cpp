#include <debugdraw.h>
#include <iostream>

DebugDraw::DebugDraw(const std::shared_ptr<Graphics>& graphics, Camera* cam) {
	graphicsRef = graphics;
	camera = cam;
	mapRef = nullptr;
	worldWidth = worldHeight = camX = camY = camWidth = camHeight = 0;
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
	std::vector<SDL_Vertex> sdlVerts;
	SDL_Renderer* ren = graphicsRef->GetRenderer();

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);
	
	for (int i = 0; i < vertexCount; i++) {
		SDL_Vertex v;
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;
		v.color = SDL_Color(0, 255, 0, 255);
		v.position = { (float)a.x, (float)a.y };

		sdlVerts.push_back(v);
	}

	sdlVerts.push_back(sdlVerts[0]);

	for (int i = 0, j = 1; j < sdlVerts.size(); i++, j++)
		SDL_RenderDrawLine(ren, sdlVerts[i].position.x, sdlVerts[i].position.y, sdlVerts[j].position.x, sdlVerts[j].position.y);
	

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTriggerPolygon(b2Body* bodyRef, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, camX = camera->GetRect().x, camY = camera->GetRect().y;
	b2Vec2 t;
	Vector2 a = {},
		b = {};

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), color.r, color.g, color.b, 255);

	for (int i = 0, j = 1; j < vertexCount; i++, j++) {
		t = bodyRef->GetWorldPoint((*(vertices + i)));
		a = Vector2(t.x, t.y);
		graphicsRef->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;

		t = bodyRef->GetWorldPoint((*(vertices + j)));
		b = Vector2(t.x, t.y);
		graphicsRef->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;

		SDL_RenderDrawLine(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);
	}

	a = b;

	b.x -= camX;
	b.y -= camY;

	t = bodyRef->GetWorldPoint(*vertices);
	b = Vector2(t.x, t.y);
	graphicsRef->Vector2MetersToPixels(b);

	SDL_RenderDrawLine(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawRect(b2Body* bodyRef, const SDL_Rect* srcRect, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawRect(graphicsRef->GetRenderer(), srcRect);
	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
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

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), color.r, color.g, color.b, 255);

	for (int i = 0; i < vertexCount-1; i++) {
		p1 = bodyRef->GetWorldPoint( *(vertices + i));
		a = Vector2(p1.x, p1.y);
		graphicsRef->Vector2MetersToPixels(a);

		a.x -= camX;
		a.y -= camY;

		p2 = bodyRef->GetWorldPoint(*(vertices + i + 1));
		b = Vector2(p2.x, p2.y);
		graphicsRef->Vector2MetersToPixels(b);

		b.x -= camX;
		b.y -= camY;

		SDL_RenderDrawLineF(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);
	}
	
	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::AddEntityRef(Entity* entityRef)
{
	int id = entityRef->GetId();

	entityRefs.insert_or_assign(id, entityRef);
}

void DebugDraw::AddShapeRef(Tile* tile)
{
	SDL_Renderer* ren = graphicsRef->GetRenderer();
	b2Body* body = tile->GetBodyReference();
	int texID = tile->GetTextureID();

	if (shapeImages[texID] != nullptr)
		return;

	SDL_Texture* shapeTex = Sprite::CreateRenderTexture(mapRef->GetTileDimensions().x, mapRef->GetTileDimensions().y, graphicsRef, SDL_PIXELFORMAT_RGBA8888);
	SDL_SetTextureBlendMode(shapeTex, SDL_BLENDMODE_BLEND);

	SDL_RenderClear(ren);
	SDL_SetRenderTarget(ren, shapeTex);
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
		
		DrawPolygon(body, poly->m_vertices, poly->m_count, SDL_Color(0, 255, 0, 255));
	}
	SDL_SetRenderTarget(ren, nullptr);
	SDL_RenderClear(ren);

	
	shapeImages.insert_or_assign(texID, shapeTex);
}

void DebugDraw::AddTileMapRef(TileManager* tilemapRef)
{
	std::vector<std::vector<Tile*>>* tilemap = tilemapRef->GetTileMap();
	mapRef = tilemapRef;

	for (int y = 0; y < tilemap->size(); y++)
		for (int x = 0; x < tilemap->at(y).size(); x++)
			if (tilemap->at(y)[x] != nullptr && tilemap->at(y)[x]->GetBodyReference() != nullptr)
				AddShapeRef(tilemap->at(y)[x]);
}

void DebugDraw::DrawRect(b2Body* body, const b2Vec2* vertices, int32 vertexCount, const SDL_Color& color)
{
}

DebugDraw::~DebugDraw()
{
	camera = nullptr;
	entityRefs.clear();
	mapRef = nullptr;

	graphicsRef.reset();
}

void DebugDraw::DrawAll(float& accum, SDL_Rect camRect)
{
	SDL_Renderer* ren = graphicsRef->GetRenderer();
	std::vector<std::vector<Tile*>>* tileRefs = mapRef->GetTileMap();

	Vector2 pixelDimensions = mapRef->GetTileDimensions();
	camX = camera->GetRect().x;
	camY = camera->GetRect().y;
	camWidth = camera->GetRect().w;
	camHeight = camera->GetRect().h;

	for (int y = camY / pixelDimensions.y; y < (camY + camHeight) / pixelDimensions.y; y++) {
		for (int x = camX / pixelDimensions.x; x < (camX + camWidth) / pixelDimensions.x; x++) {
			if (!tileRefs->at(y)[x])
				continue;

			Tile* tile = tileRefs->at(y)[x];

			b2Body* body = tile->GetBodyReference();
			SDL_Color debugColor = tile->GetDebugColor();
			if (!body)
				continue;
		
			Vector2 bPos = { body->GetTransform().p.x, body->GetTransform().p.y };
			graphicsRef->Vector2MetersToPixels(bPos);
			int texID = tile->GetTextureID();
			SDL_Rect dest{
				(int)(bPos.x - camX),
				(int)(bPos.y - camY),
				(int)pixelDimensions.x,
				(int)pixelDimensions.y
			};

			SDL_RenderCopy(ren, shapeImages[texID], nullptr, &dest);
		}
	}

	for (auto entity : entityRefs) {
		Entity* thisEntity = entity.second;
		b2Body* body = thisEntity->GetBody();
		Vector2 ePos = thisEntity->GetDrawPosition();

		if (!body)
			continue;

		if (Vector2 ePos = thisEntity->GetDrawPosition(); !(ePos.x + thisEntity->GetPixelDimensions().x >= camRect.x)
			|| !(ePos.x < camRect.x + camRect.w)
			|| !(ePos.y + thisEntity->GetPixelDimensions().y >= camRect.y)
			|| !(ePos.y < camRect.y + camRect.h))
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


	graphicsRef->Vector2MetersToPixels(transformCenter);
	
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

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 255, 0, 0);
	SDL_RenderDrawLines(graphicsRef->GetRenderer(), circle.data(), circle.size());

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
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
	graphicsRef->Vector2MetersToPixels(a);

	a.x -= camX;
	a.y -= camY;

	b = Vector2(p2.x, p2.y);
	graphicsRef->Vector2MetersToPixels(b);

	b.x -= camX;
	b.y -= camY;

	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), color.r, color.g, color.b, 255);
	SDL_RenderDrawLineF(graphicsRef->GetRenderer(), a.x, a.y, b.x, b.y);
	SDL_SetRenderDrawColor(graphicsRef->GetRenderer(), 0, 0, 0, 0);
}

void DebugDraw::DrawTransform(b2Body* bodyRef, const b2Transform& xf)
{
}

void DebugDraw::DrawPoint(b2Body* bodyRef, const b2Vec2& p, float size, const SDL_Color& color)
{
}