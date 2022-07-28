#include "player.h"
#include <iomanip>

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(std::shared_ptr<Graphics> g)
{
	name = "";
	keys = 0;
	controller = NULL;
	sensitivity = 0;
	maxSpeed = 0.25f;
	dampening = 0.8f;
	dimensions = { 0,0,0 };
	enteredFrom = { 0,0 };
	axisLeftXLock = 0; 
	axisLeftYLock = 0;
	axisRightXLock = 0;
	axisRightYLock = 0;
	maxHealth = 100;
	health = maxHealth;
	maxEnergy = 50;
	energy = maxEnergy;
	jumpForce = .01;
	scale = { 1,1 };
	prevDrawPosition = newDrawPosition = { 0,0 };
	prevBodyPosition = newBodyPosition = { 0,0 };

	punching = false;
	graphics = g;
	LoadActor(actorFilePath.c_str());
	SetWorldDimensions(b2Vec2(avgDim.x * PIX_TO_MET, avgDim.y * PIX_TO_MET));
	currentAnimation = GetAnimationByName("idle");
	currentSprite = currentAnimation->GetSprite();
}

Player::~Player()
{
	InputEvent* e;
	while (!inputQueue->empty()) {
		e = inputQueue->front();
		inputQueue->erase(inputQueue->begin());

		delete e;
	}
	delete inputQueue;
	
	while (!eventsToFire->empty()) {
		e = eventsToFire->front();
		eventsToFire->pop();
		
		delete e;
	}
	delete eventsToFire;

	graphics = nullptr;
}

void Player::Think() {
	InputEvent* cEvent = nullptr;
	InputEvent* pEvent;
	InputType t;

	DecrementJumpTimer(graphics->GetFrameDeltaTime());

	while (!inputQueue->empty()) {
		pEvent = cEvent;
		cEvent = inputQueue->front();
		inputQueue->erase(inputQueue->begin());
		t = cEvent->inputType;

		if (pEvent && pEvent->inputType == t) {
			if (t == WALK_UP) {
				velocity.y = -maxSpeed;
				cEvent->data = &velocity;
				cEvent->onHold = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_DOWN) {
				velocity.y = maxSpeed;
				cEvent->data = &velocity;
				cEvent->onHold = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_LEFT) {
				velocity.x = -maxSpeed;
				cEvent->data = &velocity;
				cEvent->onHold = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_RIGHT) {
				velocity.x = maxSpeed;
				cEvent->data = &velocity;
				cEvent->onHold = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (IsGrounded()) {
				velocity.x = 0;
				cEvent->data = &velocity;
				cEvent->onRelease = std::bind(&Entity::SetVelocity, this, cEvent);
			}
		}
		else {
			if (t == WALK_UP) {
				velocity.x = velocity.x;
				velocity.y = -maxSpeed;
				cEvent->data = &velocity;
				cEvent->onPress = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_DOWN) {
				velocity.x = velocity.x;
				velocity.y = maxSpeed;
				cEvent->data = &velocity;
				cEvent->onPress = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_LEFT) {
				velocity.x = -maxSpeed;
				velocity.y = velocity.y;
				cEvent->data = &velocity;
				cEvent->onPress = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
			else if (t == WALK_RIGHT) {
				velocity.x = maxSpeed;
				velocity.y = velocity.y;
				cEvent->data = &velocity;
				cEvent->onRelease = std::bind(&Entity::SetVelocity, this, cEvent);				
			}
		}

		if (t == JUMP && IsGrounded()) {
			//ResetJumpTimer();
			cEvent->onPress = std::bind(&Entity::Jump, this, cEvent);
		}

		eventsToFire->push(cEvent);
	}
}

void Player::Draw()
{
	Vector4 debugColor = vector4(.5, 1, 0, 1);

	if (!this)
	{
		return;
	}
	
	if (IsGrounded())
		debugDraw->SetCollisionColor(1);

	if ((health / maxHealth) < 0.25f)
	{
		color = vector4(SDL_MAX_UINT8, 0, 0, SDL_MAX_UINT8);
	}
	else
	{
		color = vector4(SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8, SDL_MAX_UINT8);
	}

	scaleCenter = vector2(currentAnimation->GetCellWidth() / 2.0f,
		currentAnimation->GetCellHeight() / 2.0f);

	//vector2d_sub(resultPos, position, GetCameraPosition());

	currentSprite->Draw(currentSprite,
		newDrawPosition,
		&scale,
		&scaleCenter,
		&rotation,
		flip,
		&color,
		currentAnimation->GetCurrentFrame(),
		currentAnimation->GetYOffset(),
		currentAnimation->GetCellWidth(),
		currentAnimation->GetCellHeight());

	currentAnimation->AnimationNextFrame(currentAnimation);
}

void Player::UpdateScreenPosition(double alpha)
{
	Vector2 p;

	prevBodyPosition = newBodyPosition;
	newBodyPosition = body->GetPosition();

	p = { newBodyPosition.x, newBodyPosition.y };

	prevDrawPosition = newDrawPosition;

	graphics->Vector2MetersToPixels(p);
	newDrawPosition = p;
	newDrawPosition.x = newDrawPosition.x * alpha + prevDrawPosition.x * (1.0 - alpha);
	newDrawPosition.y = newDrawPosition.y * alpha + prevDrawPosition.y * (1.0 - alpha);

	//std::cout << GetAverageActorDimensions().x / 4 << std::endl;

	/*if(newBodyPosition.x != prevBodyPosition.x || newBodyPosition.y != prevBodyPosition.y)
		std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	if (newDrawPosition.x != prevDrawPosition.x || newDrawPosition.y != prevDrawPosition.y)
		std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;*/
}

void Player::Update()
{
	SetLogicalState(State::State_Idle);

	if (dead)
	{
		return;
	}

	if (abs(velocity.x) == abs(maxSpeed))
	{
		if (velocity.x < 0)
		{
			flip.x = 1;
		}
		else if (velocity.x > 0)
		{
			flip.x = 0;
		}
		SetLogicalState(State::State_Walking);
	}

	//std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	//std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;
	//std::cout << "Debug Rect: " << debugCircle.x << "," << debugCircle.y << "," << debugCircle.w << "," << debugCircle.h << std::endl;
	//std::cout << "X Velocity " << body->GetLinearVelocity().x << std::endl;

	rotation.z = body->GetAngle() * GF2D_RADTODEG;

	//set anim state
	if (logicalState != animState)
	{
		switch (logicalState)
		{
			case State::State_Walking:
				animState = logicalState;
				currentAnimation = GetAnimationByName("walk");
				currentSprite = currentAnimation->GetSprite();
				scaleCenter = vector2(currentAnimation->GetCellWidth() / 2, currentAnimation->GetCellHeight() / 2);
				break;

			case State::State_Idle:
				animState = logicalState;
				currentAnimation = GetAnimationByName("idle");
				currentSprite = currentAnimation->GetSprite();
				scaleCenter = vector2(currentAnimation->GetCellWidth() / 2, currentAnimation->GetCellHeight() / 2);
				break;

			case State::State_Attacking:
				animState = logicalState;
				currentAnimation = GetAnimationByName("attacking");
				currentSprite = currentAnimation->GetSprite();
				scaleCenter = vector2(currentAnimation->GetCellWidth() / 2, currentAnimation->GetCellHeight() / 2);
				break;

			default:
				break;
		}
	}
}

int Player::Touch(Entity* other)
{
	return 0;
}

void Player::Activate(Entity* activator)
{
}

int Player::Damage(int amount, Entity* source)
{
	return 0;
}

void Player::Die()
{
}
