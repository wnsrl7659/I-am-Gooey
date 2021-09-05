// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.

#include "spacemanager.h"
#include "../settings/settings.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../system/TrueType.h"
#include "../graphics/FontRender.h"
#include "../component/MultiEmitter.h"
#include "../audio/audiomanager.h"


namespace Spaces
{
	/**************************************************************************/
	/*!
	\brief Builds Menu Buttons part of Main Menu Space
	*/
	/**************************************************************************/
	static void BuildMenu()
	{
		Spaces::spaceManager->currentScreen_ = Spaces::Screens::MainMenu;
		Space * buttonSpace = spaceManager->AddSpace("4 ButtonSpace");

		float rightpos = 1680.0f;
		GameObject* titleone = buttonSpace->AddObject("Titleone");
		FontRender* tiofont = titleone->AddComponent<FontRender>();
		tiofont->SetPos(rightpos - 100, 900.0f);
		tiofont->SetSize(1.0f);
		tiofont->SetText(L"I   Am");
		tiofont->setMode(1);
		tiofont->setType(3);
		tiofont->setTranslation(false);
		GameObject* titletwo = buttonSpace->AddObject("Titletwo");
		FontRender* titfont = titletwo->AddComponent<FontRender>();
		titfont->SetPos(rightpos, 800.0f);
		titfont->SetSize(1.0f);
		titfont->SetText(L"Gooey");
		titfont->setMode(1);
		titfont->setType(3);
		//UI* tpauseButton = titleone->AddComponent<UI>();
		//tpauseButton->Set(580.0f, 255.0f);
		//titfont->setTranslation(false);

		////
		/*
		GameObject* title = buttonSpace->AddObject("Title");
		Sprite* tsprite = title->AddComponent<Sprite>();
		tsprite->Init(Shader_BasicTexture);
		tsprite->SetTexture(Tex_title);
		Transform* ttransform = title->AddComponent<Transform>();
		ttransform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		ttransform->Scale(Vector2D(512.0f, 512.0f));
		tsprite->SetPosition(mainCam.up().x, mainCam.up().y);
		tsprite->SetSize(512.0f, 512.0f);
		tsprite->SetCamera(mainCam);
		UI* tpauseButton = title->AddComponent<UI>();
		tpauseButton->Set(580.0f, 255.0f);
		*/
		////
		GameObject* options = buttonSpace->AddObject("Options");
		Sprite* opsprite = options->AddComponent<Sprite>();
		opsprite->Init(Shader_BasicTexture);
		opsprite->SetTexture(Tex_options);
		opsprite->setPrior(2);
		opsprite->setAlpha(0.0f);
		Transform* optransform = options->AddComponent<Transform>();
		optransform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		optransform->Scale(Vector2D(256.0f, 128.0f));
		opsprite->SetPosition(mainCam.up().x, mainCam.up().y);
		opsprite->SetSize(256.0f, 128.0f);
		opsprite->SetCamera(mainCam);
		UI* oppauseButton = options->AddComponent<UI>();
		oppauseButton->Set(705.0f, -150.0f);
		FontRender* opFont = options->AddComponent<FontRender>();
		opFont->SetPos(rightpos, 390.0f);
		opFont->SetSize(0.5f);
		opFont->SetText(L"Options");
		opFont->setMode(1);

		////
		GameObject* play = buttonSpace->AddObject("Play Game");
		Sprite* plsprite2 = play->AddComponent<Sprite>();
		plsprite2->Init(Shader_BasicTexture);
		plsprite2->SetTexture(Tex_play);
		plsprite2->setPrior(2);
		plsprite2->setAlpha(0.0f);
		Transform* pltransform2 = play->AddComponent<Transform>();
		pltransform2->Translation(Vector2D(mainCam.center().x, mainCam.center().y + 50.0f));
		pltransform2->Scale(Vector2D(256.0f, 128.0f));
		plsprite2->SetPosition(mainCam.up().x, mainCam.up().y + 50.0f);
		plsprite2->SetSize(256.0f, 128.0f);
		plsprite2->SetCamera(mainCam);
		UI* plpauseButton2 = play->AddComponent<UI>();
		plpauseButton2->Set(705.0f, -50.0f);
		FontRender* plFont = play->AddComponent<FontRender>();
		plFont->SetPos(rightpos, 490.0f);
		plFont->SetSize(0.5f);
		plFont->SetText(L"Play");
		plFont->setMode(1);
		////
		GameObject* quit = buttonSpace->AddObject("Quit Game");
		Sprite* qsprite3 = quit->AddComponent<Sprite>();
		qsprite3->Init(Shader_BasicTexture);
		qsprite3->SetTexture(Tex_quit);
		qsprite3->setPrior(2);
		qsprite3->setAlpha(0.0f);
		Transform* qtransform3 = quit->AddComponent<Transform>();
		qtransform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		qtransform3->Scale(Vector2D(256.0f, 128.0f));
		qsprite3->SetPosition(mainCam.up().x, mainCam.up().y);
		qsprite3->SetSize(256.0f, 128.0f);
		qsprite3->SetCamera(mainCam);
		UI* qpauseButton3 = quit->AddComponent<UI>();
		qpauseButton3->Set(705.0f, -350.0f);
		FontRender* qFont = quit->AddComponent<FontRender>();
		qFont->SetPos(rightpos, 190.0f);
		qFont->SetSize(0.5f);
		qFont->SetText(L"Quit");
		qFont->setMode(1);
		////
		GameObject* credits = buttonSpace->AddObject("Credits");
		Sprite* csprite = credits->AddComponent<Sprite>();
		csprite->Init(Shader_BasicTexture);
		csprite->SetTexture(Tex_credits);
		csprite->setPrior(2);
		csprite->setAlpha(0.0f);
		Transform* ctransform = credits->AddComponent<Transform>();
		ctransform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		ctransform->Scale(Vector2D(256.0f, 128.0f));
		csprite->SetPosition(mainCam.up().x, mainCam.up().y);
		csprite->SetSize(256.0f, 128.0f);
		csprite->SetCamera(mainCam);
		UI* cpauseButton = credits->AddComponent<UI>();
		cpauseButton->Set(705.0f, -250.0f);
		FontRender* cFont = credits->AddComponent<FontRender>();
		cFont->SetPos(rightpos, 290.0f);
		cFont->SetSize(0.5f);
		cFont->SetText(L"Credits");
		cFont->setMode(1);
		////
		GameObject* selector = buttonSpace->AddObject("selector");
		Sprite* selectSprite = selector->AddComponent<Sprite>();
		MultiEmitter* emit = selector->AddComponent<MultiEmitter>();
		selectSprite->Init(Shader_BasicTexture);
		selectSprite->SetTexture(Tex_select);
		selectSprite->setPrior(3);
		selectSprite->setData(7659);
		selectSprite->setAlpha(0.0f);
		Transform* selectTrans = selector->AddComponent<Transform>();
		selectTrans->Translation(Vector2D(mainCam.center().x + 864.0f, mainCam.center().y));
		selectTrans->Scale(Vector2D(64.0f, 64.0f));
		selectSprite->SetPosition(mainCam.up().x, mainCam.up().y);
		selectSprite->SetSize(64.0f, 64.0f);
		selectSprite->SetCamera(mainCam);
		Selector* select = selector->AddComponent<Selector>();
		emit->addEmitter("Selector.json");
		select->Set(-225.0f, 0);
		select->AddButton(play);
		select->AddButton(options);
		select->AddButton(credits);
		select->AddButton(quit);
		////
	}

	void BuildMainMenu()
	{
		mainCam.SetPosition(0, 0);
		spaceManager->SetSpace(Screens::MainMenu);
		Messaging::messHandler->HandleMessage<std::string>("level event", &std::string("mainmenu"));
		Settings::culling = false;

		Space * NewMenu = spaceManager->AddSpace("00 SuperDooperCoolSpace");
		glm::vec2 camCenter = mainCam.center();
		Vector2D cambottom(camCenter[0], camCenter[1] - mainCam.height()/2.f);

		//BACKGROUND
		GameObject* Background = NewMenu->AddObject("Background");
		Sprite* sprBG = Background->AddComponent<Sprite>();
		Transform* trsBG = Background->AddComponent<Transform>();
		MultiEmitter* meBG = Background->AddComponent<MultiEmitter>();
		meBG->addEmitter("waves.json");
		meBG->addEmitter("waves2.json");
		meBG->addEmitter("sky.json");
		sprBG->Init(Shader_BasicTexture);
		sprBG->SetTexture(Tex_MenuBG);
		sprBG->setPrior(0);
		trsBG->Translation(cambottom + Vector2D(0, mainCam.height() / 2.f));
		trsBG->Scale(Vector2D(mainCam.width(), mainCam.height()));

		//Ground
		GameObject* ground = NewMenu->AddObject("Ground");
		Sprite* sprGround = ground->AddComponent<Sprite>();
		Transform* trsGround = ground->AddComponent<Transform>();
		MultiEmitter* GroundEM = ground->AddComponent<MultiEmitter>();
		sprGround->Init(Shader_BasicTexture);
		sprGround->SetTexture(Tex_MenuGround);
		sprGround->setPrior(2);
		trsGround->Scale(Vector2D(mainCam.width(), mainCam.height() / 3.f));
		trsGround->Translation(cambottom + Vector2D(0.f, trsGround->Scale()[1] / 1.5f));

		//TREES FG
		GameObject* Foreground = NewMenu->AddObject("TreesForeground");
		Sprite* sprFG = Foreground->AddComponent<Sprite>();
		Transform* trsFG = Foreground->AddComponent<Transform>();
		MultiEmitter* FGEM = Foreground->AddComponent<MultiEmitter>();
		sprFG->Init(Shader_BasicTexture);
		sprFG->SetTexture(Tex_TreesFG);
		sprFG->setPrior(3);
		trsFG->Scale(Vector2D(mainCam.width(), mainCam.height() / 3.f));
		trsFG->Translation(cambottom + Vector2D(0.f, trsFG->Scale()[1]/2.f));

		//TREES BACK
		GameObject* TreesBack = NewMenu->AddObject("TreesBackground");
		Sprite* sprTBG = TreesBack->AddComponent<Sprite>();
		Transform* trsTBG = TreesBack->AddComponent<Transform>();
		MultiEmitter* TBGEM = TreesBack->AddComponent<MultiEmitter>();
		sprTBG->Init(Shader_BasicTexture);
		sprTBG->SetTexture(Tex_TreesBG);
		sprTBG->setPrior(2);
		trsTBG->Scale(Vector2D(mainCam.width()*.7f, mainCam.height() / 2.4f));
		trsTBG->Translation(cambottom + Vector2D((-mainCam.width() / 2.f) + (trsTBG->Scale()[0] / 2.f), trsTBG->Scale()[1]/1.25f));

		

		//TOWER
		GameObject* Tower = NewMenu->AddObject("Tower");
		Sprite* sprTower = Tower->AddComponent<Sprite>();
		Transform* trsTower = Tower->AddComponent<Transform>();
		sprTower->Init(Shader_BasicTexture);
		sprTower->SetTexture(Tex_Tower);
		sprTower->setPrior(2);
		trsTower->Scale(Vector2D(390, mainCam.height() / 1.1f));
		trsTower->Translation(cambottom + Vector2D(-mainCam.width()/5.5f, trsTower->Scale()[1]/2.f));
		
		//BLACKFADE
		GameObject* Fade = NewMenu->AddObject("zFade");
		Sprite* sprFade = Fade->AddComponent<Sprite>();
		Transform* trsFade = Fade->AddComponent<Transform>();
		MultiEmitter* meFade = Fade->AddComponent<MultiEmitter>();
		sprFade->Init(Shader_BasicTexture);
		sprFade->SetTexture(Tex_LeftRightFade);
		sprFade->setPrior(3);
		trsFade->Scale(Vector2D(mainCam.width()/3.f, mainCam.height()));
		trsFade->Translation(Vector2D(mainCam.center().x + mainCam.width()/2.f - trsFade->Scale()[0]/2.f, mainCam.center().y));


		BuildMenu();

	}

	
}