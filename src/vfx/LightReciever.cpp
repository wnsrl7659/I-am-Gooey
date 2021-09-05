/******************************************************************************/
/*!
\file		LightReciever.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LightManager.h"
#include "LightReciever.h"
#include "../graphics/Shader.h"
#include "../graphics/Sprite.h"
#include "../gameobject/gameobject.h"
#include <glm/common.hpp>
#include "../graphics/TextureManager.h"
#include "../system/Utility.h"//float rand
#include "../settings/settings.h"
#include "../component/MultiEmitter.h"
#include "../system/Engine.h"

LightReciever::LightReciever()
{
		Z = 0;
		isWall = 0;
		SetColor1(1.0, 0.0, 1.0);//Pink default so it's obvious
		doLava = 0;
}

void LightReciever::setUniforms()
{
		auto lights = lightManager->getLights();

		Sprite* Spr = owner->GetComponent<Sprite>();//for shader information
		if (Spr)
		{
				int shaderIndex = Spr->getShaderIndex();
				if (shaderIndex >= 0 && shaderIndex < Shader_Last)//if valid shader
				{
						std::vector<float>LightData;
						std::vector<float>LightColor;

						LightData.reserve(sizeof(float) * 500);
						LightColor.reserve(sizeof(float) * 500);

						GLint loca; //Uniform Locations
						int validLights = 0;//How many valid lights did the reciever find

						//get light information
						for (auto it : lights)
						{
								//Quick distance check
								/**/
								//Dont apply this optimization to the camera when not culling (screenshot)
								if (!(!Settings::culling && it->label == "camera"))
								{
										/*
										//From vertex shader
										if(isWall != 0) //WALL
												v_World = vec3(v.x, TileY - TileZ, TileZ + (v.y - TileY));
										else //FLOOR
												v_World = vec3(v.x, v.y + TileZ, TileZ);
										*/
										glm::vec2 pos = Spr->GetPosition();
										float tx = pos.x, ty, tz = Z * 2;
										if (isWall)
												ty = pos.y - Z * 2;
										else
												ty = pos.y + Z * 2;

										float dx = abs(tx - it->X());
										float dy = abs(ty - it->Y());
										float dz = abs(tz - it->Z() * 2);
										dx *= dx;
										dz *= dz;
										dy *= dy * 2;
										if (dx + dy + dz > it->Radius() * it->Radius())
												continue;
								}
								/**/
								//if (abs(Spr->GetPosition().y - it->Y()) < it->Radius())
								//		continue;

								//For screenshot
								if (!Settings::culling && it->label == "camera")
								{
										validLights++;
										LightData.push_back(1750.f);
										LightData.push_back(-1000.f);
										LightData.push_back(1000.f);//Z
										LightData.push_back(20000.f);//R

										LightColor.push_back(0.2f);
										LightColor.push_back(0.2f);
										LightColor.push_back(0.2f);
										continue; 
								}

								validLights++;

								LightData.push_back(it->X());
								LightData.push_back(it->Y());
								LightData.push_back(it->Z());
								LightData.push_back(it->Radius());

								LightColor.push_back(it->R());
								LightColor.push_back(it->G());
								LightColor.push_back(it->B());
						}

						loca = location[shaderIndex].LightData;
						if (loca != -1)
								glUniform4fv(loca, validLights, LightData.data());

						loca = location[shaderIndex].LightColor;
						if (loca != -1)
								glUniform3fv(loca, validLights, LightColor.data());

						//Set Reciever's Uniforms
						loca = location[shaderIndex].isWall;
						if (loca != -1)
								glUniform1i(loca, isWall);

						loca = location[shaderIndex].TileZ;
						if (loca != -1)
								glUniform1f(loca, Z * 2);

						loca = location[shaderIndex].validLights;
						if (loca != -1)
								glUniform1i(loca, validLights);

						loca = glGetUniformLocation(ShaderArray[shaderIndex], "TileY");
						if (loca != -1)
								glUniform1f(loca, Spr->GetPosition().y + 32.f);

						//Normalmap Texture
						loca = location[shaderIndex].texture_normal;
						if (loca != -1)
						{
								glUniform1i(loca, 2);//Give it texture unit 2
								glActiveTexture(GL_TEXTURE0 + 2);
								glBindTexture(GL_TEXTURE_2D, GetTexture(Tex_Normal));
								//glBindSampler(2, 0);
						}
								
						//ColorMap colors
						loca = glGetUniformLocation(ShaderArray[shaderIndex], "real_color");
						if (loca != -1)
						{
								glUniform1fv(loca, 9, RealColors);
						}

						//LAVA EFFECT
						loca = glGetUniformLocation(ShaderArray[shaderIndex], "doLava");
						if (loca != -1)
						{
								glUniform1i(loca, doLava);

								loca = glGetUniformLocation(ShaderArray[shaderIndex], "timer");
								if(loca != -1)
										glUniform1f(loca, float(pEngine->run_timer.GetMilliseconds())/50.f);
						}

						//Set Wall information
						
						auto walls = lightManager->getWalls();
						auto norms = lightManager->getWallNormals();

						std::vector<float> wallData;
						std::vector<float> wallNormData;

						wallData.reserve(sizeof(float) * 500);
						wallNormData.reserve(sizeof(float) * 500);

						int validWalls = 0;

						//If enabled, get the info for the uniforms
						if (lightManager->getDoShadows())
						{
								//They should be the same size, but just in case...
								size_t size = walls.size();
								if (size > norms.size())
										size = norms.size();

								glm::vec2 camPos = mainCam.center();
								float camW = mainCam.width() * 1.1f, camH = mainCam.height()*1.3f;
								for (unsigned i = 0; i < size; ++i)
								{
										//validate if the wall is in the room
										if (abs(walls[i].x - camPos.x) > camW / 2 ||
												abs(walls[i].y - camPos.y) > camH / 2)
										{
												//++i;
												continue;
										}
										//Its visible in the camera
										validWalls++;

										wallData.push_back(walls[i].x);
										wallData.push_back(walls[i].y);
										wallData.push_back(walls[i].z);
										wallData.push_back(walls[i].w);

										wallNormData.push_back(norms[i].x);
										wallNormData.push_back(norms[i].y);
								}
						}

						loca = glGetUniformLocation(ShaderArray[shaderIndex], "validWalls");
						if (loca != -1)
								glUniform1i(loca, validWalls);

						loca = glGetUniformLocation(ShaderArray[shaderIndex], "LightWalls");
						if (loca != -1)
								glUniform4fv(loca, validWalls, wallData.data());

						loca = glGetUniformLocation(ShaderArray[shaderIndex], "LightWallNorms");
						if (loca != -1)
								glUniform2fv(loca, validWalls, wallNormData.data());
				}
		}
}

void LightReciever::CreateLightFromTileName(TileDictionary::tileName name, int floor)
{
		Sprite* spr = this->owner->GetComponent<Sprite>();
		if (!spr)
				return;
		glm::vec2 pos = spr->GetPosition();
		LightSource* LS;
		MultiEmitter* MEmitter = nullptr;
		std::vector<Emitter*>* e;
		switch (name)
		{
		case TileDictionary::Slab:
				//MEmitter = owner->AddComponent<MultiEmitter>();
				//MEmitter->addEmitter("tileglow.json");
				break;

		case TileDictionary::WindowTop:
				if(floor == 1)
						LS = lightManager->AddLight("window", false, .8f, .36f, .0f, 650.f);
				else if (floor == 2)
						LS = lightManager->AddLight("window", false, .6f, .1f, .15f, 650.f);	
				else if (floor == 3)
						LS = lightManager->AddLight("window", false, .04f, .0f, .1f, 650.f);
				else
						LS = lightManager->AddLight("window", false, .8f, .36f, .0f, 650.f);

				LS->Z(64);
				LS->X(pos.x);
				LS->Y(pos.y - 126.f);
				break;

		case TileDictionary::Torch:
				MEmitter = owner->AddComponent<MultiEmitter>();
				MEmitter->addEmitter("beacon1.json");
				MEmitter->addEmitter("beacon2.json");
						/*
				EM->setFileName("beacon.json");
				EM->Init(50, 1);
				EM->jsonLoad();
				*/

				LS = lightManager->AddLight("torch", false, .2f, .2f, .2f, 750.f);
				LS->Z(64.f);
				LS->X(pos.x);
				LS->Y(pos.y);
				spr->checkTorch(true);
				break;
		case TileDictionary::MagentaBowl:
				MEmitter = owner->AddComponent<MultiEmitter>();
				MEmitter->addEmitter("TT_walkthroughpart.json");
				MEmitter->addEmitter("TT_walkthroughpart.json");
				MEmitter->addEmitter("bf1.json");
				MEmitter->addEmitter("bf2.json");
				e = MEmitter->getEmitters();
				e->at(0)->setDoLoop(false);
				e->at(1)->setDoLoop(false);
				e->at(2)->setColorBases(1.f,0.1f,1.f);
				e->at(3)->setColorBases(1.f, 0.4f, 1.f);
				for (Emitter* emitter : *e)
				{
						Particle* particles = emitter->getParticles();
						for (int i = 0; i < emitter->getParticleCount(); ++i)
						{
								GameObject* obj = particles[i].getObject();
								obj->GetComponent<Sprite>()->setAlpha(0.f);
						}
				}
				break;
		case TileDictionary::CyanBowl:
				MEmitter = owner->AddComponent<MultiEmitter>();
				MEmitter->addEmitter("TT_walkthroughpart.json");
				//MEmitter->addEmitter("TT_walkthroughpart.json");
				MEmitter->addEmitter("bf1.json");
				MEmitter->addEmitter("bf2.json");
				e = MEmitter->getEmitters();
				e->at(0)->setDoLoop(false);
				//e->at(1)->setDoLoop(false);
				e->at(0)->setColorBases(.4f, 1.f, 1.f);
				e->at(1)->setColorBases(.1f, 1.f, 1.f);
				e->at(2)->setColorBases(.4f, 1.f, 1.f);
				for (Emitter* emitter : *e)
				{
						Particle* particles = emitter->getParticles();
						for (int i = 0; i < emitter->getParticleCount(); ++i)
						{
								GameObject* obj = particles[i].getObject();
								obj->GetComponent<Sprite>()->setAlpha(0.f);
						}
				}
				break;
		case TileDictionary::GiantBowlCorner:
				MEmitter = owner->AddComponent<MultiEmitter>();
				MEmitter->addEmitter("BigBowl1.json");

				LS = lightManager->AddLight("giantbowl", false, .4f, .1f, .1f, 750.f);
				LS->Z(64.f);
				LS->X(pos.x);
				LS->Y(pos.y);
				break;
		case TileDictionary::Coin:
				MEmitter = owner->AddComponent<MultiEmitter>();
				MEmitter->addEmitter("flame4.json");
				std::stringstream coinlabel;
				coinlabel << "coin" << owner;
				LS = lightManager->AddLight(coinlabel.str(), false, 1.f, .2f, .2f, 220.f);
				LS->X(pos.x);
				LS->Y(pos.y - 16.f);
				LS->Z(20.f);
				Z = 32.f;
				isWall = true;
				break;
		}

		if (MEmitter)
		{
				
		}
}
void LightReciever::CreateWallFromTileName(TileDictionary::tileName name)
{
		Sprite* spr = this->owner->GetComponent<Sprite>();
		if (!spr)
				return;
		glm::vec2 pos = spr->GetPosition();
		switch (name)
		{
		case TileDictionary::PillarBase: case TileDictionary::StatueBot:
				float width = 32.f; //width of the tile in world coordinates
				
				//Get coordinates for BottomLeft, TopRight, etc.
				float offset = 0.f;
				glm::vec2 BR = glm::vec2(pos.x + width - offset, pos.y - width - offset);
				glm::vec2 TR = glm::vec2(pos.x + width - offset, pos.y + width + offset);
				glm::vec2 TL = glm::vec2(pos.x - width + offset, pos.y + width + offset);
				glm::vec2 BL = glm::vec2(pos.x - width + offset, pos.y - width - offset);

				//Add the 4 walls to the light manager
				//Go counterclockwise
				/*
				lightManager->AddWall(BR, TR);
				lightManager->AddWall(TR, TL);
				lightManager->AddWall(TL, BL);
				lightManager->AddWall(BL, BR);
				*/
				
				//X method: 2 walls only, but disable normal optimization in shader
				lightManager->AddWall(TL, BR);
				lightManager->AddWall(TR, BL);
		}
}

void LightReciever::SetColorsFromTileName(TileDictionary::tileName name, int floor)
{
		SetColor3(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.5f, .02f));
		SetColor2(.85f, .2f, 0.05f);
		switch (name)
		{
		case TileDictionary::Brick:
				SetColor1(.35f, .45f, .4f);
				break;
		case TileDictionary::Slab:
				SetColor1(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.5f, .02f));
				break;

		case TileDictionary::PillarBase: case TileDictionary::PillarBroken: case TileDictionary::PillarMid: case TileDictionary::PillarTop:
				SetColor1(.6f, .75f, .6f);
				break;

		case TileDictionary::WindowTop: case TileDictionary::WindowBot:
				SetColor1(.35f, .45f, .4f);
				SetColor2(1.f, .85f, 0.f);
				if(floor == 2)
						SetColor2(.7f, .2f, .2f);
				if(floor == 3)
						SetColor2(.2f, .0f, .6f);

				break;
		case TileDictionary::DoorwayLeft:
				SetColor1(.35f, .45f, .4f);
				break;
		case TileDictionary::DoorwayRight:
				SetColor1(.35f, .45f, .4f);
				break;
		case TileDictionary::Carpet:
				SetColor1(.5f, .1f, .1f);
				break;
		case TileDictionary::CarpetArrow:
				SetColor1(.4f, .05f, .1f);
				break;
		case TileDictionary::Torch:
				SetColor1(0.9f, .8f, .4f);
				break;
		case TileDictionary::MagentaBowl:
				SetColor1(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.5f, .02f));
				SetColor2(.95f, .15f, .95f);
				break;
		case TileDictionary::CyanBowl:
				SetColor1(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.5f, .02f));
				SetColor2(.15f, .95f, .95f);
				break;
		case TileDictionary::StairsDown: case TileDictionary::StairsUp:
				SetColor1(.6f, .75f, .6f);
				break;
		case TileDictionary::Trail:
				SetColor3(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.5f, .02f));
				SetColor2(.85f, .2f, 0.05f);
				break;
		case TileDictionary::Coin:
				SetColor1(1.f, .8f, .2f);
				break;
		case TileDictionary::Rock:
				SetColor1(.6f, .6f, .75f);
				break;
		case TileDictionary::StatueTop: case TileDictionary::StatueBot:
				SetColor1(.9f, .9f, .9f);
				break;
		case TileDictionary::PitEdge:
				SetColor1(.35f, .45f, .4f);
				break;
		case TileDictionary::GiantBowl: case TileDictionary::GiantBowlCorner: case TileDictionary::GiantBowlEdge:
				SetColor1(floatRand(.5f, .02f), floatRand(.6f, .05f), floatRand(.4f, .02f));
				SetColor2(.6f, .05f, .05f);
				break;
		}
}

void LightReciever::SetColor1(float r, float g, float b)
{
		RealColors[0] = r;
		RealColors[1] = g;
		RealColors[2] = b;
}
void LightReciever::SetColor2(float r, float g, float b)
{
		RealColors[3] = r;
		RealColors[4] = g;
		RealColors[5] = b;
}
void LightReciever::SetColor3(float r, float g, float b)
{
		RealColors[6] = r;
		RealColors[7] = g;
		RealColors[8] = b;
}

