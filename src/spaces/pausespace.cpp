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

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../graphics/FontRender.h"
#include "../component/MultiEmitter.h"

namespace Spaces
{
	/**************************************************************************/
	/*!
	\brief Builds Pause Space
	*/
	/**************************************************************************/
	void BuildPauseSpace()
	{

		Space* pauseSpace = spaceManager->AddSpace("777 pausespace");
		////
		GameObject* pauseWord = pauseSpace->AddObject("pauseWord");
		Transform* pauseTrans = pauseWord->AddComponent<Transform>();
		pauseTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		UI* pauseTitle = pauseWord->AddComponent<UI>();
		pauseTitle->Set(0.0f, 250.0f);
		FontRender* paFont = pauseWord->AddComponent<FontRender>();
		paFont->SetPos(864.0f, 560.0f + 250.0f - 16.0f);
		paFont->SetSize(1.5f);
		paFont->setType(3);
		paFont->setMode(2);
		paFont->setTranslation(false);
		paFont->SetText(L"Paused");
		////
		GameObject* options = pauseSpace->AddObject("pOptions");
		Sprite* sprite = options->AddComponent<Sprite>();
		sprite->Init(Shader_BasicTexture);
		sprite->SetTexture(Tex_options);
		Transform* transform = options->AddComponent<Transform>();
		transform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		transform->Scale(Vector2D(256.0f, 128.0f));
		sprite->SetPosition(mainCam.up().x, mainCam.up().y);
		sprite->SetSize(256.0f, 128.0f);
		sprite->SetCamera(mainCam);
		UI* pauseButton = options->AddComponent<UI>();
		pauseButton->Set(0.0f, -50.0f);
		FontRender* opFont = options->AddComponent<FontRender>();
		opFont->SetPos(864.0f, 560.0f - 50.0f - 16.0f);
		opFont->SetSize(0.5f);
		opFont->setMode(2);
		opFont->SetText(L"Options");
		////
		GameObject* resumeGame = pauseSpace->AddObject("Resume Game");
		Sprite* sprite2 = resumeGame->AddComponent<Sprite>();
		sprite2->Init(Shader_BasicTexture);
		sprite2->SetTexture(Tex_resume);
		Transform* transform2 = resumeGame->AddComponent<Transform>();
		transform2->Translation(Vector2D(mainCam.center().x, mainCam.center().y + 50.0f));
		transform2->Scale(Vector2D(256.0f, 128.0f));
		sprite2->SetPosition(mainCam.up().x, mainCam.up().y + 50.0f);
		sprite2->SetSize(256.0f, 128.0f);
		sprite2->SetCamera(mainCam);
		UI* pauseButton2 = resumeGame->AddComponent<UI>();
		pauseButton2->Set(0.0f, 55.0f);
		FontRender* reFont = resumeGame->AddComponent<FontRender>();
		reFont->SetPos(864.0f, 560.0f + 50.0f - 16.0f);
		reFont->SetSize(0.5f);
		reFont->setMode(2);
		reFont->SetText(L"Resume");
		////
		GameObject* quitGame = pauseSpace->AddObject("Quit Game");
		Sprite* sprite3 = quitGame->AddComponent<Sprite>();
		sprite3->Init(Shader_BasicTexture);
		sprite3->SetTexture(Tex_quit);
		Transform* transform3 = quitGame->AddComponent<Transform>();
		transform3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		transform3->Scale(Vector2D(256.0f, 128.0f));
		sprite3->SetPosition(mainCam.up().x, mainCam.up().y);
		sprite3->SetSize(256.0f, 128.0f);
		sprite3->SetCamera(mainCam);
		UI* pauseButton3 = quitGame->AddComponent<UI>();
		pauseButton3->Set(0.0f, -345.0f);
		FontRender* qFont = quitGame->AddComponent<FontRender>();
		qFont->SetPos(864.0f, 560.0f - 350.0f - 16.0f);
		qFont->SetSize(0.5f);
		qFont->setMode(2);
		qFont->SetText(L"Quit");
		//////
		GameObject* howtoplay = pauseSpace->AddObject("Tutorial");
		Sprite* htpSprite = howtoplay->AddComponent<Sprite>();
		htpSprite->Init(Shader_BasicTexture);
		htpSprite->SetTexture(Tex_credits);
		Transform* htpTrans = howtoplay->AddComponent<Transform>();
		htpTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		htpTrans->Scale(Vector2D(256.0f, 128.0f));
		htpSprite->SetPosition(mainCam.up().x, mainCam.up().y);
		htpSprite->SetSize(256.0f, 128.0f);
		htpSprite->SetCamera(mainCam);
		UI* htpPause = howtoplay->AddComponent<UI>();
		htpPause->Set(0.0f, -145.0f);
		FontRender* htpFont = howtoplay->AddComponent<FontRender>();
		htpFont->SetPos(864.0f, 560.0f - 150.0f - 16.0f);
		htpFont->SetSize(0.5f);
		htpFont->setMode(2);
		htpFont->SetText(L"How to Play");
		////
		GameObject* menu = pauseSpace->AddObject("menu");
		Sprite* msprite = menu->AddComponent<Sprite>();
		msprite->Init(Shader_BasicTexture);
		msprite->SetTexture(Tex_menu);
		Transform* mtrans = menu->AddComponent<Transform>();
		mtrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		mtrans->Scale(Vector2D(256.0f, 128.0f));
		msprite->SetPosition(mainCam.up().x, mainCam.up().y);
		msprite->SetSize(256.0f, 128.0f);
		msprite->SetCamera(mainCam);
		UI* mpause = menu->AddComponent<UI>();
		mpause->Set(0.0f, -245.0f);
		FontRender* mFont = menu->AddComponent<FontRender>();
		mFont->SetPos(864.0f, 560.0f - 250.0f - 16.0f);
		mFont->SetSize(0.5f);
		mFont->setMode(2);
		mFont->SetText(L"Menu");
		////
		GameObject* selector = pauseSpace->AddObject("selector");
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
		select->AddButton(resumeGame);
		select->AddButton(options);
		select->AddButton(howtoplay);
		select->AddButton(menu);
		select->AddButton(quitGame);
		////
		pauseSpace->Pause();
	}
}