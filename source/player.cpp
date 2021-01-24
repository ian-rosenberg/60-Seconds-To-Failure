#include "player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer* ren)
{
	name = "";
	keys = 0;
	controller = NULL;
	sensitivity = 0;
	maxSpeed = 2;
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
	scale = { 1,1 };
	position = { 10,10 };
	renderer = ren;
	LoadActor(actorFilePath.c_str());
	currentAnimation = GetAnimationByName("idle");
}

void Player::Think()
{
	if (controller != 0 && SDL_GameControllerGetAttached(controller)) {
		Vector2 LeftStick = vector2(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX),
			SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY));

		int16_t LeftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		int16_t RightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

		Bool AButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
		Bool BButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
		Bool XButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
		Bool YButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y);
		Bool Up = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
		Bool Down = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		Bool Left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		Bool Right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		Bool Start = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START);
		Bool Back = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK);

		//Right Trigger Pressed
		if (RightTrigger > 1000) {

		}
		//Left Trigger Pressed
		if (LeftTrigger > 1000) {

		}

		//Axis lock on left stick
		if (LeftStick.x < JOYSTICK_DEAD_ZONE && LeftStick.x > -JOYSTICK_DEAD_ZONE)
			LeftStick.x = 0;
		if (LeftStick.y < JOYSTICK_DEAD_ZONE && LeftStick.y > -JOYSTICK_DEAD_ZONE)
			LeftStick.y = 0;

		//Stop moving player when left stick is left alone
		if (LeftStick.x > 0 && velocity.x < 0) {
			SetVelocity(0, velocity.y);
		}
		if (LeftStick.x < 0 && velocity.x > 0) {
			SetVelocity(0, velocity.y);
		}
		if (LeftStick.y > 0 && velocity.y < 0) {
			SetVelocity(velocity.x, 0);
		}
		if (LeftStick.y < 0 && velocity.y > 0) {
			SetVelocity(velocity.x, 0);
		}

		//Move player according to left stick
		SetVelocity(LeftStick.x * 0.0010f, LeftStick.y * 0.0010f);

		//If left stick is untouched, player stops moving
		if (LeftStick.x == 0)
			SetVelocity(0, velocity.y);

		if (LeftStick.y == 0)
			SetVelocity(velocity.x, 0);
	}
	else
	{
		keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_A] && velocity.x < 0) {
			SetVelocity(0, velocity.y);
		}
		if (keys[SDL_SCANCODE_D] && velocity.x > 0) {
			SetVelocity(0, velocity.y);
		}
		if (keys[SDL_SCANCODE_S] && velocity.y < 0) {
			SetVelocity(velocity.x, 0);
		}
		if (keys[SDL_SCANCODE_W] && velocity.y > 0) {
			SetVelocity(velocity.x, 0);
		}

		//Move player according to  key input
		if (keys[SDL_SCANCODE_A])
		{
			SetVelocity(-maxSpeed, 0);
		}
		else if (keys[SDL_SCANCODE_D])
		{
			SetVelocity(maxSpeed, 0);
		}
		if (keys[SDL_SCANCODE_W])
		{
			SetVelocity(0, -maxSpeed);
		}
		else if (keys[SDL_SCANCODE_S])
		{
			SetVelocity(0, maxSpeed);
		}

		//If keys are untouched, player stops moving
		if (!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D])
			SetVelocity(0, velocity.y);

		if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S])
			SetVelocity(velocity.x, 0);
	}


	//Set logical state
	if (velocity.x != 0.0f ||
		velocity.y != 0.0f)
	{
		SetLogicalState(State::State_Walking);
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
	Vector2 centerScalePoint;
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

	scaleCenter = vector2(currentAnimation->GetCellWidth() / 2.0f,
		currentAnimation->GetCellHeight() / 2.0f);

	//vector2d_sub(resultPos, position, GetCameraPosition());

	//Should probably just call a draw function, and let the actor take care of animating

	std::cout << '{' << position.x << ',' << position.y << '}' << std::endl;

	currentSprite->Draw(currentSprite,
		position,
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

void Player::Update()
{
	//Vector2 tilemapDimensions = vector2(GetCurrentTileMap()->boundingBox.w, GetCurrentTileMap()->boundingBox.h);

	if (!this)
	{
		return;
	}

	if (dead != 0)
	{
		return;
	}

	//Movement
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

	if (velocity.x > maxSpeed) {
		velocity.x = maxSpeed;
	}
	else if (velocity.x < -maxSpeed) {
		velocity.x = -maxSpeed;
	}
	if (velocity.y > maxSpeed) {
		velocity.y = maxSpeed;
	}
	else if (velocity.y < -maxSpeed) {
		velocity.y = -maxSpeed;
	}

	position.x += velocity.x;
	position.y += velocity.y;

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
