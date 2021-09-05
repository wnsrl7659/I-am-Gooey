// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.

#include <sstream>
#include <functional>
#include <boost/locale.hpp>

#include "spacemanager.h"
#include "../settings/settings.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../system/TrueType.h"
#include "../graphics/FontRender.h"
#include "../component/MultiEmitter.h"
#include "../audio/audiomanager.h"
#include "../component/pscript.h"
#include "../audio/audiomanager.h"
#include "../system/localize.h"

namespace Spaces
{
	static void _script_update_vol(float, GameObject * obj, AudioManager::VolType t)
	{
		if (obj)
		{
			FontRender * text = obj->GetComponent<FontRender>();
			if (text)
			{
				std::wstringstream ss;
				switch (t)
				{
				case AudioManager::VolType::Master:
					ss << Settings::masterVol;
					break;
				case AudioManager::VolType::Music:
					ss << Settings::musicVol;
					break;
				case AudioManager::VolType::Sfx:
					ss << Settings::sfxVol;
					break;
				default:
					ss << L"Error";
					break;
				}
				text->SetText(ss.str());
			}
		}
	}


	static void _script_update_text(float, GameObject * obj, std::wstring string)
	{
		if (obj)
		{
			FontRender * text = obj->GetComponent<FontRender>();
			if (text)
			{
				auto txt = boost::locale::translate(string);
				text->SetText(txt.str());
			}
		}
	}

  static void _script_update_lang(float, GameObject * obj)
  {
    if (obj)
    {
      FontRender * text = obj->GetComponent<FontRender>();
      if (text)
      {
        std::string loc = System::lochandler->GetLocale();
        std::wstring lang = System::lochandler->langMap.at(loc);
		auto txt = boost::locale::translate(lang);
        text->SetText(txt.str());
      }
    }
  }

	/**************************************************************************/
	/*!
	\brief Builds Menu Buttons part of Main Menu Space
	*/
	/**************************************************************************/
	static void BuildMenu()
	{
		Space * buttonSpace = spaceManager->AddSpace("7776 ButtonSpace");

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

		rightpos = 1525.0f;

        //// Master volume
		GameObject* play = buttonSpace->AddObject("masterV");
		Transform* pltransform2 = play->AddComponent<Transform>();
		pltransform2->Translation(Vector2D(mainCam.center().x, mainCam.center().y + 50.0f));
		pltransform2->Scale(Vector2D(256.0f, 128.0f));
		UI* plpauseButton2 = play->AddComponent<UI>();
		plpauseButton2->Set(705.0f, 110.0f);
		FontRender* plFont = play->AddComponent<FontRender>();
		plFont->SetPos(rightpos, 670.0f);
		plFont->SetSize(0.35f);
		plFont->SetText(boost::locale::translate(L"Master Volume"));
		plFont->setMode(2);
		p_script * master_vol_script = play->AddComponent<p_script>();
		master_vol_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Master Volume"));

		GameObject* number1 = buttonSpace->AddObject("masterVn");
		FontRender* n1Font = number1->AddComponent<FontRender>();
		n1Font->SetPos(rightpos, 630.0f);
		n1Font->SetSize(0.30f);
		std::wstringstream ss;
		ss << Settings::masterVol;
		n1Font->SetText(ss.str());
		n1Font->setMode(2);
		ss.str(std::wstring());
		ss.clear();
		p_script * mv_script = number1->AddComponent<p_script>();
		mv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Master));
		////
		GameObject* options = buttonSpace->AddObject("BGMV");
		Transform* optransform = options->AddComponent<Transform>();
		optransform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		optransform->Scale(Vector2D(256.0f, 128.0f));
		UI* oppauseButton = options->AddComponent<UI>();
		oppauseButton->Set(705.0f, -5.0f);
		FontRender* opFont = options->AddComponent<FontRender>();
		opFont->SetPos(rightpos, 555.0f);
		opFont->SetSize(0.35f);
		opFont->SetText(boost::locale::translate(L"Music Volume"));
		opFont->setMode(2);
		p_script * ms_vol_script = options->AddComponent<p_script>();
		ms_vol_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Music Volume"));

		GameObject* number2 = buttonSpace->AddObject("BGMVn");
		FontRender* n2Font = number2->AddComponent<FontRender>();
		n2Font->SetPos(rightpos, 515.0f);
		n2Font->SetSize(0.30f);
		ss << Settings::musicVol;
		n2Font->SetText(ss.str());
		n2Font->setMode(2);
		ss.str(std::wstring());
		ss.clear();
		p_script * bv_script = number2->AddComponent<p_script>();
		bv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Music));
		////
		GameObject* credits = buttonSpace->AddObject("EffectV");
		Transform* ctransform = credits->AddComponent<Transform>();
		ctransform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		ctransform->Scale(Vector2D(256.0f, 128.0f));
		UI* cpauseButton = credits->AddComponent<UI>();
		cpauseButton->Set(705.0f, -120.0f);
		FontRender* cFont = credits->AddComponent<FontRender>();
		cFont->SetPos(rightpos, 440.0f);
		cFont->SetSize(0.35f);
		cFont->SetText(boost::locale::translate(L"SFX Volume"));
		cFont->setMode(2);
		p_script * sfx_script = credits->AddComponent<p_script>();
		sfx_script->push_front(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"SFX Volume"));

		GameObject* number3 = buttonSpace->AddObject("EffectVn");
		FontRender* n3Font = number3->AddComponent<FontRender>();
		n3Font->SetPos(rightpos, 400.0f);
		n3Font->SetSize(0.30f);
		ss << Settings::sfxVol;
		n3Font->SetText(ss.str());
		n3Font->setMode(2);
		ss.str(std::wstring());
		ss.clear();
		p_script * sv_script = number3->AddComponent<p_script>();
		sv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Sfx));
		////
		GameObject* mute = buttonSpace->AddObject("MuteV");
		Transform* mutrans = mute->AddComponent<Transform>();
		mutrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		mutrans->Scale(Vector2D(256.0f, 128.0f));
		UI* mupause = mute->AddComponent<UI>();
		mupause->Set(705.0f, -215.0f);
		FontRender* muFont = mute->AddComponent<FontRender>();
		muFont->SetPos(rightpos, 330.0f);
		muFont->SetSize(0.35f);
		muFont->setMode(2);
		muFont->SetText(L"Mute");
		p_script * mut_script = mute->AddComponent<p_script>();
		mut_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Mute"));

		////
		GameObject* quit = buttonSpace->AddObject("Language");
		Transform* qtransform3 = quit->AddComponent<Transform>();
		qtransform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		qtransform3->Scale(Vector2D(256.0f, 128.0f));
		UI* qpauseButton3 = quit->AddComponent<UI>();
		qpauseButton3->Set(705.0f, -285.0f);
		FontRender* qFont = quit->AddComponent<FontRender>();
		qFont->SetPos(rightpos, 260.0f);
		qFont->SetSize(0.35f);
    qFont->SetText(L"Language");
    qFont->setMode(2);
    p_script * font_script = quit->AddComponent<p_script>();
    font_script->push_front(_script_update_lang);
		////
		GameObject* screen = buttonSpace->AddObject("Screen Toggle");
		Transform* stransform3 = screen->AddComponent<Transform>();
		stransform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		stransform3->Scale(Vector2D(256.0f, 128.0f));
		UI* spauseButton3 = screen->AddComponent<UI>();
		spauseButton3->Set(705.0f, -355.0f);
		FontRender* sFont = screen->AddComponent<FontRender>();
		sFont->SetPos(rightpos, 190.0f);
		sFont->SetSize(0.35f);
		sFont->SetText(boost::locale::translate(L"Fullscreen"));
		sFont->setMode(2);
		fullscreentextswtich = true;
		p_script * full_script = screen->AddComponent<p_script>();
		full_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Fullscreen"));
		////
		GameObject* back = buttonSpace->AddObject("BBack Game");
		Transform* btransform3 = back->AddComponent<Transform>();
		btransform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		btransform3->Scale(Vector2D(256.0f, 128.0f));
		UI* bpauseButton3 = back->AddComponent<UI>();
		bpauseButton3->Set(705.0f, -455.0f);
		FontRender* bFont = back->AddComponent<FontRender>();
		bFont->SetPos(rightpos, 90.0f);
		bFont->SetSize(0.35f);
		bFont->SetText(boost::locale::translate(L"Back"));
		bFont->setMode(2);
		p_script * bk_script = back->AddComponent<p_script>();
		bk_script->push_front(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Back"));
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
		emit->addEmitter("SelectorMirowed.json");
		emit->addEmitter("Selector.json");
		select->Set(-240.0f, 0);
		select->AddButton(play);
		select->AddButton(options);
		select->AddButton(credits);
		select->AddButton(mute);
		select->AddButton(quit);
		select->AddButton(screen);
		select->AddButton(back);
		////
	}

	void BuildOptionMenu()
	{
		mainCam.SetPosition(0, 0);

		Space * NewMenu = spaceManager->AddSpace("7778 SuperDooperCoolSpace - Copy");
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