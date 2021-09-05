/******************************************************************************/
/*!
\file howtoplay.cpp
\author Justice Mealer
\par    email: j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../system/timermanager.h"
#include "../system/countdowntimer.h"
#include "../messaging/messagehandler.h"
#include "../graphics/FontRender.h"
#include "../component/MultiEmitter.h"


namespace Spaces
{


	void BuildHowToPlaySpace()
	{
		Space* htps = spaceManager->AddSpace("6090 HowToPlay");
		////
		GameObject* htpw1 = htps->AddObject("howtoplayTitle1");
		Transform* htpw1Trans = htpw1->AddComponent<Transform>();
		htpw1Trans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		UI* htp1Title = htpw1->AddComponent<UI>();
		htp1Title->Set(0.0f, 250.0f);
		FontRender* htp1Font = htpw1->AddComponent<FontRender>();
		htp1Font->SetPos(864.0f, 950.0f);
		htp1Font->SetSize(1.1f);
		htp1Font->setType(3);
		htp1Font->setMode(2);
		htp1Font->setTranslation(false);
		htp1Font->SetText(L"How To");
		////
		GameObject* htpw2 = htps->AddObject("howtoplayTitle2");
		Transform* htpw2Trans = htpw2->AddComponent<Transform>();
		htpw2Trans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		UI* htp2Title = htpw2->AddComponent<UI>();
		htp2Title->Set(0.0f, 250.0f);
		FontRender* htp2Font = htpw2->AddComponent<FontRender>();
		htp2Font->SetPos(864.0f, 850.0f);
		htp2Font->SetSize(1.1f);
		htp2Font->setType(3);
		htp2Font->setMode(2);
		htp2Font->setTranslation(false);
		htp2Font->SetText(L"Play");
		////
		GameObject* htpmove = htps->AddObject("HowToPlayMove");
		Sprite* sprite = htpmove->AddComponent<Sprite>();
		sprite->Init(Shader_AnimatedTexture);
		sprite->SetTexture(Tex_TutorialMove);
		Transform* trans = htpmove->AddComponent<Transform>();
		trans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		trans->Scale(Vector2D(512.0f, 512.0f));
		sprite->SetPosition(mainCam.center().x, mainCam.center().y);
		sprite->SetSize(512.0f, 512.0f);
		sprite->SetCamera(mainCam);
		UI* u = htpmove->AddComponent<UI>();
		u->Set(-512.f,194.0f);
		Animation* anim = htpmove->AddComponent<Animation>();
		anim->Init(1, 2, 1, 2, .2f);
		(*anim)[0][0] = 0;
		(*anim)[0][1] = 1;
		anim->setSequence(0);
		FontRender* moveText = htpmove->AddComponent<FontRender>();
		moveText->SetPos(864.0f - 510.0f, 490.0f);
		moveText->SetSize(0.35f);
		moveText->setMode(2);
		moveText->SetText(L"WASD to Move");
		

		GameObject* htpbowl = htps->AddObject("HowToPlayBowl");
		Sprite* sprite2 = htpbowl->AddComponent<Sprite>();
		sprite2->Init(Shader_AnimatedTexture);
		sprite2->SetTexture(Tex_TutorialBowl);
		Transform* trans2 = htpbowl->AddComponent<Transform>();
		trans2->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		trans2->Scale(Vector2D(512.0f, 256.0f));
		sprite2->SetPosition(mainCam.center().x, mainCam.center().y);
		sprite2->SetSize(512.0f, 256.0f);
		sprite2->SetCamera(mainCam);
		UI* u2 = htpbowl->AddComponent<UI>();
		u2->Set(512.f,194.f);
		Animation* anim2 = htpbowl->AddComponent<Animation>();
		anim2->Init(1, 2, 1, 2, .2f);
		(*anim2)[0][0] = 0;
		(*anim2)[0][1] = 1;
		anim2->setSequence(0);
		FontRender* bowlText = htpbowl->AddComponent<FontRender>();
		bowlText->SetPos(864.0f + 510.0f, 490.0f);
		bowlText->SetSize(0.35f);
		bowlText->setMode(2);
		bowlText->SetText(L"Magenta: Turn  |  Cyan: Forward");

		GameObject* htpcoin = htps->AddObject("HowToPlayCoin");
		Sprite* sprite3 = htpcoin->AddComponent<Sprite>();
		sprite3->Init(Shader_AnimatedTexture);
		sprite3->SetTexture(Tex_TutorialHint);
		Transform* trans3 = htpcoin->AddComponent<Transform>();
		trans3->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		trans3->Scale(Vector2D(256.0f, 256.0f));
		sprite3->SetPosition(mainCam.center().x, mainCam.center().y);
		sprite3->SetSize(256.0f, 256.0f);
		sprite3->SetCamera(mainCam);
		UI* u3 = htpcoin->AddComponent<UI>();
		u3->Set(512.f,-316.f);
		Animation* anim3 = htpcoin->AddComponent<Animation>();
		anim3->Init(1, 2, 1, 2, .2f);
		(*anim3)[0][0] = 0;
		(*anim3)[0][1] = 1;
		anim3->setSequence(0);
		FontRender* coinText = htpcoin->AddComponent<FontRender>();
		coinText->SetPos(864.0f + 510.0f, 70.0f);
		coinText->SetSize(0.35f);
		coinText->setMode(2);
		coinText->SetText(L"Collect the secret coins");

		GameObject* htploop = htps->AddObject("HowToPlayLoop");
		Sprite* sprite4 = htploop->AddComponent<Sprite>();
		sprite4->Init(Shader_AnimatedTexture);
		sprite4->SetTexture(Tex_TutorialLoop);
		Transform* trans4 = htploop->AddComponent<Transform>();
		trans4->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		trans4->Scale(Vector2D(512.0f, 256.0f));
		sprite4->SetPosition(mainCam.center().x, mainCam.center().y);
		sprite4->SetSize(512.0f, 256.0f);
		sprite4->SetCamera(mainCam);
		UI* u4 = htploop->AddComponent<UI>();
		u4->Set(-570.f,-316.f);
		Animation* anim4 = htploop->AddComponent<Animation>();
		anim4->Init(1, 2, 1, 2, .2f);
		(*anim4)[0][0] = 0;
		(*anim4)[0][1] = 1;
		anim4->setSequence(0);
		FontRender* loopText = htploop->AddComponent<FontRender>();
		loopText->SetPos(864.0f - 510.0f, 70.0f);
		loopText->SetSize(0.35f);
		loopText->setMode(2);
		loopText->SetText(L"Make a loop");

		/////
		GameObject* exit = htps->AddObject("Tutorial Back");
		Sprite* exitSprite = exit->AddComponent<Sprite>();
		exitSprite->Init(Shader_BasicTexture);
		exitSprite->SetTexture(Tex_Box);
		Transform* exitTrans = exit->AddComponent<Transform>();
		exitTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		exitTrans->Scale(Vector2D(1.0f, 1.0f));
		exitSprite->SetPosition(mainCam.center().x, mainCam.center().y);
		exitSprite->SetSize(1.0f, 1.0f);
		exitSprite->SetCamera(mainCam);
		UI* end = exit->AddComponent<UI>();
		end->Set(0.0f, -450.0f);
		FontRender* backText = exit->AddComponent<FontRender>();
		backText->SetPos(864.0f, 50.0f);
		backText->SetSize(0.5f);
		backText->setMode(2);
		backText->SetText(L"Back");
		////
		GameObject* selector = htps->AddObject("selector");
		Sprite* selectSprite = selector->AddComponent<Sprite>();
		MultiEmitter* emit = selector->AddComponent<MultiEmitter>();
		selectSprite->Init(Shader_BasicTexture);
		selectSprite->SetTexture(Tex_select);
		selectSprite->setAlpha(0.0f);
		Transform* selectTrans = selector->AddComponent<Transform>();
		selectTrans->Translation(Vector2D(mainCam.center().x, mainCam.center().y - 900.0f));
		selectTrans->Scale(Vector2D(64.0f, 64.0f));
		selectSprite->SetPosition(mainCam.up().x, mainCam.up().y);
		selectSprite->SetSize(64.0f, 64.0f);
		selectSprite->SetCamera(mainCam);
		Selector* select = selector->AddComponent<Selector>();
		emit->addEmitter("SelectorMirowed.json");
		emit->addEmitter("Selector.json");
		select->Set(-200, -40);
		select->AddButton(exit);
		
	}
}