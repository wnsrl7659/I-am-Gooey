/******************************************************************************/
/*!
\file   audiotestspace.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  space for testing audio assets and events

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../messaging/messaging.h"
#include "../messaging/InputEvent.h"

namespace Spaces
{
	void BuildAudioTest()
	{
		Space * aTst = spaceManager->AddSpace("audiotest");
		Space * lute = spaceManager->AddSpace("lutespace");
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Ambience"));
	}
}
