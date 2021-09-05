// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.


#include "InputEvent.h"

InputEvent::InputEvent(std::string key, std::string action) : key(key), action(action)
{
}

InputEvent::InputEvent()
{
	key = "none";
	action = "none";
}

InputEvent::~InputEvent()
{
}

std::ostream& operator<<(std::ostream& os, InputEvent const& e)
{
	os << e.key << ',' << e.action;
	return os;
}