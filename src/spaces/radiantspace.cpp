/******************************************************************************/
/*!
\file   radiantspace.cpp
\author Ryan Hanson
\par    Email: ryan.hanson\@digipen.edu
\par    Course: Game 200
\brief  Team Radiant logo splash screen

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "spaces.h"
#include "../system/timermanager.h"
#include "../system/countdowntimer.h"
#include "../graphics/Shader.h"
#include "../graphics/Sprite.h"
#include "../messaging/messagehandler.h"
#include "../messaging/InputEvent.h"

namespace Spaces
{
	static void BuildEngine(void *);


	struct DummySkipper
	{
		DummySkipper()
		{
			Messaging::messHandler->Attach<DummySkipper, InputEvent, &DummySkipper::InputHandler>(this, "InputEvent");
		}

		~DummySkipper()
		{
			Messaging::messHandler->Detach<DummySkipper>(this, "InputEvent");
		}

		void InputHandler(InputEvent * data)
		{
			if (data->action == "keyup" && (data->key == "mouse left" || data->key == "mouse right" || data->key == "space" || data->key == "esc" || data->key == "enter"))
			{
				BuildEngine(nullptr);
			}
		}
	};

	DummySkipper * dumdum = nullptr;

	static void BuildEngine(void *)
	{
		if (dumdum)
		{
			delete dumdum;
		}
		spaceManager->QueueClearEvent();
		spaceManager->QueueBuildEvent(BuildEngineSpace);
	}

	void BuildRadiantSpace()
	{
		spaceManager->SetSpace(Screens::Logo);

		dumdum = new DummySkipper();

		Space* splashSpace = spaceManager->AddSpace("10101 Splash");
		GameObject * logo = splashSpace->AddObject("Logo");
		Sprite* logoSprite = logo->AddComponent<Sprite>();
		logoSprite->Init(Shader_BasicTexture);
		logoSprite->SetTexture(Tex_Logo);
		Transform* logoTrans = logo->AddComponent<Transform>();
		logoTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		logoTrans->Scale(Vector2D(1728.0f, 1120.0f));
		logoSprite->SetPosition(mainCam.center().x, mainCam.center().y + 1120.0f);
		logoSprite->SetSize(1728.0f, 1120.0f);
		logoSprite->SetCamera(mainCam);



		// set up timer to fade into menu
		CountdownTimer * fade_timer = new CountdownTimer();
		fade_timer->SetLimitSeconds(4);
		fade_timer->SetTrigger(new FreeFunctionWrapper<void, void>(BuildEngine, nullptr));
		fade_timer->Start();
		TimerManager::AddTimer(fade_timer);

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Logo"));
	}
}