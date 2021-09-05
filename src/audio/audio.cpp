/******************************************************************************/
/*!
\file   audio.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Audio namespace, for setting up, shutting down, and updating the audio manager

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "audio.h"
#include "audiomanager.h"
#include "../messaging/messagehandler.h"

namespace Audio
{
	AudioManager * aManager;

	void Init(void)
	{
		aManager = new AudioManager();
		aManager->Init();
	}

	void Update(float dt)
	{
		aManager->Update(dt);
	}

	void Shutdown(void)
	{
		aManager->Shutdown();
		delete aManager;
		aManager = nullptr;
	}

	void EditParam(Audio::EventData * d)
	{
		aManager->EditEvent(d);
	}

	void PauseAll(void *)
	{
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("pause-p all"));
	}

	void UnpauseAll(void *)
	{
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("unpause all"));
	}

	void ReturnToLevelSnap(void *)
	{
		UnpauseAll(nullptr);
		aManager->SetSnapshot("Level");
	}
}