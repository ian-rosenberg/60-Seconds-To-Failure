#pragma once

#include <unordered_map>
#include <SDL.h>

typedef enum {
	MENU,
	WALK_UP,
	WALK_RIGHT,
	WALK_DOWN,
	WALK_LEFT,
	INTERACT,
	DECLINE,
	JUMP,
	PUNCH,
	KICK,
	SHOOT,
	NONE
}InputType;

typedef enum {
	IDLE = 0,
	RELEASED = 1,
	PRESSED = 2,
	HOLD = 4
}InputState;

typedef struct InputEvent{
	InputEvent*			prevEvent;
	SDL_Event*			e;
	InputType			inputType;
	InputState			state;
	Uint32				msSinceLastInput;
	/*void				(*onPress)(void* data);        /**<callback for press event
	void				(*onHold)(void* data);         /**<callback for hold event
	void				(*onRelease)(void* data);      /**<callback for release event
	**/
	InputEvent();

	~InputEvent();
}InputEvent_S;

class InputDriver {
private:
	std::vector<InputEvent*>*				inputQueue;
	std::vector<InputEvent*>*				heldEvents;

public:
	InputDriver();

	~InputDriver();

	inline void PushBackInputEvent(InputEvent* newEvent) { inputQueue->push_back(newEvent); }

	void PushBackHeldEvent(InputEvent* newEvent);

	InputEvent* GetNextInputEvent();

	inline InputEvent* GetNextHeldInputEvent() { return !heldEvents->empty() ? heldEvents->back() : nullptr; }

	InputEvent* PeekInputEventAtIndex(Uint16 index = 0);

	void ClearInputEventQueue();

	void DeleteEvent(InputEvent* e);

	void RemoveHeldEvent(InputEvent* e);

	InputEvent* FindFirstHeldEvent(InputType type);

	InputEvent* FindIthHeldEvent(InputType it, int occurences);

	int HeldEventCount(InputType type);
};