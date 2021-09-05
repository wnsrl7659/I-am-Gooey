/******************************************************************************/
/*!
\file   Fade.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Fade.h"
#include "../messaging/messaging.h"
#include "../messaging/messagehandler.h"
#include "../messaging/InputEvent.h"
#include "../spaces/spacemanager.h"
#include "../component/MultiEmitter.h"
#include "../audio/audiomanager.h"

GameObject* fadeobj;
static bool fadein, fadeout;
static Transform* trs;
static Sprite* spr;
static bool fadeined;
static ButtonEvent whats(noEvent);
static int diff;
static int caseID;
static void* tempmanager;
static int templevel;

void FadeInit()
{
	fadeobj = new GameObject;

	trs = fadeobj->AddComponent<Transform>();
	trs->Init();
	trs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1] - 1120 - 560));
	spr = fadeobj->AddComponent<Sprite>();

	spr->Init(Shader_BasicTexture);
	spr->SetTexture(Tex_Fade);
	spr->SetSize(1728, 2240);
	spr->IgnoreCulling();

	spr->setPrior(6);
	spr->setAlpha(0.0f);
	fadein = false;
	fadeout = false;
	fadeined = false;
	diff = 32;

	auto ME = fadeobj->AddComponent<MultiEmitter>();
	ME->addEmitter("Transition1.json");
	ME->addEmitter("Transition2.json");
	/*
	ME->addEmitter("lk1.json");
	ME->addEmitter("lk2.json");
	ME->addEmitter("lk3.json");
	ME->addEmitter("lk5.json");
	*/
	//ME->addEmitter("lk6.json");
}

void FadeUpdate(float dt)
{
	if (!IsFadeWorking())
	{
		return;
	}
	if (fadeined == true)
	{
		trs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1] + 560));
	}
	if (fadein == true)
	{
		Vector2D pos;
		pos[0] = mainCam.GetPosition()[0];
		pos[1] = mainCam.GetPosition()[1] - 1120 - 560 + diff;

		diff += 32;

		trs->Translation(pos);

		if (pos[1] >= (mainCam.GetPosition()[1] + 560))
		{
			fadeined = true;
			fadein = false;
			diff = 0;
			if (caseID == 0)
			{
			}
			else if (caseID == 1)
			{
				FadeOut();
			}
			else if (caseID == 2)
			{
				Spaces::Levels c_level = static_cast<Spaces::Levels>(templevel);
				SpaceManager* tempsp = (SpaceManager*)tempmanager;
				tempsp->QueueClearEvent();
				tempsp->QueueBuildEvent(Spaces::level_loaders[c_level]);
				Audio::aManager->StopAllEvents();
				FadeOut();
			}
			else if (caseID == 3)
			{
				SpaceManager* tempsp = (SpaceManager*)tempmanager;
				tempsp->QueueClearEvent();
				tempsp->QueueBuildEvent(Spaces::BuildMainMenu);
				Audio::aManager->StopAllEvents();
				FadeOut();
			}
		}
	}
	if (fadeout == true)
	{
		fadeined = false;

		Vector2D pos;
		pos[0] = mainCam.GetPosition()[0];
		pos[1] = mainCam.GetPosition()[1] + 560 - diff;

		diff += 32;

		trs->Translation(pos);

		if (pos[1] <= (mainCam.GetPosition()[1] - 1120 - 560))
		{
			fadeout = false;
			diff = 0;
		}
	}

	fadeobj->Update(dt);
}

void FadeShutdown()
{
	delete fadeobj;
}


void FadeIn(int ID, ButtonEvent what, void* tempowner, int level)
{
	if (fadeined == true)
	{
		return;
	}
	if (fadein == false && fadeout == false)
	{
		caseID = ID;

		if (ID == 0)
		{
		}
		else if (ID == 1)
		{
			whats = what;
		}
		else if (ID == 2)
		{
			templevel = level;
			tempmanager = tempowner;
		}
		else if (ID == 3)
		{
			tempmanager = tempowner;
		}


		fadein = true;
		spr->setAlpha(1.0f);
		trs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1] - 1120 - 560));
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Transition"));
	}
}

void FadeOut(void *)
{
	if (fadeined == false)
	{
		return;
	}
	if (fadein == false && fadeout == false)
	{
		fadeout = true;
		spr->setAlpha(1.0f);
		trs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1] + 560));
	}
}

bool IsFadeWorking()
{
	if (fadein == true || fadeined == true || fadeout == true)
	{
		return true;
	}

	return false;
}
bool IsFadeIned()
{
	return fadeined;
}