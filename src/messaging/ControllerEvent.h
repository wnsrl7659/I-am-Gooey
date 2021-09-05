// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.

#pragma once
#include "message.h"
#include <string>

enum MovementEvent
{
	none,
	up,
	down,
	left,
	right,
};


class ControllerEvent
{
public:
	MovementEvent move;
	ControllerEvent(MovementEvent move_) : move(move_) {};
	~ControllerEvent() {};
};