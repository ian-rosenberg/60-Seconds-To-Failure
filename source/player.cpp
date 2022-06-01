#include "player.h"
#include <iomanip>

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(std::shared_ptr<Graphics> g)
{
	name = "";
	keys = 0;
	controller = NULL;
	sensitivity = 0;
	maxSpeed = 4 * PIX_TO_MET;
	dampening = 0.0f;
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
	jumpForce = 25;
	scale = { 1,1 };
	prevDrawPosition = newDrawPosition = { 0,0 };
	prevBodyPosition = newBodyPosition = { 0,0 };

	punching = false;
	graphics = g;
	LoadActor(actorFilePath.c_str());
	CalculateAverageActorDimensions();
	SetWorldDimensions(b2Vec2(avgDim.x * PIX_TO_MET, avgDim.y * PIX_TO_MET));
	currentAnimation = GetAnimationByName("idle");
	currentSprite = currentAnimation->GetSprite();
}

void Player::Think()
{
	InputEvent* cEvent = inputDriver->GetNextInputEvent();

	if (cEvent) {
		switch (cEvent->inputType) {
			case MENU:
				break;

			case WALK_UP:
				break;

			case WALK_RIGHT:
				SetVelocity(cEvent->state > RELEASED ? maxSpeed :0);
				break;

			case WALK_DOWN:
				break;

			case WALK_LEFT:
				SetVelocity(cEvent->state > RELEASED ? -maxSpeed : 0);
				break;

			case INTERACT:
				break;

			case DECLINE:
				break;

			case JUMP:
 				if (cEvent->state == PRESSED) {
					if (cEvent->prevEvent) {
						if (cEvent->state == cEvent->prevEvent->state && cEvent->prevEvent->inputType != JUMP) {
							Jump();
						}
					}
					else {
						Jump();
					}
				}
				break;

			case PUNCH:
				break;

			case KICK:
				break;

			case SHOOT:
				break;

			default:
				break;
		}
	}

	//Handle walk release

	//Set logical state
 	if (punching) {
		SetLogicalState(State::State_Attacking);
		SetAnimationByName("attacking");
	}
	else if (velocity.x != 0.0f)
	{

		SetLogicalState(State::State_Walking);
		if(logicalState != animState)
			SetAnimationByName("walk");
	}
	else
	{
		SetLogicalState(State::State_Idle);
		SetAnimationByName("idle");
	}
}

void Player::Draw()
{
	Vector2 centerScalePoint = {0};
	Vector2 resultPos = { 0 };
	Vector4 debugColor = vector4(.5, 1, 0, 1);

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

	if (!IsGrounded() && debugDraw)
		debugDraw->SetCollisionColor(0);

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


	if (currentAnimation->AnimationNextFrame(currentAnimation) == AnimationReturnType::ART_END 
		&& strcmp(currentAnimation->GetName().c_str(), "attacking")==0) 
	{
		punching = false;
	}
}

void Player::UpdateScreenPosition(double alpha)
{
	prevBodyPosition = newBodyPosition;
	newBodyPosition = body->GetPosition();

	prevDrawPosition = newDrawPosition;

	debugRect.w = avgDim.x;
	debugRect.h = avgDim.y;

	newDrawPosition.x = ((SCALED_WIDTH / 2.0f) + newBodyPosition.x) * MET_TO_PIX;
	newDrawPosition.x = newDrawPosition.x * alpha + prevDrawPosition.x * (1.0 - alpha);
	newDrawPosition.y = ((SCALED_HEIGHT / 2.0f) + newBodyPosition.y) * MET_TO_PIX;
	newDrawPosition.y = newDrawPosition.y * alpha + prevDrawPosition.y * (1.0 - alpha);
	debugRect.x = newDrawPosition.x - avgDim.x/2;
	debugRect.y = newDrawPosition.y - avgDim.y/2;

	/*if(newBodyPosition.x != prevBodyPosition.x || newBodyPosition.y != prevBodyPosition.y)
		std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	if (newDrawPosition.x != prevDrawPosition.x || newDrawPosition.y != prevDrawPosition.y)
		std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;*/
}

void Player::Update()
{
	if (dead != 0)
	{
		return;
	}

	if (velocity.x != 0 || velocity.y != 0)
	{
		if (velocity.x < 0)
		{
			flip.x = 1;
		}
		else if (velocity.x > 0)
		{
			flip.x = 0;
		}
	}

	//std::cout << "World Position: " << newBodyPosition.x << "," << newBodyPosition.y << std::endl;
	//std::cout << "Draw Position: " << newDrawPosition.x << "," << newDrawPosition.y << std::endl;
	//std::cout << "Debug Rect: " << debugRect.x << "," << debugRect.y << "," << debugRect.w << "," << debugRect.h << std::endl;
	//std::cout << "X Velocity " << body->GetLinearVelocity().x << std::endl;

	rotation.z = body->GetAngle() * GF2D_RADTODEG;

	//set anim state
	if (logicalState != animState)
	{
		switch (logicalState)
		{
			case State::State_Walking:
				logicalState = animState;
				currentAnimation = GetAnimationByName("walk");
				currentSprite = currentAnimation->GetSprite();
				scaleCenter = vector2(currentAnimation->GetCellWidth() / 2, currentAnimation->GetCellHeight() / 2);
				break;

			case State::State_Idle:
				logicalState = animState;
				currentAnimation = GetAnimationByName("idle");
				currentSprite = currentAnimation->GetSprite();
				scaleCenter = vector2(currentAnimation->GetCellWidth() / 2, currentAnimation->GetCellHeight() / 2);
				break;

			case State::State_Attacking:
				logicalState = animState;
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
