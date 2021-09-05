/******************************************************************************/
/*!
\file   messaging.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
 namespace to contain the message handler, as well as create and destroy it

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "messaging.h"
#include "../trace/trace.h"

namespace Messaging
{
	MessageHandler * messHandler;

	void Init()
	{
		messHandler = new MessageHandler();
		if (!messHandler)
		{
			Trace::Message("Message Handler failed to init!");
		}
	}

	void Update(float dt)
	{
		static_cast<void>(dt);
	}

	void Shutdown()
	{
		delete messHandler;
		messHandler = nullptr;
	}
}
