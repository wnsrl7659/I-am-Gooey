/******************************************************************************/
/*!
\file   DebugWindow.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Displays an imgui window for debug information

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <iostream>
#include <queue>
#include <functional>
#include <filesystem>
#include <boost/locale.hpp>

#include "DebugWindow.h"
#include "../spaces/spaces.h"
#include "../graphics/Sprite.h"
#include "../physics/Physics.h"
#include "../graphics/Shader.h"
#include "../physics/Transform.h"
#include "../component/controller.h"
#include "../graphics/Camera.h"
#include "../settings/settings.h"
#include "../component/emitter.h"
#include "../component/collider.h"
//#include "../puzzles/puzzle.h"
//#include "../data/puzzlemanager.h"
#include "../system/common_defines.h"
#include "../vfx/LightManager.h"
#include "../system/heatmap.h"
#include "../utils/playback.h"
#include "../utils/utils.h"
#include "../utils/autoplay.h"
#include "../component/MultiEmitter.h"
#include "../system/timermanager.h"
#include "../system/countdowntimer.h"
#include "../timer/timer.h"
#include "../audio/audio.h"
#include "../audio/audiomanager.h"
#include "../puzzles/newpuzzle.h"
#include "../vfx/LightReciever.h"
#include "../system/Engine.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_glfw.h"
#include "../IMGUI/imgui_impl_opengl3.h"
#include "../IMGUI/imgui_internal.h"
#include "../system/Window.h"
#include "../system/localize.h"

namespace DebugWindow {
	static std::queue<float> FPSqueue; //!< Queue which stores fps data
	static bool DrawHasBegun = false; //!< boolean of whether gui draw has been called this frame

	/*****************************************************************/
	/*!
		\brief
			ImguiCalls for starting a window
	*/
	/*****************************************************************/
	static void windowDrawBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawHasBegun = true;
	}
	/*****************************************************************/
	/*!
		\brief
			Imgui calls for ending a window (each frame)
	*/
	/*****************************************************************/
	static void windowDrawEnd()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		DrawHasBegun = false;
	}

	static void AudioManagerUI()
	{
		if (ImGui::TreeNode("Audio Manager"))
		{
			if (Audio::aManager)
			{
				auto events = Audio::aManager->GetActiveEvents();

				for (auto e : *events)
				{
					if (ImGui::TreeNode(e.first.c_str()))
					{


						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	}

	static void TimerManagerUI()
	{
		if (ImGui::TreeNode("Timer Manager"))
		{
			auto t = TimerManager::GetTimers();

			for (auto tim : *t)
			{
				CountdownTimer * timer = dynamic_cast<CountdownTimer*>(tim);
				if (!timer) continue;
				if (ImGui::TreeNode("Timer"))
				{
					ImGui::Text("Timer Limit: %fms", timer->GetLimit());
					ImGui::Text("Timer Current: %fms", timer->GetMilliseconds());

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for an animation component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void AnimationComponentUI(BaseComponent* component)
	{
		Animation* anim = dynamic_cast<Animation*>(component);

		float speed = anim->GetAnimationSpeed();
		ImGui::SliderFloat("Animation Speed", &speed, 0, 1.0f);
		anim->setAnimationSpeed(speed);

		ImGui::Text("FrameTimer: %f", anim->GetFrameTimer());

		ImGui::Text("\nAnimationData \n%i x %i", anim->GetAnimationLength(), anim->GetAnimationCount());
		for (int animRow = 0; animRow < anim->GetAnimationCount(); animRow++)
		{
			//One row
			std::string str = "";
			char *cStr = new(char[50]);
			for (int animCol = 0; animCol < anim->GetAnimationLength(); animCol++)
			{
				sprintf_s(cStr, 50, "%3i ", (*anim)[animRow][animCol]);
				str += cStr;
			}
			ImGui::Text(str.c_str());
			delete[] cStr;
		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a transform component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void TransformComponentUI(BaseComponent* component)
	{
		Transform* transform = dynamic_cast<Transform*>(component);

		ImGui::Text("Transform Address: %i", transform);

		Vector2D translation = transform->Translation();
		ImGui::DragFloat("Xpos", &translation[0], 2.0);
		ImGui::DragFloat("Ypos", &translation[1], 2.0);
		transform->Translation(translation);

		float rotation = transform->Rotation();
		ImGui::DragFloat("Angle", &rotation, 3.14f / 30.f);
		transform->Rotation(rotation);

		Vector2D scale = transform->Scale();
		ImGui::DragFloat("X Scale", &scale[0], 5);
		ImGui::DragFloat("Y Scale", &scale[1], 5);
		transform->Scale(scale);
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for the flower Component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	/*static void FlowerComponentUI(BaseComponent* component)
	{
	  Flower * flower = dynamic_cast<Flower*>(component);
	  if (!flower) return;

	  if (flower->Activated())
	  {
		ImGui::Text("Activated: True");
	  }
	  else
	  {
		ImGui::Text("Activated: False");
	  }
	}*/

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a sprite component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void SpriteComponentUI(BaseComponent* component)
	{
		Sprite* spr = dynamic_cast<Sprite*>(component);

		float alpha = spr->getAlpha();
		ImGui::SliderFloat("Alpha", &alpha, 0, 1);
		spr->setAlpha(alpha);


		ImGui::Text("Texture ID: %d", spr->GetTexture());
		ImGui::Text("ShaderID: %s", ShaderGetNameFromInt(spr->GetShader()).c_str());
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a physics component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void PhysicsComponentUI(BaseComponent* component)
	{
		if (component)
		{
			Physics* phys = dynamic_cast<Physics*>(component);
			if (phys)
			{
				float velo = Physics::s_PlayerVelocity;
#ifdef _SUBMISSION
				ImGui::InputFloat("Player Walk Speed", &velo, 2.5, 1, 4);
#else
				ImGui::InputFloat("Velocity", &velo, 2.5, 1, 4);
#endif
				Physics::s_PlayerVelocity = velo;
			}
		}
	}

	/*****************************************************************/
/*!
		\brief
				ImGui display for a light reciever component
		\param component
				the component, which information is being displayed
*/
/*****************************************************************/
	static void LightRecieverComponentUI(BaseComponent* component)
	{
		if (component)
		{
			LightReciever* LR = dynamic_cast<LightReciever*>(component);

			float z = LR->getZ();
			ImGui::DragFloat("Z", &z);
			LR->setZ(z);

			ImGui::Text("Is Wall: %i", LR->getIsWall());
		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a collider component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void ColliderComponentUI(BaseComponent* component)
	{
		Collider * collider = dynamic_cast<Collider *>(component);
		std::stringstream cent, rad;
		cent << "Center: (" << collider->Center()[0] << ", " << collider->Center()[1] << ")";
		ImGui::Text(cent.str().c_str());
		rad << "Radius: " << collider->Radius();
		ImGui::Text(rad.str().c_str());

		Vector2D offset = collider->GetOffset();
		ImGui::DragFloat("OffsetX", &offset[0]);
		ImGui::DragFloat("OffsetY", &offset[1]);
		collider->SetOffset(offset);
		ImGui::Text("%i", collider->handler_);

		if (collider->IsColliding())
		{
			ImGui::Text("Is Colliding: True");
		}
		else
		{
			ImGui::Text("Is Colliding: False");
		}
		switch (collider->GetType())
		{
		case Collider::ColType::Box:
			ImGui::Text("Type: Box");
			break;
		case Collider::ColType::OneWay:
			ImGui::Text("Type: One Way");
			switch (collider->GetDir())
			{
			case Collider::ColDir::Up:
				ImGui::Text("Direction: Up");
				break;
			case Collider::ColDir::Right:
				ImGui::Text("Direction: Right");
				break;
			case Collider::ColDir::Down:
				ImGui::Text("Direction: Down");
				break;
			case Collider::ColDir::Left:
				ImGui::Text("Direction: Left");
				break;
			default:
				ImGui::Text("Direction: No Direction");
				break;
			}
			break;
		default:
			break;
		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a controller component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void ControllerComponentUI(BaseComponent* component)
	{
		Controller* contr = dynamic_cast<Controller*>(component);
		ImGui::Text("This object has a controller!");
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a paralax component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void ParalaxComponentUI(BaseComponent* component)
	{
		Paralax* para = dynamic_cast<Paralax*>(component);
		float speed = para->Speed();
		ImGui::DragFloat("Speed", &speed);
		para->Speed(speed);
		ImGui::Text("Interval: %f", para->Interval());
	}

	/*****************************************************************/
	/*!
		\brief
			Helper function which makes two float sliders
		\param getter
			Getter function for the float pointer
		\param setter
			Setter function which sets the floats after modifying them
		\param emitter
			The emitter to call the functions from
		\param variableName
			The Display name for the Slider
		\param floor
			The lowest value for the slider
		\param ceiling
			The highest value for the slider
	*/
	/*****************************************************************/
	static void floatGetSetSlider(float* (Emitter::*getter)(), void(Emitter::*setter)(float, float), Emitter* emitter, const char* variableName, float floor, float ceiling)
	{
		float* got = (emitter->*getter)(); //this syntax hurts me
		std::string base = variableName;
		base += " base";
		std::string range = variableName;
		range += " range";
		ImGui::SliderFloat(base.c_str(), &got[0], floor, ceiling);
		ImGui::SliderFloat(range.c_str(), &got[1], 0, ceiling * 2); //Range doesnt need negative range
		(emitter->*setter)(got[0], got[1]);
	}

	/*****************************************************************/
	/*!
		\brief
			Helper function which makes a checkbox for an emitter variable
		\param getter
			Getter function for the flag
		\param setter
			Setter function which sets the flag
		\param emitter
			The emitter to call the functions from
		\param variableName
			The Display name for the Slider
	*/
	/*****************************************************************/
	static void boolGetSetCheck(bool (Emitter::*getter)(), void(Emitter::*setter)(bool), Emitter* emitter, const char* variableName)
	{
		bool is = (emitter->*getter)();
		ImGui::Checkbox(variableName, &is);
		(emitter->*setter)(is);
	}

	/*****************************************************************/
	/*!
		\brief
			Helper function which makes a GUI branch for a Wave
			variable
		\param name
			The name of the wave variable
		\param wave
			The wave variable to derive data from
	*/
	/*****************************************************************/
	static void TreeWave(const char * name, Wave& wave)
	{
		if (ImGui::TreeNode(name))
		{
			float values[30];
			for (int i = 0; i < 30; i++)
				values[i] = wave.returnScalar(.1f*i);
			ImGui::PlotLines("Graph", values, 30);
			float value;
			value = wave.getHeight();
			ImGui::SliderFloat("WaveHeight", &value, 0, 2);
			wave.setHeight(value);

			value = wave.getCenter();
			ImGui::SliderFloat("WaveCenter", &value, -2, 2);
			wave.setCenter(value);

			value = wave.getWidth();
			ImGui::SliderFloat("WaveWidth", &value, 0, 5);
			wave.setWidth(value);

			ImGui::TreePop();
		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a emitter component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void EmitterComponentUI(BaseComponent* component)
	{
		Emitter* emitter = dynamic_cast<Emitter*>(component);
		ImGui::Text(emitter->getFileName().c_str());
		//REINITIALIZE BUTTON
		if (ImGui::Button("REINITIALIZE"))
			emitter->Init(emitter->getParticleCount(), emitter->getLifetime());

		if (ImGui::TreeNode("Save/Load"))
		{
			const char* name = emitter->getFileName().c_str();
			ImGui::Text("Saves/Loads from /assets/emitters/%s", emitter->getFileName().c_str());
			static char buffer[30];
			strcpy_s(buffer, emitter->getFileName().c_str());
			//strcpy_s(buffer, emitter->getFileName().c_str());
			ImGui::InputText("FileName", buffer, 30);

			std::string newName = buffer;
			emitter->setFileName(newName);


			//SAVE BUTTON
			if (ImGui::Button("SAVE JSON"))
				emitter->jsonSave();
			//LOAD BUTTON
			if (ImGui::Button("LOAD JSON"))
				emitter->jsonLoad();
			ImGui::TreePop();
		}
		//MAIN EMITTER VARIABLES
		int count = emitter->getParticleCount();
		float life = emitter->getLifetime();
		int changeChecker1 = count;
		float changeChecker2 = life;

		if (ImGui::TreeNode("Main Emitter Variables"))
		{
			ImGui::InputInt("ParticleCount", &count, 50);
			ImGui::InputFloat("ParticleLifetime", &life, .25);
			if (count != changeChecker1 || life != changeChecker2)
			{
				if (count <= 0)
					count = 1;
				//if (life <= .1f)
				//  life = .1f;
				emitter->Init(count, life); //Reinitialize emitter with new variables
			}
			ImGui::TreePop();
		}



		//EMITTER FLAGS
		if (ImGui::TreeNode("Emitter Flags"))
		{
			/**/
			boolGetSetCheck(&Emitter::getIsAnchored, &Emitter::setIsAnchored, emitter, "Is Anchored");
			boolGetSetCheck(&Emitter::getIsRadial, &Emitter::setIsRadial, emitter, "Is Radial");
			boolGetSetCheck(&Emitter::getIsBurst, &Emitter::setIsBurst, emitter, "Is Burst");
			boolGetSetCheck(&Emitter::getDoLoop, &Emitter::setDoLoop, emitter, "Loop Particles");
			boolGetSetCheck(&Emitter::getIsColor, &Emitter::setIsColor, emitter, "Is Color Mode");
			/**/

			ImGui::TreePop();
		}
		//EMITTER PHYSICS VARIABLES
		if (ImGui::TreeNode("Emitter Variables"))
		{
			float scalar = emitter->getScalar();
			ImGui::SliderFloat("Scalar", &scalar, 0, 5);
			emitter->setScalar(scalar);

			if (ImGui::TreeNode("Position"))
			{
				float offX = emitter->getPosOffset()[0];
				float offY = emitter->getPosOffset()[1];
				ImGui::DragFloat("OffsetX", &offX);
				ImGui::DragFloat("OffsetY", &offY);
				emitter->setPosOffset(offX, offY);

				floatGetSetSlider(&Emitter::getxPos, &Emitter::setxPos, emitter, "InitialX", -1000, 1000);
				floatGetSetSlider(&Emitter::getyPos, &Emitter::setyPos, emitter, "InitialY", -1000, 1000);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Direction"))
			{
				floatGetSetSlider(&Emitter::getAngle, &Emitter::setAngle, emitter, "Angle", 0.f, 6.28f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Movement"))
			{
				floatGetSetSlider(&Emitter::getForwardVel, &Emitter::setForwardVel, emitter, "ForwardVelocity", -10, 10);
				floatGetSetSlider(&Emitter::getForwardAcc, &Emitter::setForwardAcc, emitter, "ForwardAcceleration", -10, 10);
				floatGetSetSlider(&Emitter::getSideVel, &Emitter::setSideVel, emitter, "SideVelocity", -10, 10);
				floatGetSetSlider(&Emitter::getSideAcc, &Emitter::setSideAcc, emitter, "SideAcceleration", -10, 10);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Image"))
			{
				int imageIndex = emitter->getAnimIndex();
				ImGui::SliderInt("ImageIndex", &imageIndex, 0, 15);
				emitter->setAnimIndex(imageIndex);
				floatGetSetSlider(&Emitter::getAlpha, &Emitter::setAlpha, emitter, "Alpha", 0.f, 1.0f);
				floatGetSetSlider(&Emitter::getAlphaSpeed, &Emitter::setAlphaSpeed, emitter, "AlphaChange", -.2f, .2f);
				floatGetSetSlider(&Emitter::getRotationalSpeed, &Emitter::setRotationalSpeed, emitter, "RotationChange", -.1, .1);
				floatGetSetSlider(&Emitter::getSizeX, &Emitter::setSizeX, emitter, "X Scale", -100, 100);
				floatGetSetSlider(&Emitter::getSizeY, &Emitter::setSizeY, emitter, "Y Scale", -100, 100);
				if (emitter->getIsColor())
				{
					floatGetSetSlider(&Emitter::getColorR, &Emitter::setColorR, emitter, "Red", 0.f, 1.0f);
					floatGetSetSlider(&Emitter::getColorG, &Emitter::setColorG, emitter, "Green", 0.f, 1.0f);
					floatGetSetSlider(&Emitter::getColorB, &Emitter::setColorB, emitter, "Blue", 0.f, 1.0f);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Waves"))
			{
				Wave wav = emitter->getWaveSide();
				TreeWave("PositionSideWave", wav);
				emitter->setWaveSide(wav);

				wav = emitter->getWaveForward();
				TreeWave("PositionForwardWave", wav);
				emitter->setWaveForward(wav);

				wav = emitter->getWaveSize();
				TreeWave("SizeWave", wav);
				emitter->setWaveSize(wav);

				wav = emitter->getWaveAlpha();
				TreeWave("TransparencyWave", wav);
				emitter->setWaveAlpha(wav);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		//LIST OF PARTICLES
		if (ImGui::TreeNode("ParticleList"))
		{
			Particle* particle = emitter->getParticles();
			for (int i = 0; i < emitter->getParticleCount(); i++)
			{
				GameObject* obj = particle[i].getObject();
				char string[20];
				sprintf_s(string, 20, "Particle %i", i);
				if (ImGui::TreeNode(string))
				{
					ImGui::Text("LifeTime: %f", particle[i].getTime());

					//Loop through the particles components
					for (auto itComponent : obj->components)
					{
						BaseComponent* component2 = itComponent.second;
						if (ImGui::TreeNode(component2->name.c_str()))
						{
							if (component2->name.find("Sprite", 0) != std::string::npos)
								SpriteComponentUI(component2);
							else if (component2->name.find("Transform", 0) != std::string::npos)
								TransformComponentUI(component2);

							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		//RESET BUTTON
		if (ImGui::Button("RESET VALUES"))
		{
			emitter->ResetToDefault();
			emitter->Init(emitter->getParticleCount(), emitter->getLifetime());
		}

	}

	/*****************************************************************/
	/*!
			\brief
					ImGui display for a Multiemitter component
			\param component
					the component, which information is being displayed
	*/
	/*****************************************************************/
	static void MultiEmitterComponentUI(BaseComponent* component)
	{
		MultiEmitter* Memitter = dynamic_cast<MultiEmitter*>(component);
		auto emitters = Memitter->getEmitters();

		ImGui::Text("Owner: %i", Memitter->owner);

		if (ImGui::Button("REINITIALIZE"))
		{
			for (auto emitter : *emitters)
				emitter->Init(emitter->getParticleCount(), emitter->getLifetime());
		}

		//Add new emitter section
		ImGui::Text("~ADD NEW EMITTER~");
		static char MultiEmitterBuffer[30] = "NewEmitter.json";
		ImGui::InputText("FileName", MultiEmitterBuffer, 30);

		std::string fileName = MultiEmitterBuffer;
		if (ImGui::Button("CreateEmitter"))
		{
			Memitter->addEmitter(fileName.c_str());
		}

		//Emitter loop
		int i = 0;
		for (auto it : *emitters)
		{
			i++;
			std::stringstream name;
			name << "Emitter[" << i << "]";
			if (ImGui::TreeNode(name.str().c_str()))
			{
				EmitterComponentUI(it);

				if (ImGui::TreeNode("REMOVE EMITTER!"))
				{
					if (ImGui::Button("RIP Emitter?"))
						Memitter->removeEmitter(it);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

		}
	}

	/*****************************************************************/
	/*!
		\brief
			ImGui display for a paralax component
		\param component
			the component, which information is being displayed
	*/
	/*****************************************************************/
	static void TeleporterComponentUI(BaseComponent* component)
	{
		Teleporter* para = dynamic_cast<Teleporter*>(component);
		ImGui::Text("This object has a teleporter!");
	}

	/*****************************************************************/
	/*!
		\brief
			Tab for camera information
	*/
	/*****************************************************************/
	static void CameraTree()
	{
		if (ImGui::TreeNode("CAMERA"))
		{
			float x = mainCam.center().x;
			ImGui::DragFloat("Xpos", &x, 2.0);
			float y = mainCam.center().y;
			ImGui::DragFloat("Ypos", &y, 2.0);
			mainCam.SetPosition(x, y);

			static float camAngle = 0;
			ImGui::DragFloat("Angle", &camAngle, 3.14f / 60);
			mainCam.SetRotation(camAngle);

			static float camZoom = 1;
			ImGui::DragFloat("Zoom", &camZoom, 0.01f);
			mainCam.SetZoom(camZoom);

			ImGui::TreePop();
		}
	}

	static void localize_tree()
	{
		if (ImGui::TreeNode("Localization"))
		{
			auto& loc = System::lochandler->GetLocale();
			auto& lang = System::lochandler->langMap.at(loc);

			std::string lang_mbs = boost::locale::conv::utf_to_utf<char>(lang);

			ImGui::Text("Locale: %s", loc.c_str());
			ImGui::Text("Language: %s", lang_mbs.c_str());

			ImGui::TreePop();
		}
	}

	/*****************************************************************/
	/*!
		\brief
			Tab for settings information
	*/
	/*****************************************************************/
	static void SettingsTree()
	{
#ifndef _SUBMISSION
		if (ImGui::TreeNode("SETTINGS"))
		{
			bool doShadows = lightManager->getDoShadows();
			ImGui::Checkbox("Cast Shadows", &doShadows);
			lightManager->setDoShadows(doShadows);

			ImGui::Checkbox("Collisions", &Settings::collisions);
			ImGui::Checkbox("Debug", &Settings::debug);
			ImGui::Checkbox("Live Update", &Settings::liveUpdate);
			ImGui::Checkbox("Master Mute", &Settings::masterMuted);
			ImGui::Checkbox("Music Muted", &Settings::musicMuted);
			ImGui::Checkbox("sfx Muted", &Settings::sfxMuted);
			ImGui::Checkbox("windowed", &Settings::windowed);
			ImGui::Checkbox("colorStandard", &Settings::colorStandard);
			ImGui::Checkbox("colorBG", &Settings::colorBG);
			ImGui::Checkbox("colorRY", &Settings::colorRY);
			ImGui::Checkbox("colorRG", &Settings::colorRG);
			ImGui::Checkbox("colorMono", &Settings::colorMono);
			ImGui::Checkbox("culling", &Settings::culling);
			ImGui::Checkbox("camfollowing", &Settings::camfollowing);

			ImGui::SliderInt("masterVol", &Settings::masterVol, 0, 100);
			ImGui::SliderInt("musicVol", &Settings::musicVol, 0, 100);
			ImGui::SliderInt("sfxVol", &Settings::sfxVol, 0, 100);

			ImGui::TreePop();
		}
#else
		ImGui::Checkbox("Master Volume Mute", &Settings::masterMuted);
		ImGui::Checkbox("Music Volume Mute", &Settings::musicMuted);
		ImGui::Checkbox("SFX Volume Mute", &Settings::sfxMuted);

		ImGui::SliderInt("masterVol", &Settings::masterVol, 0, 100);
		ImGui::SliderInt("musicVol", &Settings::musicVol, 0, 100);
		ImGui::SliderInt("sfxVol", &Settings::sfxVol, 0, 100);

#endif // !_SUBMISSION
	}

	/*****************************************************************/
	/*!
		\brief
			Gui tab for game data information
	*/
	/*****************************************************************/
	//static void DataManagerTree()
	//{
	//  if (ImGui::TreeNode("PUZZLEMANAGER"))
	//  {
	//    ImGui::Text("Total Puzzles: %i", Data::pManager->PuzzleCount());
	//    ImGui::Text("Completed Puzzles: %i", Data::pManager->CompletedPuzzleCount());

	//    ImGui::InputInt("Hints", reinterpret_cast<int*>(Data::pManager->Hint()), 1);

	//    //auto cols = Data::pManager->GetColliderVector();
	//    auto puzz = Data::pManager->GetPuzzleVector();

	//    if (ImGui::TreeNode("Puzzles"))
	//    {
	//      for (auto c : *puzz)
	//      {
	//        if (ImGui::TreeNode(c->name_.c_str()))
	//        {
	//          ImGui::Text("Nodes completed: %u/%u", c->CompletedPoints(), c->Points());
	//          auto poi = c->GetPoints();
	//          for (auto p : *poi)
	//          {
	//            if (ImGui::TreeNode(p->name.c_str()))
	//            {
	//              Flower * f = dynamic_cast<Flower *>(p->GetComponent<Flower>());
	//              if (f)
	//              {
	//                FlowerComponentUI(f);
	//              }
	//              ImGui::TreePop();
	//            }
	//          }

	//          ImGui::TreePop();
	//        }
	//      }

	//      ImGui::TreePop();
	//    }

	//    /*if (ImGui::TreeNode("Colliders"))
	//    {
	//      ImGui::Text("Count: %i", cols->size());
	//      for (auto c : *cols)
	//      {
	//        if (ImGui::TreeNode(c->name))
	//        {
	//          ColliderComponentUI(c);

	//          ImGui::TreePop();
	//        }
	//      }
	//      ImGui::TreePop();
	//    }*/

	//    ImGui::TreePop();
	//  }
	//}

	  /*****************************************************************/
	  /*!
			  \brief
					  Tab for settings information
	  */
	  /*****************************************************************/
	static void NewPuzzleTree()
	{
#ifndef _SUBMISSION
		if (ImGui::TreeNode("PUZZLES"))
		{
			//PUZZLE MANAGER//
			ImGui::Text("Coins: %d", PzManage::pzm->hints);
			if (ImGui::Button("Gib hint"))
			{
				++PzManage::pzm->hints;
			}
			if (ImGui::Button("Taketh Away"))
			{
				--PzManage::pzm->hints;
			}

			ImGui::Text("Active Node: %d", &*PzManage::pzm->GetActiveNode());
			ImGui::Text("Active Node: %d", PzManage::pzm->GetNodeIsActive());

			//PUZZLE MANAGER//

			std::vector<Puzzle*> *puzzles = PzManage::pzm->GetPuzzles();
			for (Puzzle* puzzle : *puzzles)
			{
				std::stringstream puzzlename;
				puzzlename << "Puzzle[" << puzzle->puzzleNumber << "]";
				if (ImGui::TreeNode(puzzlename.str().c_str()))
				{
					//PUZZLE//

					ImGui::Text("Puzzle Complete: %d", puzzle->isCompleted);

					//PUZZLE//
					std::vector<PuzzleNode*> *nodes = puzzle->GetNodes();
					for (PuzzleNode* node : *nodes)
					{
						std::stringstream nodename;
						nodename << "Node[" << &*node << "]" << " " << node->isActive;
						if (ImGui::TreeNode(nodename.str().c_str()))
						{
							//NODE//

							ImGui::Text("isStart: %d", node->isStart);
							ImGui::Text("isCol: %d", node->isCol);
							if (ImGui::TreeNode("Animation"))
							{
								AnimationComponentUI(node->owner->GetComponent<Animation>());
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("MultiEmitter"))
							{
								MultiEmitterComponentUI(node->owner->GetComponent<MultiEmitter>());
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Sprite"))
							{
								SpriteComponentUI(node->owner->GetComponent<Sprite>());
								ImGui::TreePop();
							}

							//NODE//
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
#endif // !_SUBMISSION
	}

	/*****************************************************************/
/*!
		\brief
				Gui tab for light information
*/
/*****************************************************************/
	static void LightManagerTree()
	{
		if (ImGui::TreeNode("LIGHTMANAGER"))
		{
			bool doShadows = lightManager->getDoShadows();
			ImGui::Checkbox("Cast Shadows", &doShadows);
			lightManager->setDoShadows(doShadows);
			//LIGHTS
			if (ImGui::TreeNode("LIGHTS"))
			{
				auto lights = lightManager->getLights();
				auto hiddenlights = lightManager->getHiddenLights();
				int i = 0;
				static bool LMHeatVisible = false;
				if (ImGui::Checkbox("MakeHeatVisible", &LMHeatVisible))
				{
					if (LMHeatVisible)
						lightManager->MakeLightVisible("Heat");
					else
						lightManager->MakeLightHidden("Heat");
				}
				ImGui::Text("HiddenLights: %i", hiddenlights.size());
				for (auto it : lights)
				{
					i++;
					std::stringstream name;
					name << "Light: ";
					name << i;
					name << " " << it->label;
					if (ImGui::TreeNode(name.str().c_str()))
					{
						float red = it->R();
						ImGui::SliderFloat("Color Red", &red, -1, 1);
						it->R(red);

						float blue = it->B();
						ImGui::SliderFloat("Color Blue", &blue, -1, 1);
						it->B(blue);

						float green = it->G();
						ImGui::SliderFloat("Color Green", &green, -1, 1);
						it->G(green);

						float radius = it->BaseRadius();
						ImGui::DragFloat("Radius", &radius);
						it->BaseRadius(radius);

						float x = it->X();
						ImGui::DragFloat("X", &x);
						it->X(x);

						float y = it->Y();
						ImGui::DragFloat("Y", &y);
						it->Y(y);

						float z = it->Z();
						ImGui::DragFloat("Z", &z);
						it->Z(z);

						bool flicker = it->DoFlicker();
						ImGui::Checkbox("Flicker", &flicker);
						it->DoFlicker(flicker);

						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			//END LIGHTS

			//WALLS
			if (ImGui::TreeNode("WALLS"))
			{

				std::vector<glm::vec4> walls = lightManager->getWalls();
				std::vector<glm::vec2> norms = lightManager->getWallNormals();

				if (walls.size() > norms.size())
					ImGui::Text("ERROR: More walls than normals!?");
				else if (norms.size() > walls.size())
					ImGui::Text("ERROR: More normals than walls!?");
				else//They are the same size, loop through the list
				{
					int size = walls.size();
					for (int i = 0; i < size; i++)
					{
						std::stringstream treeName;
						treeName << "Wall[" << i << "]";
						if (ImGui::TreeNode(treeName.str().c_str()))
						{
							ImGui::Text("Wall: (%.1f, %.1f) (%.1f, %.1f)", walls[i].x, walls[i].y, walls[i].z, walls[i].w);
							ImGui::Text("Norm: (%.1f, %.1f)", norms[i].x, norms[i].y);

							ImGui::TreePop();
						}
					}
				}

				ImGui::TreePop();
			}
			//END WALLS
			ImGui::TreePop();
		}
	}


	/*****************************************************************/
	/*!
		\brief
			Modifiable values for an entire space
		\param space
			the space to modify
	*/
	/*****************************************************************/
	static void ModifySpaceAll(Space* space)
	{
		float allAlpha = 0.0;
		float allX = 0.0;
		float allY = 0.0;
		auto ObjMap = space->GetObjectMap();

		//Sprite Options
		if (ImGui::TreeNode("All Sprites"))
		{
			ImGui::DragFloat("Add to alpha for All", &allAlpha, 0.01f, -.05f, .05f, "click and drag to change alpha");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("All Transform"))
		{
			ImGui::DragFloat("Add to X for All", &allX, 5.0f, -5.0f, 5.0f, "click and drag to change X");
			ImGui::DragFloat("Add to Y for All", &allY, 5.0f, -5.0f, 5.0f, "click and drag to change Y");
			ImGui::TreePop();
		}

		//Loop through all objects and set the variable
		for (auto itObj : *ObjMap)
		{
			GameObject* object = itObj.second;
			Sprite* spr = (object->GetComponent<Sprite>());
			if (spr)
			{
				spr->setAlpha(spr->getAlpha() + allAlpha);
			}
			Transform* trs = (object->GetComponent<Transform>());
			if (trs)
			{
				Vector2D pos = trs->Translation();
				pos[0] += allX;
				pos[1] += allY;
				trs->Translation(pos);
			}
		}
	}

	static void PlaybackTree()
	{
		if (ImGui::TreeNode("Playback Manager"))
		{
			if (ImGui::Button("Start Playback"))
			{
				Playback::Start();
			}
			if (ImGui::Button("Stop Playback"))
			{
				Playback::Stop();
			}
			ImGui::Text("Current file: %s", Playback::ply->CurrentFile().c_str());
			std::stringstream ss;
			ss << Utils::GetGameDataFolder() << "\\PlaybackInfo";
			if (!std::filesystem::exists(ss.str()))
			{
				std::filesystem::create_directories(ss.str());
			}
			auto s = Utils::GetFolderContents(ss.str());
			auto it = s.begin();
			while (it != s.end())
			{
				if (ImGui::Button(it->c_str()))
				{
					std::stringstream sss;
					sss << Utils::GetGameDataFolder();
					sss << "\\PlaybackInfo\\" << *it << "\\playback.dat";
					Playback::ply->LoadPlaybackFile(sss.str());
				}
				++it;
			}

			ImGui::TreePop();
		}
	}

	/*****************************************************************/
	/*!
		\brief
			Using the helper functions, makes the GUI window to
			display all spaces, and all game object in those spaces,
			and then all components in each game object. Additionally
			has the CAMERA and SETTINGS tabs.
		\param spaces
			The map of currently used spaces.
	*/
	/*****************************************************************/
	void drawSpaceWindow(std::unordered_map<std::string, Space *>& spaces, SpaceManager& spaceManager)
	{
#ifdef _SUBMISSION
		Space * psp = spaceManager.GetSimilarSpace("playerspace");
		Physics * pp = NULL;
		if (psp)
		{
			GameObject * p = psp->GetObject("player");
			if (p)
			{
				pp = p->GetComponent<Physics>();
			}
		}
		if (!DrawHasBegun)
			windowDrawBegin();

		ImGui::SetNextWindowPos(ImVec2(20, 150), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
#ifdef _DEBUG
		ImGui::Begin("Cheat Viewer: Debug");
#elif defined _RELEASE
		ImGui::Begin("Cheat Viewer: Release");
#elif defined _SUBMISSION
		ImGui::Begin("Cheat Viewer: Submission");
#else
		ImGui::Begin("You shouldn't see this");
#endif
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Debug Draw", &Settings::debugLines);
		SettingsTree();
		if (ImGui::Button("Start Autoplay"))
		{
			Utils::atst->Start();
		}
		if (ImGui::Button("Stop Autoplay"))
		{
			Utils::atst->Stop();
		}
		if (ImGui::Button("Start Input Capture"))
		{
			InputCapture::icatcher->Start();
		}
		if (ImGui::Button("End InputCapture"))
		{
			InputCapture::icatcher->Stop();
		}
		PlaybackTree();
		if (pp)
			PhysicsComponentUI(pp);
#else


		if (!DrawHasBegun)
			windowDrawBegin();

		ImGui::SetNextWindowPos(ImVec2(20, 150), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

#ifdef _DEBUG
		ImGui::Begin("Property Viewer: Debug");
#else
		ImGui::Begin("Property Viewer: Release");
#endif // DEBUG


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Engine dt: %f", pEngine->dt_);
		//ResetButton
		if (ImGui::Button("RestartLevel"))
			spaceManager.ResetSpace();
		if (ImGui::Button("Win"))
		{
			//Data::pManager->Win();
		}
		if (ImGui::Button("Create Crash Dump"))
		{
			Trace::CreateCrashDump();
		}
		if (ImGui::Button("Start Input Capture"))
		{
			InputCapture::icatcher->Start();
		}
		if (ImGui::Button("End InputCapture"))
		{
			InputCapture::icatcher->Stop();
		}
		if (ImGui::Button("Start Autoplay"))
		{
			Utils::atst->Start();
		}
		if (ImGui::Button("Stop Autoplay"))
		{
			Utils::atst->Stop();
		}

		localize_tree();

		PlaybackTree();

		CameraTree();

		SettingsTree();

		LightManagerTree();

		NewPuzzleTree();

		TimerManagerUI();

		AudioManagerUI();

		//DataManagerTree();

				ImGui::Text("c_level: %d", Spaces::spaceManager->clevel());
		for (auto itSpace : spaces)
		{
			Space* space = itSpace.second;
			//BRANCH FOR EACH SPACE
			if (ImGui::TreeNode(space->name.c_str()))
			{
				auto ObjMap = *(space->GetObjectMap()); //Get all the objects

				std::string allObj = "All Objs of ";
				allObj += space->name;
				if (ImGui::TreeNode(allObj.c_str()))
				{
					ModifySpaceAll(space);

					ImGui::TreePop();
				}

				for (auto itObj : ObjMap)
				{
					GameObject* object = itObj.second;

					//If the object ends up being a tile
					if (object->name.find("tile", 0) != std::string::npos)
					{

					}

					//BRANCH FOR EACH OBJECT
					if (ImGui::TreeNode(object->name.c_str()))
					{
						for (auto itComponent : object->components)
						{
							//BRANCH FOR EACH COMPONENT
							BaseComponent* component = itComponent.second;
							if (ImGui::TreeNode(component->name.c_str()))
							{
								/*ADD COMPONENTS DEPENDING ON THE COMPONENT*/
								if (component->name.find("Animation", 0) != std::string::npos)
									AnimationComponentUI(component);
								else if (component->name.find("Sprite", 0) != std::string::npos)
									SpriteComponentUI(component);
								else if (component->name.find("Physics", 0) != std::string::npos)
									PhysicsComponentUI(component);
								else if (component->name.find("Transform", 0) != std::string::npos)
									TransformComponentUI(component);
								else if (component->name.find("Collider", 0) != std::string::npos)
									ColliderComponentUI(component);
								else if (component->name.find("Controller", 0) != std::string::npos)
									ControllerComponentUI(component);
								else if (component->name.find("Paralax", 0) != std::string::npos)
									ParalaxComponentUI(component);
								else if (component->name.find("Teleporter", 0) != std::string::npos)
									TeleporterComponentUI(component);
								else if (component->name.find("MultiEmitter", 0) != std::string::npos)
									MultiEmitterComponentUI(component);
								else if (component->name.find("Emitter", 0) != std::string::npos)
									EmitterComponentUI(component);
								else if (component->name.find("LightReciever", 0) != std::string::npos)
									LightRecieverComponentUI(component);
								//else if (component->name.find("Flower", 0) != std::string::npos)
								  //FlowerComponentUI(component);
								/**/
								ImGui::TreePop();
							}
						}
						if (ImGui::TreeNode("Add Component"))
						{
							if (!object->GetComponent<MultiEmitter>())
							{
								if (ImGui::Button("Emitter"))
								{
									// add emitter to object on next frame

									object->AddComponentNextFrame<MultiEmitter>();
								}
							}

							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
#endif // _SUBMISSION
		ImGui::End();
		//windowDrawEnd();

	}

	/*****************************************************************/
	/*!
		\brief
			Creates a Gui window with FPS information, and a graph
			containing time elapsed per frame, for the last 120
			frames
	*/
	/*****************************************************************/
	void drawFPSWindow()
	{
		if (!DrawHasBegun)
			windowDrawBegin();
		ImGui::Begin("FPS Viewer");
		/**/
		float fps = ImGui::GetIO().Framerate;

		FPSqueue.push(fps);

		int extraSpace = FPSqueue.size() - 120;
		for (int i = 0; i < extraSpace; i++)
			FPSqueue.pop();

		std::queue<float> duplicate = FPSqueue;

		float values[121];
		values[0] = 0;
		int queueSize = duplicate.size();
		for (int i = 0; i < queueSize; i++)
		{
			values[i + 1] = duplicate.front();
			duplicate.pop();
		}

		ImGui::PlotLines("Graph", values, queueSize, 1);
		//float value;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		/**/
		ImGui::End();
		//windowDrawEnd();
	}

	/*****************************************************************/
	/*!
		\brief
			Ends the GUI draw, can only be called once per frame
	*/
	/*****************************************************************/
	void drawWindows()
	{
		windowDrawEnd();
	}
}