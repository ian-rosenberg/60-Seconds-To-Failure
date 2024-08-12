#include "player.h"
#include <iomanip>

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(const std::shared_ptr<Graphics>& graphics) : Entity{ -1 }
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
	jumpForce = 27.5f;
	scale = { 1,1 };
	prevDrawPosition = newDrawPosition = resultPosition = { 0,0 };
	prevBodyPosition = newBodyPosition = { 0,0 };

	punching = false;
	animActor =	LoadActor(actorFilePath.c_str(), graphics);
	pixelDimensions = animActor->GetAvgDimensions();
	SetWorldDimensions(b2Vec2(pixelDimensions.x * MET_IN_PIX, pixelDimensions.y * MET_IN_PIX));
	SetLogicalState(State::State_Idle);
	animActor->SetAnimationState(State::State_Falling);
	currentEvent = nullptr;
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

	currentEvent = nullptr;
}

void Player::Think() {
	InputEvent* pEvent;
	InputType t;

	while (!inputQueue->empty()) {
		pEvent = currentEvent;
		currentEvent = inputQueue->front();
		inputQueue->erase(inputQueue->begin());
		t = currentEvent->inputType;
		currentEvent->repeat = currentEvent->e->key.repeat;

		if (IsGrounded() && animActor->GetAnimationState() != State::State_Landing) {

			if (currentEvent->repeat > 0) {
				if (t == WALK_UP) {
					velocity.y = -maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onHold = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_DOWN) {
					velocity.y = maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onHold = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_LEFT) {
					velocity.x = -maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onHold = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_RIGHT) {
					velocity.x = maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onHold = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
			}
			else {
				if (t == WALK_UP) {
					velocity.x = velocity.x;
					velocity.y = -maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onPress = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_DOWN) {
					velocity.x = velocity.x;
					velocity.y = maxSpeed;
					currentEvent->data = &velocity;
					currentEvent->onPress = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_LEFT) {
					velocity.x = -maxSpeed;
					velocity.y = velocity.y;
					currentEvent->data = &velocity;
					currentEvent->onPress = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
				else if (t == WALK_RIGHT) {
					velocity.x = maxSpeed;
					velocity.y = velocity.y;
					currentEvent->data = &velocity;
					currentEvent->onPress = std::bind(&Entity::SetVelocity, this, currentEvent);
				}
			}
		}


 		if (t == JUMP && jumpTimer <= 0 && jumpCount < jumpMax) {
			ResetJumpTimer();
  			jumpCount++;
			currentEvent->onPress = std::bind(&Entity::Jump, this, currentEvent);
  			jumpTimer = jumpCooldown;
		}

		eventsToFire->push(currentEvent);
	}
}

void Player::Draw(Vector2 cameraPosition){	
	if (!this)
	{
		return;
	}

	Entity::Draw(cameraPosition);

}



void Player::Update()
{
	this->Entity::Update();

	if (IsGrounded()) {
		ResetJumpTimer();
		jumpCount = 0;
	}

	jumpTimer -= SDL_GetTicks64();

	//If anim in progress
	//if (currentAnimation->InProgress()) {
	//	//jumping/falling
	//	if ((int)bodyVelocity.y != 0 && animState != State::State_Jumping) {
	//		if (bodyVelocity.y > 0) {
	//			logicalState = State::State_Jumping;
	//		}
	//		else if(bodyVelocity.y < 0 && animState == State::State_Jumping){
	//			logicalState = State::State_Falling;
	//		}
	//		else if ((int)bodyVelocity.y == 0 && animState == State::State_Falling) {
	//			logicalState = State::State_Landing;
	//		}
	//	}	
	//}
	//else {
	//	if ((int)bodyVelocity.y > 0 && animState != State::State_Jumping)
	//		logicalState = State::State_Jumping;
	//	else
	//		logicalState = State::State_Idle;
	//}

	//std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	//std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;
	//std::cout << "Debug Rect: " << debugCircle.x << "," << debugCircle.y << "," << debugCircle.w << "," << debugCircle.h << std::endl;
	//std::cout << "X Velocity " << body->GetLinearVelocity().x << std::endl;

 	rotation.z = body->GetAngle() * GF2D_RADTODEG;

	//set anim state
	
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
