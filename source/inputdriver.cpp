#include "inputdriver.h"

InputDriver::InputDriver()
{
	inputQueue = new std::vector<InputEvent*>();	
	heldEvents = new std::vector<InputEvent*>();
}

InputDriver::~InputDriver()
{
	InputEvent* e;

	if (heldEvents){
		while (!heldEvents->empty()) {
			e = heldEvents->at(heldEvents->size() - 1);
			heldEvents->pop_back();
			delete e;
		}
	}

	if (!inputQueue->empty())
		ClearInputEventQueue();

	delete inputQueue;
}

void InputDriver::PushBackHeldEvent(InputEvent* newEvent)
{
	InputEvent* ie = (!heldEvents->empty() && heldEvents->back() != nullptr) ? heldEvents->back() : nullptr;

	if (ie && ie->inputType == newEvent->inputType)
		return;

	heldEvents->push_back(newEvent);
}

InputEvent* InputDriver::GetNextInputEvent()
{
	InputEvent* e = PeekInputEventAtIndex(0);

	if(e)
		inputQueue->erase(inputQueue->begin());

	return e ? e : nullptr;
}

InputEvent* InputDriver::PeekInputEventAtIndex(Uint16 index)
{
	if (index < inputQueue->size() && index >= 0)
		return inputQueue->at(index);
	else if (index == -1)
		return inputQueue->at(inputQueue->size() - 1);
	else
		return nullptr;
}

void InputDriver::ClearInputEventQueue()
{
	InputEvent* e = NULL;

	while (!inputQueue->empty()) {
		e = inputQueue->at(inputQueue->size()-1);
		inputQueue->pop_back();
		delete e;
	}
}

void InputDriver::DeleteEvent(InputEvent* e)
{
	if (e) {
		if (e->prevEvent)
			delete e->prevEvent;
		delete e;
	}
}

void InputDriver::RemoveHeldEvent(InputEvent* e)
{
	std::vector<InputEvent*>::iterator i;
	InputEvent* ie;

	for (auto it = heldEvents->begin(); it != heldEvents->end(); it++) {
		i = it;
		ie = *i;

		if (ie && ie->inputType == ie->inputType)
			break;
	}
	
	heldEvents->erase(i);
}

InputEvent* InputDriver::FindFirstHeldEvent(InputType type)
{
	for (auto it = heldEvents->begin(); it != heldEvents->end(); it++)
		if ((*it)->inputType == type)
			return *it;

	return nullptr;
}

InputEvent* InputDriver::FindIthHeldEvent(InputType type, int occurences)
{
	int i = 0;
	for (auto it = heldEvents->begin(); it != heldEvents->end(); it++) {
		if (i == occurences - 1 && (*it)->inputType == type)
			return *it;
		else if ((*it)->inputType == type)
			i++;
	}

	return nullptr;
}

int InputDriver::HeldEventCount(InputType type)
{
	int i = 0;
	for (auto it = heldEvents->begin(); it != heldEvents->end(); it++) {
		if ((*it)->inputType == type)
			i++;
	}
	return i;
}

InputEvent::InputEvent()
{
	prevEvent = NULL;
	e = NULL;
	inputType = NONE;
	state = IDLE;
	msSinceLastInput = 0;
	/*onPress = nullptr;
	onHold = nullptr;
	onRelease = nullptr;*/
}

InputEvent::~InputEvent()
{
	e = nullptr;
}
