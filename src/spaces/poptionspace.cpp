/******************************************************************************/
/*!
\file   pausespace.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Pause Space File, creates the Pause menu when called, and immediately makes it inactive

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <sstream>
#include <functional>
#include <boost/locale.hpp>

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../graphics/FontRender.h"
#include "../component/MultiEmitter.h"
#include "../audio/audiomanager.h"
#include "../settings/settings.h"
#include "../system/localize.h"
#include "../component/pscript.h"

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
				std::string loc = System::GetLocale();
				std::wstring lang = System::lochandler->langMap.at(loc);
				text->SetText(boost::locale::translate(lang));
			}
		}
	}

	/**************************************************************************/
	/*!
	\brief Builds Pause Space
	*/
	/**************************************************************************/
	void BuildPoptionSpace()
	{

		Space* optionSpace = spaceManager->AddSpace("7777 poptionspace");
		////
		GameObject* optionWord = optionSpace->AddObject("optionWord");
		Transform* optionTrans = optionWord->AddComponent<Transform>();
		optionTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		UI* optionTitle = optionWord->AddComponent<UI>();
		optionTitle->Set(0.0f, 250.0f);
		FontRender* optionFont = optionWord->AddComponent<FontRender>();
		optionFont->SetPos(864.0f, 800.0f);
		optionFont->SetSize(1.5f);
		optionFont->setType(3);
		optionFont->setMode(2);
		optionFont->setTranslation(false);
		optionFont->SetText(boost::locale::translate(L"Options"));
		////
		GameObject* master = optionSpace->AddObject("masterV");
		Transform* mtrans = master->AddComponent<Transform>();
		mtrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		mtrans->Scale(Vector2D(256.0f, 128.0f));
		UI* mpause = master->AddComponent<UI>();
		mpause->Set(0.0f, 110.0f);
		FontRender* mFont = master->AddComponent<FontRender>();
		mFont->SetPos(864.0f, 670.0f);
		mFont->SetSize(0.35f);
		mFont->setMode(2);
		mFont->SetText(boost::locale::translate(L"Master Volume"));
		p_script * master_vol_script = master->AddComponent<p_script>();
		master_vol_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Master Volume"));
		GameObject* number1 = optionSpace->AddObject("masterVn");
		FontRender* n1Font = number1->AddComponent<FontRender>();
		n1Font->SetPos(864.0f, 630.0f);
		n1Font->SetSize(0.30f);
		n1Font->SetText(L"50");
		n1Font->setMode(2);
		p_script * mv_script = number1->AddComponent<p_script>();
		mv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Master));
		////
		GameObject* bgm = optionSpace->AddObject("BGMV");
		Transform* bgmtrans = bgm->AddComponent<Transform>();
		bgmtrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		bgmtrans->Scale(Vector2D(256.0f, 128.0f));
		UI* bgmpause = bgm->AddComponent<UI>();
		bgmpause->Set(0.0f, -5.0f);
		FontRender* bgmFont = bgm->AddComponent<FontRender>();
		bgmFont->SetPos(864.0f, 555.0f);
		bgmFont->SetSize(0.35f);
		bgmFont->setMode(2);
		bgmFont->SetText(boost::locale::translate(L"Music Volume"));
		GameObject* number2 = optionSpace->AddObject("BGMVn");
		FontRender* n2Font = number2->AddComponent<FontRender>();
		p_script * ms_vol_script = bgm->AddComponent<p_script>();
		ms_vol_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Music Volume"));

		n2Font->SetPos(864.0f, 515.0f);
		n2Font->SetSize(0.30f);
		n2Font->SetText(L"50");
		n2Font->setMode(2);
		p_script * bv_script = number2->AddComponent<p_script>();
		bv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Music));
		////
		GameObject* effect = optionSpace->AddObject("EffectV");
		Transform* etrans = effect->AddComponent<Transform>();
		etrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		etrans->Scale(Vector2D(256.0f, 128.0f));
		UI* epause = effect->AddComponent<UI>();
		epause->Set(0.0f, -120.0f);
		FontRender* eFont = effect->AddComponent<FontRender>();
		eFont->SetPos(864.0f, 440.0f);
		eFont->SetSize(0.35f);
		eFont->setMode(2);
		eFont->SetText(boost::locale::translate(L"SFX Volume"));
		p_script * sfx_script = effect->AddComponent<p_script>();
		sfx_script->push_front(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"SFX Volume"));

		GameObject* number3 = optionSpace->AddObject("EffectVn");
		FontRender* n3Font = number3->AddComponent<FontRender>();
		n3Font->SetPos(864.0f, 400.0f);
		n3Font->SetSize(0.30f);
		n3Font->SetText(L"50");
		n3Font->setMode(2);
		p_script * sv_script = number3->AddComponent<p_script>();
		sv_script->push_back(std::bind(_script_update_vol, std::placeholders::_1, std::placeholders::_2, AudioManager::VolType::Sfx));
		////
		GameObject* mute = optionSpace->AddObject("MuteV");
		Transform* mutrans = mute->AddComponent<Transform>();
		mutrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		mutrans->Scale(Vector2D(256.0f, 128.0f));
		UI* mupause = mute->AddComponent<UI>();
		mupause->Set(0.0f, -215.0f);
		FontRender* muFont = mute->AddComponent<FontRender>();
		muFont->SetPos(864.0f, 330.0f);
		muFont->SetSize(0.35f);
		muFont->setMode(2);
		muFont->SetText(boost::locale::translate(L"Mute"));
		p_script * mut_script = mute->AddComponent<p_script>();
		mut_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Mute"));

		////
		GameObject* lang = optionSpace->AddObject("Language");
		Transform* ltrans = lang->AddComponent<Transform>();
		ltrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		ltrans->Scale(Vector2D(256.0f, 128.0f));
		UI* lpause = lang->AddComponent<UI>();
		lpause->Set(0.0f, -285.0f);
		FontRender* lFont = lang->AddComponent<FontRender>();
		lFont->SetPos(864.0f, 260.0f);
		lFont->SetSize(0.35f);
		lFont->setMode(2);
		lFont->SetText(L"Language");
		p_script * lang_script = lang->AddComponent<p_script>();
		lang_script->push_front(_script_update_lang);

		////
		GameObject* screen = optionSpace->AddObject("Screen Toggle");
		Transform* stransform3 = screen->AddComponent<Transform>();
		stransform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		stransform3->Scale(Vector2D(256.0f, 128.0f));
		UI* spauseButton3 = screen->AddComponent<UI>();
		spauseButton3->Set(0.0f, -355.0f);
		FontRender* sFont = screen->AddComponent<FontRender>();
		sFont->SetPos(864.0f, 190.0f);
		sFont->SetSize(0.35f);
		sFont->SetText(boost::locale::translate(L"Fullscreen"));
		sFont->setMode(2);
		ffullscreentextswtich = true;
		p_script * full_script = screen->AddComponent<p_script>();
		full_script->push_back(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Fullscreen"));
		////
		GameObject* back = optionSpace->AddObject("Back Game");
		Transform* btrans = back->AddComponent<Transform>();
		btrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		btrans->Scale(Vector2D(256.0f, 128.0f));
		UI* bpause = back->AddComponent<UI>();
		bpause->Set(0.0f, -455.0f);
		FontRender* bFont = back->AddComponent<FontRender>();
		bFont->SetPos(864.0f, 90.0f);
		bFont->SetSize(0.35f);
		bFont->setMode(2);
		bFont->SetText(boost::locale::translate(L"Back"));
		p_script * bk_script = back->AddComponent<p_script>();
		bk_script->push_front(std::bind(_script_update_text, std::placeholders::_1, std::placeholders::_2, L"Back"));
		////
		GameObject* selector = optionSpace->AddObject("selector");
		Sprite* selectSprite = selector->AddComponent<Sprite>();
		MultiEmitter* emit = selector->AddComponent<MultiEmitter>();
		selectSprite->Init(Shader_BasicTexture);
		selectSprite->SetTexture(Tex_select);
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
		select->Set(-200.0f, 0);
		select->AddButton(master);
		select->AddButton(bgm);
		select->AddButton(effect);
		select->AddButton(mute);
		select->AddButton(lang);
		select->AddButton(screen);
		select->AddButton(back);

	}
}