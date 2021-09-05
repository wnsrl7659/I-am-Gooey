// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.


#ifndef INPUTEVENT_H

#define INPUTEVENT_H



#include "message.h"
#include <string>

class InputEvent
{
public:
	std::string key;
	std::string action;


	InputEvent(std::string key_, std::string action_);
	InputEvent();
	~InputEvent();

	friend std::ostream& operator<<(std::ostream& os, const InputEvent& e);
};


#endif // !INPUTEVENT_H


