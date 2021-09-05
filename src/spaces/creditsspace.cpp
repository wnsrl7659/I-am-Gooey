/******************************************************************************/
/*!
\file   creditsspace.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Credits Space File, builds the Credits when called

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <functional>

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../system/timermanager.h"
#include "../system/countdowntimer.h"
#include "../messaging/messagehandler.h"
#include "spacemanager.h"
#include "../audio/audiomanager.h"

namespace Spaces
{
	static void BackToMenu(void *);

	static CountdownTimer * exit_timer;

	struct input_dummy;
	static input_dummy * dummy;


	struct input_dummy
	{
		input_dummy()
		{
			Messaging::messHandler->Attach<input_dummy, InputEvent, &input_dummy::input_handler>(this, "InputEvent");
		}

		~input_dummy()
		{
			Messaging::messHandler->Detach<input_dummy>(this, "InputEvent");
		}

		void input_handler(InputEvent * data)
		{
			if (data->action == "keyup" && data->key == "esc")
			{
				exit_timer->Stop();
				Spaces::spaceManager->ExecuteNextFrame(std::bind(BackToMenu, nullptr));
			}
		}
	};

	static void BackToMenu(void *)
	{
		delete dummy;
		dummy = nullptr;
		spaceManager->QueueClearEvent();
		spaceManager->QueueBuildEvent(Spaces::BuildMainMenu);
		spaceManager->SetLevel(Spaces::Levels::initial);
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Credits_Track"));
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Main_Menu"));
	}



	void BuildCredit(Space* creditsSpace, int texture, float ypos, const char * name)
	{
		////
		GameObject * titleCredits = creditsSpace->AddObject(name);
		Sprite* titleSprite = titleCredits->AddComponent<Sprite>();
		titleSprite->Init(Shader_BasicTexture);
		titleSprite->SetTexture(texture);
		Transform* titleTrans = titleCredits->AddComponent<Transform>();
		titleTrans->Translation(Vector2D(mainCam.center().x + 0.0f, mainCam.center().y + ypos));
		titleTrans->Scale(Vector2D(1638.4f, 819.2f));
		titleSprite->SetPosition(mainCam.center().x + 0.0f, mainCam.center().y + ypos);
		titleSprite->SetSize(1638.4f, 819.2f);
		titleSprite->SetCamera(mainCam);
		Paralax* titlePara = titleCredits->AddComponent<Paralax>();
		titlePara->Direction(Paralax::pDir::Y);
		titlePara->Speed(3.2f);
	}
	/**************************************************************************/
	/*!
	\brief Builds Credit Space
	*/
	/**************************************************************************/
	void BuildCreditsSpace()
	{
		Space * creditsSpace = spaceManager->AddSpace("34500 Credits");
		////original start at -1024 --1024
		BuildCredit(creditsSpace, Tex_titlecredits, -900.0f, "Title Credits");
		BuildCredit(creditsSpace, Tex_teamcredits, -1800.0f, "Team Credits");
		BuildCredit(creditsSpace, Tex_specialcredits, -2700.0f, "Special Credits");
		BuildCredit(creditsSpace, Tex_instcredits, -3600.0f, "Instructor Credits");
		BuildCredit(creditsSpace, Tex_execcredits, -4500.0f, "Exec Credits");
		BuildCredit(creditsSpace, Tex_glfwcredits, -5400.0f, "Licenses");
		////
		GameObject* exit = creditsSpace->AddObject("End");
		Sprite* exitSprite = exit->AddComponent<Sprite>();
		exitSprite->Init(Shader_BasicTexture);
		exitSprite->SetTexture(Tex_Box);
		Transform* exitTrans = exit->AddComponent<Transform>();
		exitTrans->Translation(Vector2D(mainCam.center().x + 0.0f, mainCam.center().y + 1080.0f));
		exitTrans->Scale(Vector2D(1.0f, 1.0f));
		exitSprite->SetPosition(mainCam.center().x + 0.0f, mainCam.center().y + 1080.0f);
		exitSprite->SetSize(1.0f, 1.0f);
		exitSprite->SetCamera(mainCam);
		UI* end = exit->AddComponent<UI>();
		end->Set(0.0f, 1080.0f);
		////
		GameObject* selector = creditsSpace->AddObject("selector");
		Sprite* selectSprite = selector->AddComponent<Sprite>();
		selectSprite->Init(Shader_BasicTexture);
		selectSprite->SetTexture(Tex_select);
		Transform* selectTrans = selector->AddComponent<Transform>();
		selectTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		selectTrans->Scale(Vector2D(1.0f, 1.0f));
		selectSprite->SetPosition(mainCam.up().x, mainCam.up().y);
		selectSprite->SetSize(1.0f, 1.0f);
		selectSprite->SetCamera(mainCam);
		Selector* select = selector->AddComponent<Selector>();
		select->Set(0.0f, 0.0f);
		select->AddButton(exit);
		
    exit_timer = new CountdownTimer();
    exit_timer->SetLimitSeconds(50);
    exit_timer->SetTrigger(new FreeFunctionWrapper<void, void>(BackToMenu, nullptr));
    exit_timer->Start();
    TimerManager::AddTimer(exit_timer);
	Audio::aManager->StopAllEvents();
	Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
	Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Credits_Track"));
	dummy = new input_dummy;
	}
}