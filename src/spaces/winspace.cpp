/******************************************************************************/
/*!
\file   winsppace.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../component/MultiEmitter.h"
#include "../vfx/color.h"
#include "../physics/Physics.h"
#include "../graphics/FontRender.h"
#include "../puzzles/newpuzzle.h"
#include "../audio/audiomanager.h"

namespace Spaces
{
	void BuildWinSpace()
	{
		Space* winSpace = spaceManager->AddSpace("609 Win Space");
		spaceManager->SetSpace(Screens::Win);
		////

		int hintCount = PzManage::pzm->hints;

		float skyR = hintCount * 4 * 4/3 * .1 / 2.f + .1, 
				  skyG = hintCount * .80 * 4/3 * .1 / 2.f + .02, 
				  skyB = hintCount * .75 * 4/3 * .1 / 2.f;

	//BACKGROUND
		Space* Space = spaceManager->AddSpace("10101 WinSpace");
		GameObject * BG = Space->AddObject("BG");
		Sprite* BGSpr = BG->AddComponent<Sprite>();
		BGSpr->Init(Shader_Multiply);
		BGSpr->SetTexture(Tex_WinBG);

		Transform* BGTrs = BG->AddComponent<Transform>();
		BGTrs->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		BGTrs->Scale(Vector2D(mainCam.width(), mainCam.height()));
		BGSpr->SetPosition(mainCam.center().x, mainCam.center().y);
		BGSpr->SetSize(1024.0f, 512.0f);
		BGSpr->setAlpha(1.0f);
		BGSpr->setPrior(0);

		Color* BGColor = BG->AddComponent<Color>();
		BGColor->r(0.0f);
		BGColor->g(0.0f);
		BGColor->b(0.0f);
		BGColor->SetColorChange(-.4f, -.10f, -.075f);
		BGColor->SetColorMax(skyR, skyG, skyB);


  //EMITTER's OBJECT
		GameObject * SUN = Space->AddObject("SUN");
		//Sprite* SUN = BG->AddComponent<Sprite>();
		Transform* SUNTrs = SUN->AddComponent<Transform>();
		SUNTrs->Translation(Vector2D(mainCam.center().x, mainCam.center().y - mainCam.height()/8));
		Physics* SUNPhy = SUN->AddComponent<Physics>();
		SUNPhy->Velocity(Vector2D(0.f, 1.5f));
		SUNPhy->friction = .16f;
		
		MultiEmitter* SUNEm = SUN->AddComponent<MultiEmitter>();
		if(hintCount >= 20)
				SUNEm->addEmitter("sun3.json");
		if(hintCount >= 10)
				SUNEm->addEmitter("sun2.json");
		if(hintCount >= 15)
				SUNEm->addEmitter("sun1.json");
		SUNEm->addEmitter("flame3.json");
		SUNEm->addEmitter("flame2.json");
		SUNEm->addEmitter("flame1.json");
		SUNEm->addEmitter("flame5.json");
		SUNEm->addEmitter("flame4.json");
		
		for (auto it : *SUNEm->getEmitters())
		{
				
				float max = 1.f + (float(hintCount) / 4.f);
				it->setSizeScaleMax(max);
				it->setSizeScaleChange(max/6.f);
		}


	//TOWER
		Vector2D cambottom(mainCam.center()[0], mainCam.center()[1] - mainCam.height() / 2.f);
		GameObject* Tower = Space->AddObject("Tower");
		Sprite* sprTower = Tower->AddComponent<Sprite>();
		Transform* trsTower = Tower->AddComponent<Transform>();
		sprTower->Init(Shader_Multiply);
		sprTower->SetTexture(Tex_WinTower);
		sprTower->setPrior(2);
		trsTower->Scale(Vector2D(mainCam.width()/3, mainCam.height() / 1.1f));
		trsTower->Translation(cambottom + Vector2D(0.f, 0.f));

		Color* TowerColor = Tower->AddComponent<Color>();
		TowerColor->r(0.0f);
		TowerColor->g(0.0f);
		TowerColor->b(0.0f);
		TowerColor->SetColorChange(-.4f, -.10f, -.075f);
		TowerColor->SetColorMax(skyR, skyG, skyB);

//Credits Button
		/**/
		GameObject* credits = Space->AddObject("Credits");
		Transform* ctransform = credits->AddComponent<Transform>();
		MultiEmitter* emit = credits->AddComponent<MultiEmitter>();
		ctransform->Translation(Vector2D(mainCam.center().x + 900.0f, mainCam.center().y));
		ctransform->Scale(Vector2D(256.0f, 128.0f));
		UI* creditUI = credits->AddComponent<UI>();
		creditUI->Set(460.0f, -445.0f);
		emit->addEmitter("SelectorMirowed.json");
		emit->addEmitter("Selector.json");
		auto emitters = emit->getEmitters();

		FontRender* cFont = credits->AddComponent<FontRender>();
		cFont->SetPos(1525.0f, 90.0f);
		cFont->SetSize(0.5f);
		cFont->SetText(L"NEXT");
		cFont->setMode(2);
		Selector* select = credits->AddComponent<Selector>();
		select->Set(0.0f, 0);
		select->AddButton(credits);

		//there are 20 hints total. string will be: hints / 20;
		/**/
//CoinCounter
		//set up 2 separate "acorn" objects, one for hints collected, 
		//the other for hints total
		GameObject* acorn1 = Space->AddObject("acorn");
		Sprite* cornsprite1 = acorn1->AddComponent<Sprite>();
		Transform* coinTrs1 = acorn1->AddComponent<Transform>();
		Animation* canim1 = acorn1->AddComponent<Animation>();
		UI * corn1 = acorn1->AddComponent<UI>();
		cornsprite1->Init(Shader_AnimatedTexture);
		cornsprite1->setPrior(3);
		canim1->Init(32, 16, 1, 1);
		canim1->setAnimationSpeed(0);
		(*canim1)[0][0] = 43;
		canim1->setSequence(0);
		cornsprite1->SetTexture(Tex_tile_set);
		cornsprite1->SetCamera(mainCam);
		coinTrs1->Scale(Vector2D(256.0f, 256.0f));
		cornsprite1->SetSize(64.0f, 64.0f);
		coinTrs1->Translation(Vector2D(1408.0f, -1024.0f));
		cornsprite1->SetPosition(1408.0f, -1024.0f);
		corn1->Set(mainCam.width() / 2 - 1100.0f, mainCam.height() / 2 - 540.0f);
		FontRender* acornFont1 = acorn1->AddComponent<FontRender>();
		acornFont1->SetPos(750.0f, 550.0f);
		acornFont1->SetSize(1.0f);
		acornFont1->setMode(2);
		acornFont1->setType(3);
		std::wstringstream ss;
		ss << hintCount;
		acornFont1->SetText(ss.str());

		/**/
		GameObject* acorn2 = Space->AddObject("acorn2");
		Sprite* cornsprite2 = acorn2->AddComponent<Sprite>();
		Transform* coinTrs2 = acorn2->AddComponent<Transform>();
		Animation* canim2 = acorn2->AddComponent<Animation>();
		UI * corn2 = acorn2->AddComponent<UI>();
		cornsprite2->Init(Shader_AnimatedTexture);
		cornsprite2->setPrior(3);
		canim2->Init(32, 16, 1, 1);
		canim2->setAnimationSpeed(0);
		(*canim2)[0][0] = 43;
		canim2->setSequence(0);
		cornsprite2->SetTexture(Tex_tile_set);
		cornsprite2->SetCamera(mainCam);
		coinTrs2->Scale(Vector2D(256.0f, 256.0f));
		cornsprite2->SetSize(64.0f, 64.0f);
		coinTrs2->Translation(Vector2D(1408.0f, -1024.0f));
		cornsprite2->SetPosition(1408.0f, -1024.0f);
		corn2->Set(mainCam.width() / 2 - 800.0f, mainCam.height() / 2 - 540.0f);
		FontRender* acornFont2 = acorn2->AddComponent<FontRender>();
		acornFont2->SetPos(970.0f, 550.0f);
		acornFont2->SetSize(1.0f);
		acornFont2->setMode(2);
		acornFont2->setType(3);
		acornFont2->SetText(L" /   20");

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Ambient_Wind"));
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play FinaleScene"));

		Audio::EventData data;
		data.eventName = "FinaleScene";
		data.paramName = "Hints";
		if (hintCount >= 20)
		{
			data.val = 20.1f;
		}
		else if (hintCount >= 15)
		{
			data.val = 15.1f;
		}
		else if (hintCount >= 10)
		{
			data.val = 10.1f;
		}
		Audio::EditParam(&data);
	}
}
