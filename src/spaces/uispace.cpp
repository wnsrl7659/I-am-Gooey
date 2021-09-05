// copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.

#include "spacemanager.h"
#include "../graphics/Sprite.h"
#include "../graphics/Camera.h"
#include "../graphics/Shader.h"

namespace Spaces
{
	void BuildUIspace()
	{
		Space * ui = spaceManager->AddSpace("10000 ui");
		GameObject * go = ui->AddObject("ui");
		Sprite * sprite = go->AddComponent<Sprite>();
		Transform* transform = go->AddComponent<Transform>();
		Animation* anim = go->AddComponent<Animation>();
		sprite->Init(Shader_AnimatedTexture);

		anim->Init(1, 1, 1, 1);

		anim->setAnimationSpeed(0);

		for (int z = 0; z < 1; z++)
		{
			(*anim)[z][0] = z;
		}

		anim->setSequence(0);

		sprite->setAlpha(1.0f);
		sprite->setData(1000);
		
		sprite->SetTexture(Tex_Box);

		sprite->SetCamera(mainCam);

		transform->Scale(Vector2D(1664.0f, 494.0f));
		sprite->SetSize(1664.0f, 494.0f);

		//transform->Translation(Vector2D(1664.0f,-1024.0f));
		transform->Translation(Vector2D(mainCam.center().x, mainCam.center().y));
		sprite->SetPosition(mainCam.center().x, mainCam.center().y);

		/***********************************************************/
		go = ui->AddObject("uij");
		sprite = go->AddComponent<Sprite>();
		transform = go->AddComponent<Transform>();
		anim = go->AddComponent<Animation>();
		sprite->Init(Shader_AnimatedTexture);

		anim->Init(1, 1, 1, 1);

		anim->setAnimationSpeed(0);

		for (int z = 0; z < 1; z++)
		{
			(*anim)[z][0] = z;
		}

		anim->setSequence(0);

		sprite->setAlpha(1.0f);
		sprite->setData(1000);

		sprite->SetTexture(Tex_Box);

		sprite->SetCamera(mainCam);

		transform->Scale(Vector2D(256.0f, 256.0f));
		sprite->SetSize(256.0f, 256.0f);

		transform->Translation(Vector2D(mainCam.center().x - 400.0f, mainCam.center().y));
		sprite->SetPosition(mainCam.center().x - 400.0f, mainCam.center().y);

		/***********************************************************/
		go = ui->AddObject("uik");
		sprite = go->AddComponent<Sprite>();
		transform = go->AddComponent<Transform>();
		anim = go->AddComponent<Animation>();
		sprite->Init(Shader_AnimatedTexture);

		anim->Init(1, 1, 1, 1);

		anim->setAnimationSpeed(0);

		for (int z = 0; z < 1; z++)
		{
			(*anim)[z][0] = z;
		}

		anim->setSequence(0);

		sprite->setAlpha(1.0f);
		sprite->setData(1000);

		sprite->SetTexture(Tex_Box);

		sprite->SetCamera(mainCam);

		transform->Scale(Vector2D(256.0f, 256.0f));
		sprite->SetSize(256.0f, 256.0f);

		transform->Translation(Vector2D(mainCam.center().x - 100.0f, mainCam.center().y));
		sprite->SetPosition(mainCam.center().x - 100.0f, mainCam.center().y);

		/***********************************************************/
		go = ui->AddObject("uil");
		sprite = go->AddComponent<Sprite>();
		transform = go->AddComponent<Transform>();
		anim = go->AddComponent<Animation>();
		sprite->Init(Shader_AnimatedTexture);

		anim->Init(1, 1, 1, 1);

		anim->setAnimationSpeed(0);

		for (int z = 0; z < 1; z++)
		{
			(*anim)[z][0] = z;
		}

		anim->setSequence(0);

		sprite->setAlpha(1.0f);
		sprite->setData(1000);

		sprite->SetTexture(Tex_Box);

		sprite->SetCamera(mainCam);

		transform->Scale(Vector2D(256.0f, 256.0f));
		sprite->SetSize(256.0f, 256.0f);

		transform->Translation(Vector2D(mainCam.center().x + 200.0f, mainCam.center().y));
		sprite->SetPosition(mainCam.center().x + 200.0f, mainCam.center().y);

		/***********************************************************/
		go = ui->AddObject("ui;");
		sprite = go->AddComponent<Sprite>();
		transform = go->AddComponent<Transform>();
		anim = go->AddComponent<Animation>();
		sprite->Init(Shader_AnimatedTexture);

		anim->Init(1, 1, 1, 1);

		anim->setAnimationSpeed(0);

		for (int z = 0; z < 1; z++)
		{
			(*anim)[z][0] = z;
		}

		anim->setSequence(0);

		sprite->setAlpha(1.0f);
		sprite->setData(1000);

		sprite->SetTexture(Tex_Box);

		sprite->SetCamera(mainCam);

		transform->Scale(Vector2D(256.0f, 256.0f));
		sprite->SetSize(256.0f, 256.0f);

		transform->Translation(Vector2D(mainCam.center().x + 500.0f, mainCam.center().y));
		sprite->SetPosition(mainCam.center().x + 500.0f, mainCam.center().y);

		/***********************************************************/
	}
}
