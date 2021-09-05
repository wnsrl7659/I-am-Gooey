/******************************************************************************/
/*!
\file   playerspace.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  space for containing the player

\copyright    All content � 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../component/collider.h"
#include "../settings/settings.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../vfx/LightManager.h"
#include "../component/controller.h"
#include "../component/MultiEmitter.h"
#include "../messaging/messagehandler.h"
#include "../graphics/FontRender.h"
#include "../component/pscript.h"
#define SPRITESHEETHEIGHT 32
#define SPRITESHEETWIDTH 16

static void acornUpdate(float, GameObject* obj)
{
	Transform* trs = obj->GetComponent<Transform>();
	if (trs)
	{
		trs->Translation(Vector2D(mainCam.center()[0] + mainCam.width() / 2 - 150.0f, 
									mainCam.center()[1]+  mainCam.height() / 2 - 50.0f));
	}
}

namespace Spaces
{

  void PlayerCollisionHandler(GameObject * a, GameObject * b)
  {
	  if (a && b)
	  {
		  if (a->removing_ || b->removing_) return;
		  if (a->name.find("player") != std::string::npos)
		  {
			  /*if (dynamic_cast<Flower *>(b->GetComponent<Flower>()))
			  {
				  return;
			  }*/
			  if (dynamic_cast<Teleporter *>(b->GetComponent<Teleporter>()))
			  {
				  return;
			  }
			  if (b->name.find("Bowl") != std::string::npos) //hard for checking acorn. cuz acorn have wall collision
			  {
				  return;
			  }
			  if (b->name.find("hint") != std::string::npos)
			  {
				  return;
			  }
			  if (b->name.find("endga") != std::string::npos)
			  {
				  a->RemoveComponent<Controller>();
					a->GetComponent<Sprite>()->setAlpha(0.f);
					for (auto em : *a->GetComponent<MultiEmitter>()->getEmitters())
					{
						em->setSizeScaleChange(.4f);
					}
					LightSource* light = lightManager->GetLight("player");
					if (light)
					{
							light->Radius(light->Radius() * 5);
							light->BaseRadius(light->BaseRadius() * 10);
					}
					Physics* phys = a->GetComponent<Physics>();
					Transform* trs = a->GetComponent<Transform>();
					if (phys && trs)
					{
						Vector2D diff = trs->Translation() - Vector2D(1728.f, -976.f);
						phys->Velocity(diff * (1.f/2.f));
					}
							
				  return;
			  }
			  Sprite * bsprite = b->GetComponent<Sprite>();
			  if (bsprite->getData() == 129)
			  {
				  return;
			  }
			  Transform * transform = a->GetComponent<Transform>();
			  Physics * physics = a->GetComponent<Physics>();
			  DebugDraw * debugdraw = a->GetComponent<DebugDraw>();
			  
			  // set the transform to old transform, in theory this should 
			  // just move the player back to a position where they are no longer
			  // colliding and still able to move
			  if (transform)
			  {
				  //transform->Translation(transform->OldTranslation());
				  hitbywall = true;

				  switch(move)
				  {
				  case left:
					  move = right;
					  transform->Translation(Vector2D(transform->Translation()[0]+8, transform->Translation()[1]));
					  break;
				  case right:
					  move = left;
					  transform->Translation(Vector2D(transform->Translation()[0]-8, transform->Translation()[1]));
					  break;
				  case up:
					  move = down;
					  transform->Translation(Vector2D(transform->Translation()[0], transform->Translation()[1]-8));
					  break;
				  case down:
					  move = up;
					  transform->Translation(Vector2D(transform->Translation()[0], transform->Translation()[1]+8));
					  break;
				  default:
					  break;
				  }
				  nextmove = none;
				  //physics->Velocity(Vector2D());
				  Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Collision"));

			  }
			  if (debugdraw)
			  {
				 debugdraw->SetTempColor(1.0f, 0.0f, 0.0f);
			  }
		  }
		  else if (b->name.find("player") != std::string::npos)
		  {
			  Transform * transform = b->GetComponent<Transform>();
			  Physics * physics = b->GetComponent<Physics>();

			  //physics->Velocity(-physics->Velocity());
		  }
	  }
  }

	GameObject * BuildPlayerspace()
	{
		spaceManager->AddSpace("99999 lutespace");
		Space * playerspace = spaceManager->AddSpace("9999 playerspace");
		GameObject * player = playerspace->AddObject("player");
		
		Serial::Deserialize("defaultPlayer.json", player);
		Sprite* sprite = player->GetComponent<Sprite>();
		sprite->SetCamera(mainCam);
		sprite->setPrior(2);

	////////////////////
	GameObject* acorn = playerspace->AddObject("acorn");
	Sprite* cornsprite = acorn->AddComponent<Sprite>();
	Transform* ctransform = acorn->AddComponent<Transform>();
	Animation* canim = acorn->AddComponent<Animation>();
	//UI * corn = acorn->AddComponent<UI>();
	p_script* coinScript = acorn->AddComponent<p_script>();
	
	coinScript->push_back(acornUpdate);

	//sprite->setShader(Shader_SolidColor);

	cornsprite->Init(Shader_AnimatedTexture);

	cornsprite->setPrior(3);
	//cornsprite->setData(43);
	canim->Init(SPRITESHEETHEIGHT, SPRITESHEETWIDTH, 1, 1);
	canim->setAnimationSpeed(0);
	(*canim)[0][0] = 43;
	canim->setSequence(0);

	cornsprite->SetTexture(Tex_tile_set);
	cornsprite->SetCamera(mainCam);

	ctransform->Scale(Vector2D(256.0f, 256.0f));
	cornsprite->SetSize(64.0f, 64.0f);

	ctransform->Translation(Vector2D(1408.0f, -1024.0f));
	cornsprite->SetPosition(1408.0f, -1024.0f);

	//corn->Set(mainCam.width() / 2 - 150.0f, mainCam.height() / 2 - 50.0f);
	////////////////////
	GameObject* number = playerspace->AddObject("number");
	Sprite* numsprite = number->AddComponent<Sprite>();
	Transform* numtransform = number->AddComponent<Transform>();
	Animation* numanim = number->AddComponent<Animation>();
	Number * num = number->AddComponent<Number>();

	numsprite->Init(Shader_AnimatedTexture);
	numsprite->setPrior(3);
	numsprite->setAlpha(0.0f);

	numanim->Init(2, 8, 1, 1);
	numanim->setAnimationSpeed(0);
	(*numanim)[0][0] = 0;
	numanim->setSequence(0);

	numsprite->SetTexture(Tex_numberfont);
	numsprite->SetCamera(mainCam);

	numtransform->Scale(Vector2D(256.0f, 256.0f));
	numsprite->SetSize(64.0f, 64.0f);

	numtransform->Translation(Vector2D(1408.0f, -1024.0f));
	numsprite->SetPosition(1408.0f, -1024.0f);

	num->Set(mainCam.width() / 2 - 100.0f, mainCam.height() / 2 - 50.0f, tens);
	FontRender* numfont = number->AddComponent<FontRender>();
	numfont->SetPos(mainCam.width() - 40.0f, mainCam.height() - 70.0f);
	numfont->SetSize(0.5f);
	numfont->SetText(L"0");
	numfont->setMode(1);
	numfont->setType(3);

	///////////////////////
	GameObject* number2 = playerspace->AddObject("number2");
	Sprite* num2sprite = number2->AddComponent<Sprite>();
	Transform* num2transform = number2->AddComponent<Transform>();
	Animation* num2anim = number2->AddComponent<Animation>();
	Number * num2 = number2->AddComponent<Number>();

	num2sprite->Init(Shader_AnimatedTexture);
	num2sprite->setPrior(3);
	num2sprite->setAlpha(0.0f);

	num2anim->Init(2, 8, 1, 1);
	num2anim->setAnimationSpeed(0);
	(*num2anim)[0][0] = 8;
	num2anim->setSequence(0);

	num2sprite->SetTexture(Tex_numberfont);
	num2sprite->SetCamera(mainCam);

	num2transform->Scale(Vector2D(256.0f, 256.0f));
	num2sprite->SetSize(64.0f, 64.0f);

	num2transform->Translation(Vector2D(1408.0f, -1024.0f));
	num2sprite->SetPosition(1408.0f, -1024.0f);

	num2->Set(mainCam.width() / 2 - 70.0f, mainCam.height() / 2 - 50.0f, ones);
	///////////////////////
    //Serial::Serialize(player, "player.json");
		
		Collider * collider = player->AddComponent<Collider>();
		collider->SetHandler(PlayerCollisionHandler);
		collider->SetTransform(player->GetComponent<Transform>());
		collider->SetOffset(Vector2D(0, 0));

		MultiEmitter* MEmitter = player->AddComponent<MultiEmitter>();
		//MEmitter->addEmitter("slime.json");
		
		MEmitter->addEmitter("flame3.json");
		MEmitter->addEmitter("flame2.json");
		MEmitter->addEmitter("flame1.json");
		MEmitter->addEmitter("flame5.json");
		MEmitter->addEmitter("flame4.json");

		/*
		Emitter * emitter = player->AddComponent<Emitter>();
		emitter->setFileName("slime.json");
		emitter->jsonLoad();
		emitter->Init(50, 2);
		*/

		DebugDraw* debplay = player->AddComponent<DebugDraw>();
		debplay->SetMode(0);
		debplay->SetOriginalColor(0.0f, 1.0f, 1.0f);

		LightSource* light = lightManager->AddLight("player", false, 1.f, .05f, -.05f, 360);
		light->Owner(player);
		light->Z(50.f);
		light->Radius(light->BaseRadius());
		light->DoFlicker(true);
		//Serial::Serialize(player, "player.json");
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Fire_Emitter"));
		return player;
	}
}