/******************************************************************************/
/*!
\file   splashspace.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Displays the Splash Screen

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../system/countdowntimer.h"
#include "../system/timermanager.h"
#include "../audio/audiomanager.h"
#include "../audio/audio.h"
#include "../graphics/Shader.h"

GameObject* logo;
GameObject* splash;

namespace Spaces
{
	void BuildSplashSpace()
	{
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Main_Menu"));
		Audio::EventData * data = new Audio::EventData();
		data->eventName = "Music_Title";
		data->paramName = "First Play";
		data->val = 1.0f;
		CountdownTimer * cnt = new CountdownTimer();
		cnt->SetLimitSeconds(12);
		cnt->SetTrigger(new MemberFunctionWrapper<void, AudioManager, Audio::EventData>(&AudioManager::EditEvent, Audio::aManager, data));
		cnt->Start();
		TimerManager::AddTimer(cnt);

		Space* splashSpace = spaceManager->AddSpace("10101 Splash");
		splash = splashSpace->AddObject("Splash");
		Sprite* splashSprite = splash->AddComponent<Sprite>();
		splashSprite->Init(Shader_BasicTexture);
		splashSprite->SetTexture(Tex_splash);
		Transform* splashTrans = splash->AddComponent<Transform>();
		splashTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		splashTrans->Scale(Vector2D(1024.0f, 512.0f));
		splashSprite->SetPosition(mainCam.center().x, mainCam.center().y);
		splashSprite->SetSize(1024.0f, 512.0f);
		splashSprite->SetCamera(mainCam);

		logo = splashSpace->AddObject("Logo");
		Sprite* logoSprite = logo->AddComponent<Sprite>();
		logoSprite->Init(Shader_BasicTexture);
		logoSprite->SetTexture(Tex_Logo);
		Transform* logoTrans = logo->AddComponent<Transform>();
		logoTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y + 1120.0f));
		logoTrans->Scale(Vector2D(1728.0f, 1120.0f));
		logoSprite->SetPosition(mainCam.center().x, mainCam.center().y + 1120.0f);
		logoSprite->SetSize(1728.0f, 1120.0f);
		logoSprite->SetCamera(mainCam);
		
		spaceManager->SetSpace(Screens::Splash);
	}
}