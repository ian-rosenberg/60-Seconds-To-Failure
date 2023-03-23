#include "player.h"
#include <iomanip>

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(std::shared_ptr<Graphics> g) : Entity{ -1 }
{
	name = "Player";
	keys = 0;
	controller = nullptr;
	sensitivity = 0;
	maxSpeed = 25.f;
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
	jumpForce = 10.f;
	scale = { 1,1 };
	prevDrawPosition = newDrawPosition = { 0,0 };
	prevBodyPosition = newBodyPosition = { 0,0 };

	punching = false;
	graphics = g;
	LoadActor(actorFilePath.c_str());
	SetWorldDimensions(b2Vec2(avgDim.x * MET_IN_PIX, avgDim.y * MET_IN_PIX));
	currentAnimation = GetAnimationByName("idle");
	currentSprite = currentAnimation->GetSprite();
}

Player::~Player()
{
	InputEvent* e;
	while (inputQueue != nullptr && !inputQueue->empty()) {
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

	graphics.reset();
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

void Player::Draw(Vector2 cameraPosition)
{
	Vector4 debugColor = vector4(.5, 1, 0, 1);
	Vector2 resultPosition = { newDrawPosition.x - cameraPosition.x,
		newDrawPosition.y - cameraPosition.y };

	if (!this)
	{
		return;
	}

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

	currentSprite->Draw(currentSprite,
		resultPosition,
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

	//std::cout << resultPosition.x << "," << resultPosition.y << std::endl;
	//std::cout << "Player Velocity (" << velocity.x << ", " << velocity.y << ")" << std::endl;
}



void Player::Update()
{
	b2Vec2 bodyVelocity = body->GetLinearVelocity();
		
	SetLogicalState(State::State_Idle);

	if (dead)
	{
		return;
	}
	
	if (abs(bodyVelocity.x) > .01f)
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
