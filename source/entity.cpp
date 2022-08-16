#include "entity.h"

Entity::Entity()
{
	id = -1;
	gravityEnabled = 0;
	dampening = 1.0f;
	logicalState = State::State_Idle;
	worldDimensions = {};
	newDrawPosition = vector2(0, 0);
	velocity = { 0,0 };
	prevDrawPosition = newDrawPosition = { 0,0 };
	prevBodyPosition = newBodyPosition = { 0,0 };
	scale = { 0,0 };
	scaleCenter = { 0,0 };
	rotation = { 0,0,0 };
	flip = { 0,0 };
	facing = { 0,0 };
	maxHealth = 0;
	health = 0;
	jumpTimer = 0.0f;
	grounded = false;
	energy = 0;
	maxEnergy = 0;
	maxSpeed = 0;
	dead = 0;
	jumpForce = 1.0;
	body = NULL;
	jumpTrigger = NULL;
	parentEntity = NULL;
	debugDraw = NULL;
	interpComponent = new PhysicsComponent{ {0,0}, 0.0f, {0,0}, 0.0f };
}

Entity::~Entity()
{
	if (animations) {
		while (!animations->empty()) {
			Animation* anim = animations->back();
			animations->pop_back();
			delete anim;
		}
	}

	if (debugDraw)
		delete debugDraw;

	if (currentAnimation)
		currentAnimation = NULL;
	
	if (currentSprite)
		currentSprite = NULL;

	if (interpComponent)
		delete interpComponent;
}
void Entity::SetAnimationByName(const char* name)
{
	std::vector<Animation*>* animations = GetAnimations();

	
	for (int i = 0; i < animations->size(); i++)
	{
		if (strcmp(animations->at(i)->GetName().c_str(), name) == 0) {
			SetAnimation(animations->at(i));
			return;
		}
	}
}

void Entity::SetJumpTrigger(b2Fixture* f) {
	jumpTrigger = f;
}

Animation* Entity::GetAnimationByName(const char* name)
{
	std::vector<Animation*>* animations = GetAnimations();
		
	for (int i = 0; i < animations->size(); i++)
	{
		if (strcmp(animations->at(i)->GetName().c_str(), name) == 0) {
			return animations->at(i);
		}
	}

	return NULL;
}

void Entity::SetVelocity(InputEvent* e)
{
	Vector2* vlcty = (Vector2*)e->data;
	velocity.x = vlcty->x;
	velocity.y = body->GetLinearVelocity().y;
	b2Vec2 v = b2Vec2(velocity.x, velocity.y);
	body->SetLinearVelocity(v);
}

void Entity::SetPreviousPhysicsState()
{
	interpComponent->prevAngle = rotation.z;
	interpComponent->prevPosition = newBodyPosition;
}

void Entity::RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
	b2Vec2 tmp;
	tmp.x = vector.x * cos(angle) - vector.y * sin(angle);
	tmp.y = vector.x * sin(angle) + vector.y * cos(angle);
	vector = tmp + center;
}

void Entity::SetLogicalState(State state)
{
	logicalState = state;
}

void Entity::SetId(int newId)
{
	id = newId;
}

void Entity::SetBody(b2Body* b) {
	if (!b) {
		std::cout << "Body is NULL." << std::endl;

		return;
	}
	
	body = b;
	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(interpComponent);
}

void Entity::SetWorldDimensions(b2Vec2 dim) {
	worldDimensions.Set(dim.x, dim.y);
}

void Entity::Jump(InputEvent* e)
{
	if (!jumpTrigger || !grounded)
		return;

 	body->ApplyLinearImpulse(b2Vec2(0, -body->GetMass() * jumpForce), body->GetWorldCenter(), false);
	this->ToggleGrounded(false);
}

void Entity::ToggleGrounded(int flag)
{
	debugDraw->SetCollisionColor(flag);

	grounded = flag == 1;
}

void Entity::UpdateScreenPosition(double alpha)
{
	Vector2 p;
	//b2Vec2 bPos = body->GetPosition();

	prevBodyPosition = newBodyPosition;
	newBodyPosition = interpComponent->smoothedPosition;

	//std::cout << "Smoothed Body Position: ( " << interpComponent->smoothedPosition.x << "," << interpComponent->smoothedPosition.y << "," << std::endl;
	//std::cout << "Box2D Body Position: ( " << bPos.x << "," << bPos.y << ")" << std::endl << std::endl;
	
	p = { newBodyPosition.x, newBodyPosition.y };

	prevDrawPosition = newDrawPosition;

	graphics->Vector2MetersToPixels(p);
	newDrawPosition = p;
	//newDrawPosition.x = newDrawPosition.x * alpha + prevDrawPosition.x * (1.0 - alpha);
	//newDrawPosition.y = newDrawPosition.y * alpha + prevDrawPosition.y * (1.0 - alpha);

	//std::cout << GetAverageActorDimensions().x / 4 << std::endl;

	/*if(newBodyPosition.x != prevBodyPosition.x || newBodyPosition.y != prevBodyPosition.y)
		std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	if (newDrawPosition.x != prevDrawPosition.x || newDrawPosition.y != prevDrawPosition.y)
		std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;*/
}

EntityManager::EntityManager() {
	entities = new std::vector<Entity*>();
	inputQueue = new std::vector<Entity::InputEvent*>();
	eventsToFire = new std::queue<Entity::InputEvent*>();
	debugDraw = 0;
	graphics = nullptr;
}

EntityManager::EntityManager(Uint8 debugFlag, std::shared_ptr<Graphics> g)
{
	entities = new std::vector<Entity*>();
	inputQueue = new std::vector<Entity::InputEvent*>();
	eventsToFire = new std::queue<Entity::InputEvent*>();
	debugDraw = debugFlag;
	graphics = std::shared_ptr<Graphics>(g);
}

EntityManager::~EntityManager()
{
	if (entities) {
		while (!entities->empty()) {
			Entity* e = entities->back();
			entities->pop_back();
			delete e;
		}
	}

	if (!inputQueue->empty())
		ClearInputEventQueue();

	delete inputQueue;

	if (!eventsToFire->empty())
		ClearEventsToFireQueue();

	delete eventsToFire;
}

void EntityManager::AddEntity(Entity* ent)
{
	if (debugDraw) {
		ent->EnableDebugDraw(new DebugDraw(graphics, ent->GetActorName(), ent->GetAvgPixelDimensions()));
		ent->GetDebugDraw()->SetBodyReference(ent->GetBody());
		ent->GetDebugDraw()->SetWorldDimensions(ent->GetWorldDimensions());
		if(ent->GetJumpTrigger())
			ent->GetDebugDraw()->SetTriggerFixture(ent->GetJumpTrigger());

	}

	entities->push_back(ent);
	ent->SetId(entities->size());
}

Entity* EntityManager::DeleteEntity(Entity* ent)
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		if (*it == ent) {
			Entity* ent = *it;
			entities->erase(it);

			return ent;
		}
	}
}

void EntityManager::EntityDrawAll(double alpha)
{
	SDL_Rect r;
	Vector2 p, dim;


	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{

		(*it)->UpdateScreenPosition(alpha);
		(*it)->Draw();

		//Debug Drawing
		if ((*it)->GetDebugDrawEnabled()) {
			DebugDrawing(*it);
		}
	}
}

void EntityManager::DebugDrawing(Entity* it)
{
	Vector2 drawPos = it->GetScreenPosition();
	it->GetDebugDraw()->UpdateBodyPosition(b2Vec2(drawPos.x, drawPos.y));

	for (b2Fixture* f = it->GetBody()->GetFixtureList(); f; f = f->GetNext()) {
		b2Shape::Type shapeType = f->GetType();

		if (f == it->GetJumpTrigger()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			it->GetDebugDraw()->DrawTriggerPolygon(poly->m_vertices, poly->m_count);
			continue;
		}

		if (shapeType == b2Shape::e_polygon) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			it->GetDebugDraw()->DrawPolygon(poly->m_vertices, poly->m_count, b2Color(0,1.0f, 0));
			continue;
		}

		else if (shapeType == b2Shape::e_edge) {
			b2EdgeShape* edge = (b2EdgeShape*)f->GetShape();

			it->GetDebugDraw()->DrawSegment(edge->m_vertex1, edge->m_vertex2, b2Color(0, 1.0f, 0));
	
			continue;
		}

		else if (shapeType == b2Shape::e_circle) {
			b2CircleShape* poly = (b2CircleShape*)f->GetShape();
			it->GetDebugDraw()->DrawCircle(poly->m_p, poly->m_radius * MET_TO_PIX, b2Color(0, 1.0f, 0));
			continue;
		}
	}
}

void EntityManager::EntityUpdateAll(double ticks)
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->SetPreviousPhysicsState();
		(*it)->Update();
	}
}

void EntityManager::EntityThinkAll()
{
	for (std::vector<Entity*>::iterator it = entities->begin();
		it != entities->end();
		it++)
	{
		(*it)->Think();
	}
}

void EntityManager::InputUpdate()
{
	Entity::InputEvent* prev;
	Entity::InputEvent* cur;
	Uint8 gravityEnabled = 0;

	while (!eventsToFire->empty()) {

		cur = eventsToFire->front();
		eventsToFire->pop();

		gravityEnabled = cur->gravity;
		prev = cur->prevEvent ? cur->prevEvent : nullptr;
		
		if(cur->onHold)
			cur->onHold(cur);
		else if (cur->onPress)
			cur->onPress(cur);
		else if (cur->onRelease)
			cur->onRelease(cur);
		
		delete cur;
	}
}

void EntityManager::ClearInputEventQueue()
{
	Entity::InputEvent* e = NULL;

	while (!inputQueue->empty()) {
		e = inputQueue->at(inputQueue->size() - 1);
		inputQueue->pop_back();
		delete e;
	}
}

void EntityManager::ClearEventsToFireQueue()
{
	Entity::InputEvent* e = NULL;

	while (!eventsToFire->empty()) {
		e = eventsToFire->front();
		eventsToFire->pop();
		delete e;
	}
}

Entity::InputEvent* EntityManager::PeekInputEventAtIndex(Uint16 index)
{
	if (index < inputQueue->size() && index >= 0)
		return inputQueue->at(index);
	else if (index == -1)
		return inputQueue->at(inputQueue->size() - 1);
	else
		return nullptr;
}

Entity::InputEvent* EntityManager::GetNextInputEvent()
{
	Entity::InputEvent* e = PeekInputEventAtIndex(0);

	if (e)
		inputQueue->erase(inputQueue->begin());

	return e ? e : nullptr;
}
