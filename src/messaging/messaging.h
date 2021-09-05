/******************************************************************************/
/*!
\file   messaging.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  namespace to contain the message handler, as well as create and destroy it

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef MESSAGING_H
#define MESSAGING_H

#include "messagehandler.h"

namespace Messaging
{
	void Init();

	void Update(float dt);

	void Shutdown();

	extern MessageHandler * messHandler;
}

#endif // !MESSAGING_H
