/******************************************************************************/
/*!
\file   digispace.cpp
\author Ryan Hanson
\par    Email: ryan.hanson\@digipen.edu
\par    Course: Game 200
\brief  DigiPen logo splash screen

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../audio/audiomanager.h"
#include "../messaging/messaging.h"
#include "../messaging/messagehandler.h"
#include "../system/countdowntimer.h"
#include "../system/timermanager.h"
#include "../graphics/Sprite.h"
#include "../graphics/TextureManager.h"
#include "../graphics/Shader.h"
#include "../graphics/Fade.h"

void* digitimer = NULL;
void* digisprite = NULL;

namespace Spaces
{

	static void GoToRadiant(void *);


	struct DummySkipperD
	{
		DummySkipperD()
		{
			Messaging::messHandler->Attach<DummySkipperD, InputEvent, &DummySkipperD::InputHandler>(this, "InputEvent");
		}

		~DummySkipperD()
		{
			Messaging::messHandler->Detach<DummySkipperD>(this, "InputEvent");
		}

		void InputHandler(InputEvent * data)
		{
			if (data->action == "keyup" && (data->key == "mouse left" || data->key == "mouse right" || data->key == "space" || data->key == "esc" || data->key == "enter"))
			{
				GoToRadiant(nullptr);
			}
		}
	};

	DummySkipperD * dumdum = nullptr;

	/**************************************************************************/
	/*!
		\brief Builds the radiant splash space
	*/
	/**************************************************************************/
	static void GoToRadiant(void *)
	{
		if (dumdum)
		{
			delete dumdum;
		}
		spaceManager->QueueClearEvent();
		spaceManager->QueueBuildEvent(BuildRadiantSpace);
	}

	/**************************************************************************/
	/*!
		\brief Builds the DigiPen logo space
	*/
	/**************************************************************************/
	void BuildDigiSpace()
	{
		dumdum = new DummySkipperD();

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Main_Menu"));

		Space* splashSpace = spaceManager->AddSpace("10101 Splash");
		GameObject * splash = splashSpace->AddObject("Splash");
		Sprite* splashSprite = splash->AddComponent<Sprite>();
		digisprite = (void *)splashSprite;
		splashSprite->Init(Shader_BasicTexture);
		splashSprite->SetTexture(Tex_splash);
		Transform* splashTrans = splash->AddComponent<Transform>();
		splashTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		splashTrans->Scale(Vector2D(1024.0f, 512.0f));
		splashSprite->SetPosition(mainCam.center().x, mainCam.center().y);
		splashSprite->SetSize(1024.0f, 512.0f);
		splashSprite->SetCamera(mainCam);
		splashSprite->setAlpha(0.0f);

		spaceManager->SetSpace(Screens::Splash);

		// set up timer for fade out
		CountdownTimer * fade_timer = new CountdownTimer();
		digitimer = (void *)fade_timer;
		fade_timer->SetLimitSeconds(4);
		fade_timer->SetTrigger(new FreeFunctionWrapper<void, void>(GoToRadiant, nullptr));
		//fade_timer->Start();
		TimerManager::AddTimer(fade_timer);
	}
}