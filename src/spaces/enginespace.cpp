/******************************************************************************/
/*!
\file   enginespace.cpp
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Splash screen for the engine name.

\copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.
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
#include "../component/paralax.h"
#include "../component/pscript.h"
#include "../component/controller.h"
#include "../settings/settings.h"

namespace Spaces
{
	static float vol_save;
	static CountdownTimer * pause_timer = nullptr;

	static void BuildMenu(void *);

	struct DummySkipperE
	{
		/**************************************************************************/
		/*!
			\brief Constructor
		*/
		/**************************************************************************/
		DummySkipperE()
		{
			Messaging::messHandler->Attach<DummySkipperE, InputEvent, &DummySkipperE::InputHandler>(this, "InputEvent");
		}

		/**************************************************************************/
		/*!
			\brief Destructor
		*/
		/**************************************************************************/
		~DummySkipperE()
		{
			Messaging::messHandler->Detach<DummySkipperE>(this, "InputEvent");
		}

		/**************************************************************************/
		/*!
			\brief Handler for input events
			\param data
				Pointer to the event data
		*/
		/**************************************************************************/
		void InputHandler(InputEvent * data)
		{
			if (data->action == "keyup" && (data->key == "mouse left" || data->key == "mouse right" || data->key == "space" || data->key == "esc" || data->key == "enter"))
			{
				if (pause_timer) pause_timer->Stop();
				BuildMenu(nullptr);
			}
		}
	};

	DummySkipperE * dumdum = nullptr;

	/**************************************************************************/
	/*!
		\brief Builds the main menu
	*/
	/**************************************************************************/
	static void BuildMenu(void *)
	{
		if (dumdum)
		{
			delete dumdum;
		}
		spaceManager->QueueClearEvent();
		spaceManager->QueueBuildEvent(BuildMainMenu);
		Settings::sfxVol = vol_save;
	}

	/**************************************************************************/
	/*!
		\brief Psuedo script function to move the engine logo across the screen
		\param obj
			Object that will be moving, in this case the engine logo
	*/
	/**************************************************************************/
	static void WhileMoving(float, GameObject * obj)
	{
		if (obj)
		{
			Transform * trans = obj->GetComponent<Transform>();
			if (trans)
			{
				if (trans->Translation()[0] <= mainCam.center().x)
				{
					Paralax * para = obj->GetComponent<Paralax>();
					if (para)
					{
						para->Speed(0.0f);
						pause_timer = new CountdownTimer();
						pause_timer->SetLimitSeconds(1);
						pause_timer->SetTrigger(new FreeFunctionWrapper<void, void>(BuildMenu, nullptr));
						pause_timer->Start();
						TimerManager::AddTimer(pause_timer);
					}
				}
			}
		}
	}

	/**************************************************************************/
	/*!
		\brief Makes Gooey walk across the screen, to the left
	*/
	/**************************************************************************/
	static void StartWalking(void *)
	{
		plhandleInput(GLFW_KEY_A, GLFW_PRESS, GetWindow());
	}

	/**************************************************************************/
	/*!
		\brief Builds the engine space
	*/
	/**************************************************************************/
	void BuildEngineSpace()
	{
		vol_save = Settings::sfxVol;
		Settings::sfxVol *= 2;
		dumdum = new DummySkipperE();

		Space* splashSpace = spaceManager->AddSpace("10101 Splash");
		GameObject * logo = splashSpace->AddObject("Logo");
		Sprite* logoSprite = logo->AddComponent<Sprite>();
		logoSprite->Init(Shader_BasicTexture);
		logoSprite->SetTexture(Tex_EngineScreen);
		Transform* logoTrans = logo->AddComponent<Transform>();
		logoTrans->Translation(Vector2D(mainCam.center().x + 1728.0f, mainCam.center().y));
		logoTrans->Scale(Vector2D(1728.0f, 1120.0f));
		logoSprite->SetPosition(mainCam.center().x, mainCam.center().y + 1120.0f);
		logoSprite->SetSize(1728.0f, 1120.0f);
		logoSprite->SetCamera(mainCam);
		Paralax * para = logo->AddComponent<Paralax>();
		para->Direction(Paralax::pDir::X);
		para->Speed(-5.0f);
		para->Init();
		// script component
		p_script * script = logo->AddComponent<p_script>();
		script->push_front(WhileMoving);

		GameObject * player = BuildPlayerspace();
		Transform * ptrans = player->GetComponent<Transform>();
		ptrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		Space * pspace = spaceManager->GetSpace("9999 playerspace");
		pspace->RemoveObject("acorn");
		pspace->RemoveObject("number");
		pspace->RemoveObject("number2");


		// timer to start the player walking
		CountdownTimer * walk_timer = new CountdownTimer();
		walk_timer->SetLimitMilliseconds(1000);
		walk_timer->SetTrigger(new FreeFunctionWrapper<void, void>(StartWalking, nullptr));
		walk_timer->Start();
		TimerManager::AddTimer(walk_timer);
	}
}