/******************************************************************************/
/*!
\file	autoplay.cpp
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Plays the game all by itself, for testing purposes
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "autoplay.h"
#include "../messaging/messaging.h"
#include "../messaging/InputEvent.h"

namespace Utils
{
	AutoTester * atst = nullptr;

	void InitTester()
	{
		atst = new AutoTester();
	}

	void ShutdownTester()
	{
		if (atst)
		{
			delete atst;
			atst = nullptr;
		}
	}
}

AutoTester::AutoTester()
{
}

AutoTester::~AutoTester()
{
}

void AutoTester::Start()
{
	running_ = true;
}

void AutoTester::Stop()
{
	running_ = false;
}

void AutoTester::Update(float dt)
{
	static_cast<void>(dt);
	if (running_)
	{
		int d = rand() % 4;
		switch (d)
		{
		case 0:
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &InputEvent("w", "keydown"));
			break;
		case 1:
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &InputEvent("a", "keydown"));
			break;
		case 2:
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &InputEvent("s", "keydown"));
			break;
		case 3:
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &InputEvent("d", "keydown"));
			break;
		default:
			break;
		}
	}
}