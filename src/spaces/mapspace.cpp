/******************************************************************************/
/*!
\file	mapspace.cpp
\author	Justice Mealer
\par    email: j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../graphics/DebugDraw.h"
#include "../graphics/Sprite.h"
#include "../graphics/Camera.h"
#include "../graphics/Tilemap.h"
#include "../component/collider.h"
#include "../component/collfix.h"
#include "../component/onewaycollider.h"
#include "../system/Engine.h"
#include <sstream>
#include <set>
#include "../system/common_defines.h"
#include "../graphics/Shader.h"
#include "../component/button.h"
#include "../graphics/Shader.h"
#include "../graphics/TileParser.h"
#include "../puzzles/newpuzzle.h"
#include "../vfx/LightReciever.h"
#include "../vfx/LightManager.h"
#include "../vfx/TileDictionary.h"
#include "../component/MultiEmitter.h"
#include "../graphics/Fade.h"
#include "../component/controller.h"
#include "../system/timermanager.h"
#include "../system/countdowntimer.h"
/*
Adding debug draw to puzzles:

creating new puzzle for each flower layer causes debug draw to not be attached properly due to position in json file and such bullshit


*/
#define SPRITESHEETHEIGHT 32
#define SPRITESHEETWIDTH 16



namespace Spaces
{
	void TransferCollisionHandler(GameObject * a, GameObject * b)
	{
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("Stairs"));
		spaceManager->LoadNextLevel();
	}

	void EndgameCollisionHandler(GameObject * a, GameObject * b)
	{
		if (IsFadeWorking() == false)
		{	
			// get bowl
			GameObject * bowl = (a->name.Has("player")) ? b : a;
			// get bowl's particle emitters
			MultiEmitter * me = bowl->GetComponent<MultiEmitter>();
			//me->addEmitter("finalbowl.json");
			Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Final_Flame"));
			CountdownTimer * cnt = TimerManager::NewCountdownTimer();
			cnt->SetTrigger(new MemberFunctionWrapper<void, SpaceManager, void>(&SpaceManager::LoadNextLevel, Spaces::spaceManager, nullptr));
			cnt->SetLimitSeconds(2);
			cnt->Start();
			TimerManager::AddTimer(cnt);

			nextmove = none;
		}
	}

	void TileCollisionHandler(GameObject * a, GameObject * b)
	{
		if (a && b)
		{
			// we don't care if two tiles are colliding
			if ((a->name.find("tile") != std::string::npos) && (b->name.find("tile") != std::string::npos))
			{
				return;
			}
			else if (a->name.find("player") != std::string::npos)
			{

			}
			else if (b->name.find("player") != std::string::npos)
			{
				DebugDraw * debugdraw = a->GetComponent<DebugDraw>();
				if (debugdraw)
				{
					debugdraw->SetTempColor(1.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	void AcornCollisionHandler(GameObject * a, GameObject * b)
	{
		if (a && b)
		{
			if (a->removing_ || b->removing_)
			{
				return;
			}
			if (a->name == "player")
			{
				//Data::pManager->AddHint();
				b->GetComponent<Sprite>()->setAlpha(0.0f);
				b->space->MarkObjectForRemoval(b->name);
			}
			else if (b->name == "player")
			{
				//Data::pManager->AddHint();
				Sprite * sp = a->GetComponent<Sprite>();
				if (sp)
				{
					sp->setAlpha(0.0f);
				}
				a->space->MarkObjectForRemoval(a->name);
			}
			Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Hint_PickUp"));
		}
	}

	/*void FlowerCollisionHandler(GameObject * a, GameObject * b)
	{
		if (a && b)
		{
			if (a->name.find("player") != std::string::npos)
			{
				Flower * fl = dynamic_cast<Flower *>(b->GetComponent<Flower>());
				if (fl)
				{
					if (fl->owner)
					{
						if (!fl->puzzle_->Completed())
						{
							fl->Activate();

						}
					}
				}
			}
			else if (b->name.find("player") != std::string::npos)
			{
				Flower * fl = dynamic_cast<Flower *>(a->GetComponent<Flower>());
				if (fl)
				{
					if (fl->owner)
					{
						if (!fl->puzzle_->Completed())
						{
							fl->Activate();

						}
					}
				}
			}
		}
	}

	void SolutionCollisionHandler(GameObject * a, GameObject * b)
	{
		if (a && b)
		{
			if ((a->name.find("tile") != std::string::npos) && (b->name.find("tile") != std::string::npos))
			{
				return;
			}
			else if (a->name.find("player") != std::string::npos)
			{

			}
			else if (b->name.find("player") != std::string::npos)
			{
				DebugPath * debugpath = a->GetComponent<DebugPath>();

				auto layers = Spaces::spaceManager->GetSpaces();
				if (!debugpath->puzzle_->Started())
				{
					debugpath->SetStatus(0);
					return;
				}


				if (debugpath)
				{
					if (debugpath->GetStatus() == 0)
					{
						Collider* cp = b->GetComponent<Collider>();
						debugpath->SetStatus(1);
						debugpath->SetPlayer(cp);
					}
					if (debugpath->GetStatus() == 2)
					{
						layers = Spaces::spaceManager->GetSpaces();
						for (auto l : *layers)
						{
							if (l.first.find("Solution") != std::string::npos)
							{
								for (auto obj : *l.second->GetObjectMap())
								{
									// check if the puzzle is completed
									DebugPath * db = obj.second->GetComponent<DebugPath>();
									if (db)
									{
										Puzzle * p = db->puzzle_;
										if (p)
										{
											if (!p->Completed())
											{
												db->ClearPath();
											}
										}
									}
								}
							}
							if (l.first.find("Flower") != std::string::npos)
							{
								for (auto obj : *l.second->GetObjectMap())
								{
									if (!obj.second->GetComponent<Flower>()->first_)
									{
										obj.second->GetComponent<Flower>()->puzzle_->Fail();
									}
								}
							}
						}
					}
				}
			}
		}
	}*/

	void HintCollisionHandler(GameObject* a, GameObject* b)
	{
		if (a && b)
		{
			if ((a->name.find("tile") != std::string::npos) && (b->name.find("tile") != std::string::npos))
			{
				return;
			}
			else if ((a->name.find("hint") != std::string::npos) && (b->name.find("player") != std::string::npos))
			{
				Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Hint_PickUp"));
				a->space->MarkObjectForRemoval(a->name);

				std::stringstream coinlabel;
				coinlabel << "coin" << a;
				lightManager->DeleteLight(coinlabel.str());

				PzManage::pzm->hints++;
			}
			else if ((b->name.find("hint") != std::string::npos) && (a->name.find("player") != std::string::npos))
			{
				Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Hint_PickUp"));
				a->space->MarkObjectForRemoval(a->name);

				std::stringstream coinlabel;
				coinlabel << "coin" << a;
				lightManager->DeleteLight(coinlabel.str());

				PzManage::pzm->hints++;
			}
		}
	}

	void PuzzleCollisionHandler(GameObject* a, GameObject* b)
	{
		if (a && b)
		{
			if(trailresetswitch == true)
			{
				PzManage::pzm->ResetCurrentPuzzle();
				return;
			}
			if ((a->name.find("tile") != std::string::npos) && (b->name.find("tile") != std::string::npos))
			{
				return;
			}
			else if (((a->name.find("Bowl") != std::string::npos) && ((b->name.find("player") != std::string::npos))))
			{
				PuzzleNode* node = a->GetComponent<PuzzleNode>();
				
				if (!node->isCol)
				{
					if (node->GetParent()->isCompleted == false)
					{
						MultiEmitter* me = a->GetComponent<MultiEmitter>();
						if (me)
						{
							Physics* playerPhy = b->GetComponent<Physics>();
							Vector2D playerVel = playerPhy->Velocity();

							std::vector<Emitter*>* e = me->getEmitters();
							
							bool isCyan = false;
							if (a->name.find("Cyan") != std::string::npos)
									isCyan = true;

							//whether hes moving horizontally
							//defaults to Up
							char direction = 0b1000;
							//Left
							if (playerVel[0] < 0)
									direction = 0b0001;
							//Right
							if (playerVel[0] > 0)
									direction = 0b0010;
							//Down
							if (playerVel[1] < 0)
									direction = 0b0100;

							//
							for(int i = 0; i <= 1; i++)
							if (e->at(i) && e->at(i)->getFileName().find("TT_walkthroughpart") != std::string::npos)
							{
									float mirror = 1.f;
									if (i == 1)
											mirror = -1.f;

									if (isCyan)
									{
											if (direction & 0b1000)//UP
													e->at(i)->setPosOffset(0.f, 64.f * mirror);
											if (direction & 0b0100)//DOWN
													e->at(i)->setPosOffset(0.f, -64.f * mirror);
											if (direction & 0b0010)//RIGHT
													e->at(i)->setPosOffset(64.f * mirror, 0.f * mirror);
											if (direction & 0b0001)//LEFT
													e->at(i)->setPosOffset(-64.f * mirror, 0.f * mirror);
									}
									else
									{
											if (direction & 0b0010)//RIGHT
													e->at(i)->setPosOffset(0.f, 64.f * mirror);
											if (direction & 0b0001)//LEFT
													e->at(i)->setPosOffset(0.f, -64.f * mirror);
											if (direction & 0b1000)//UP
													e->at(i)->setPosOffset(64.f * mirror, 0.f * mirror);
											if (direction & 0b0100)//DOWN
													e->at(i)->setPosOffset(-64.f * mirror, 0.f * mirror);
									}

									//e->at(i)->setDoLoop(true);
									e->at(i)->Init(e->at(i)->getParticleCount(), e->at(i)->getLifetime());
							}
						}
						node->isCol = true;
					}
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Brazier"));
				}
				node->Activate();


			}
			else if ((b->name.find("Bowl") != std::string::npos) && (a->name.find("player") != std::string::npos))
			{
				PuzzleNode* node = b->GetComponent<PuzzleNode>();
				
				if (!node->isCol)
				{
					if (node->GetParent()->isCompleted == false)
					{
						MultiEmitter* me = b->GetComponent<MultiEmitter>();
						if (me)
						{
							Physics* playerPhy = a->GetComponent<Physics>();
							Vector2D playerVel = playerPhy->Velocity();

							std::vector<Emitter*>* e = me->getEmitters();
							float* angle = (*e)[0]->getAngle();
							//Left
							if (playerVel[0] < 0)
								angle[0] += 3.14 / 2;
							//Right
							if (playerVel[0] > 0)
								angle[0] -= 3.14 / 2;
							//Down
							if (playerVel[1] < 0)
								angle[0] += 3.14;

							if (e->at(0))
							{
								e->at(0)->setAngle(angle[0], angle[1]);
								e->at(0)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
							}
							if (e->at(1))
							{
								e->at(1)->setAngle(angle[0] + 3.14f, angle[1]);
								e->at(1)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
							}
						}
						node->isCol = true;
					}
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Brazier"));
				}
				node->Activate();


			}
		}
	}
	void StairCollisionHandler(GameObject*a, GameObject*b)
	{
		if (a && b)
		{
			if ((a->name.find("tile") != std::string::npos) && (b->name.find("tile") != std::string::npos))
			{
				return;
			}
			else if (((a->name.find("stair") != std::string::npos) && ((b->name.find("player") != std::string::npos))))
			{
				//Spaces::spaceManager->QueueClearEvent();
				//Spaces::spaceManager->QueueBuildEvent();
			}
			else if (((b->name.find("stair") != std::string::npos) && ((a->name.find("player") != std::string::npos))))
			{
				//send message here
				// " "
			}
		}
	}

	/*void BuildMapspace(const std::string& filepath)
	{
		std::set<int> tilesWithColliders; // a list of the IDs for each tile that has a collider

		Space * tilemap = spaceManager->AddSpace("999 mapspace");
		Tilemap tile(filepath.c_str());
		Puzzle * pz = nullptr;

		for (int i = 0; i < tile.getLayers(); ++i)
		{
			if (tile.isCollisionLayer(i))
			{
				for (int j = 0; j < tile.getWidth()*tile.getHeight(); ++j)
				{
					if (tile.getData(j, i) != 0)
					{
						tilesWithColliders.insert(tile.getData(j, i));
					}
				}
			}
		}

		for (int i = 0; i < tile.getLayers(); i++)
		{
			if (!tile.isVisibleLayer(i))
			{
				continue;
			}
			std::ostringstream spos;
			spos << i << " " << tile.getName(i);
			Space * spa = spaceManager->AddSpace(spos.str());

			if (spos.str().Has("Flower"))
			{
				pz = new Puzzle(spos.str());
				Data::pManager->AddPuzzle(pz);
			}

			for (int j = 0; j < tile.getWidth()*tile.getHeight(); j++)
			{
				if (tile.getData(j, i))
				{
					std::ostringstream os;

					os << "tile: layer: " << i << " #" << j << " id: " << tile.getData(j, i);
					GameObject * go = spa->AddObject(os.str());
					Sprite * sprite = go->AddComponent<Sprite>();
					sprite->Init(Shader_TileLighting);
					Animation* anim = go->AddComponent<Animation>();
					Transform* transform = go->AddComponent<Transform>();

					LightReciever* LR = go->AddComponent<LightReciever>();


					sprite->setData(tile.getData(j, i));
					if (i == 2)
					{
						sprite->setAlpha(1.0f);
					}
					if (sprite->getData() == 129)
					{
						sprite->setAlpha(1.0f);
					}

					anim->Init(31, 31, 961, 1);

					anim->setAnimationSpeed(0);

					for (int z = 0; z < 961; z++)
					{
						(*anim)[z][0] = z;
					}

					anim->print();

					sprite->SetTexture(1);

					anim->setSequence(tile.getData(j, i) - 1);

					sprite->SetCamera(mainCam);

					transform->Scale(Vector2D(64.0f, 64.0f));
					sprite->SetSize(64.0f, 64.0f);

					int x = tile.getX(j, i);
					int y = tile.getY(j, i);
					transform->Translation(Vector2D((float)x - 832.0f + 32.0f, (float)y + 512.0f - 32.0f));
					sprite->SetPosition((float)x - 832.0f + 32.0f, (float)y + 512.0f - 32.0f);

					int xid = x / 64;
					int yid = abs(y / 64);

					sprite->setMap(xid, yid);

					if (tile.getName(i).Has("Solution"))
					{
						sprite->setAlpha(0.0f);
						Collider * collider = go->AddComponent<Collider>();
						DebugPath * dPath = go->AddComponent<DebugPath>();

						collider->SetTransform(transform);
						collider->SetHandler(SolutionCollisionHandler);

					}
					else if (tilesWithColliders.find(tile.getData(j, i)) != tilesWithColliders.end())
					{
						/*if (TileName(i).Has("Ledge"))
						{
							continue;
						}
						Collider * collider = go->AddComponent<Collider>();
						DebugDraw * debug = go->AddComponent<DebugDraw>();

						collider->SetTransform(transform);
						if (tile.getName(i).find("Acorn") != std::string::npos)
						{
							collider->SetHandler(AcornCollisionHandler);
						}
						else if (tile.getName(i).find("Flower") != std::string::npos)
						{
							trailmap[xid][yid] = 2;
							collider->SetHandler(FlowerCollisionHandler);
							Flower * fl = go->AddComponent<Flower>();
							pz->AddPoint(go);
						}
						else if (TileName(i).Has("Level"))
						{
							collider->SetHandler(TransferCollisionHandler);

						}
						else if (TileName(i).Has("Ledge"))
						{
							collider->SetType(Collider::ColType::OneWay);
							if (TileName(i).Has("Up"))
							{
								collider->SetDir(Collider::ColDir::Up);
							}
							else if (TileName(i).Has("Right"))
							{
								collider->SetDir(Collider::ColDir::Right);
							}
							else if (TileName(i).Has("Left"))
							{
								collider->SetDir(Collider::ColDir::Left);
							}
							else if (TileName(i).Has("Down"))
							{
								collider->SetDir(Collider::ColDir::Down);
							}
							else if (TileName(i).Has("Corner"))
							{
								collider->SetType(Collider::ColType::Box);
							}
							collider->SetHandler(TileCollisionHandler);
						}
						else
						{
							collider->SetHandler(TileCollisionHandler);
						}

					}
				}
			}
		}
		Data::pManager->InitPuzzles();
	}*/

	void BuildNewMapSpace(const std::string& filepath)
	{
		Space * tilemap = spaceManager->AddSpace("999 mapspace");
		LayerMap layerMap(filepath.c_str());

		//Add a camera light for the level
		LightSource* LP = lightManager->AddLight("camera", false, .44f, .15f, .05f, 3000.f);
		LP->Z(1250.f);
		//Add a light for each room
		/*
		for(int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
						LightSource* LP = lightManager->AddLight("room", false, 1.f, .475f, .36f, 1400.f);
						LP->X(0 + 1728 * i);
						LP->Y(0 - 1100 * j);
						LP->Z(200.f);
				}
				*/
		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				trailmap[i][j].count = 0;
				trailmap[i][j].status = 0;
			}
		}
		std::vector<Layer>* layers = layerMap.GetLayers();
		int size = layers->size();
		for (int i = 0; i < size; i++)
		{
			Layer& currLayer = layers->at(i);
			std::ostringstream spos;
			spos << i << " " << currLayer.GetName();
			Space * spa = spaceManager->AddSpace(spos.str());
			std::vector<int>* tiles = currLayer.GetTiles();
			std::vector<int>* xPos = currLayer.GetXData();
			std::vector<int>* yPos = currLayer.GetYData();
			float zHeight = currLayer.GetZPos();
			if (currLayer.GetName().Has("room"))
			{
				Puzzle * pu = new Puzzle(currLayer.GetName(), PzManage::pzm->numpuzzles + 1);
				PzManage::pzm->AddPuzzle(pu);
				spos << " " << "Puzzle: " << pu->puzzleNumber;
			}

			for (unsigned int j = 0; j < tiles->size(); j++)
			{
				if (tiles->at(j) == 0)
				{
					continue;
				}
				std::ostringstream os;
				os << "tile: " << j << " layer: " << i << " id: " << tiles->at(j);
				//21, 22, 37, 38 = Magenta Bowl
				if (tiles->at(j) == 21 || tiles->at(j) == 22 || tiles->at(j) == 37 || tiles->at(j) == 38 || tiles->at(j) == 4)
				{
					os << " Magenta Bowl " << currLayer.GetName();
				}
				if (tiles->at(j) == 5)
				{
					os << " Cyan Bowl " << currLayer.GetName();
				}
				if (tiles->at(j) == 43)
				{
					os << " hint";
				}
				if (currLayer.GetName().Has("door"))
				{
					os << currLayer.GetName();
				}
				if (currLayer.GetName().Has("stair"))
				{
					os << currLayer.GetName();
				}
				if (currLayer.GetName().Has("endgam"))
				{
					os << currLayer.GetName();
				}
				if (currLayer.GetName().Has("emitter"))
				{
					os << currLayer.GetName();
				}
				
				GameObject * go = spa->AddObject(os.str());
				Sprite * sprite = go->AddComponent<Sprite>();
				sprite->Init(Shader_TileLighting);
				Animation* anim = go->AddComponent<Animation>();
				Transform* transform = go->AddComponent<Transform>();

				if (currLayer.GetName().Has("door"))
				{
					sprite->setPrior(1);
				}
				if (currLayer.GetName().Has("arch"))
				{
					sprite->setPrior(3);
				}
				if (currLayer.GetName().Has("goocol_top"))
				{
					sprite->setPrior(3);
				}
				if (currLayer.GetName().Has("wall_top"))
				{
					sprite->setPrior(3);
				}
				if (currLayer.GetName().Has("wall_mid"))
				{
					sprite->setPrior(3);
				}
				if (currLayer.GetName().Has("col_mid"))
				{
					sprite->setPrior(3);
				}
				if (currLayer.GetName().Has("col_top"))
				{
					sprite->setPrior(3);
				}

				sprite->setData(tiles->at(j));

				anim->Init(SPRITESHEETHEIGHT, SPRITESHEETWIDTH, SPRITESHEETWIDTH * SPRITESHEETHEIGHT, 1);

				anim->setAnimationSpeed(0);

				for (int z = 0; z < (SPRITESHEETHEIGHT * SPRITESHEETWIDTH); z++)
				{
					(*anim)[z][0] = z;
				}
				sprite->SetTexture(1);

				anim->setSequence(tiles->at(j) - 1);

				sprite->SetCamera(mainCam);

				transform->Scale(Vector2D(64.0f, 64.0f));
				sprite->SetSize(64.0f, 64.0f);

				int x = xPos->at(j);
				int y = yPos->at(j);
				// x =  + 32.0f
				transform->Translation(Vector2D((float)x - 864.0f - 448.0f, (float)y + 560.0f - 32.0f + 320.0f));
				sprite->SetPosition((float)x - 864.0f - 448.0f, (float)y + 560.0f - 32.0f + 320.0f);

				int xid = x / 64;
				int yid = abs(y / 64);

				if (currLayer.GetName().Has("hint"))
				{
				}
				else
				{
					sprite->setMap(xid, yid);
					trailmap[xid][yid].sprite[trailmap[xid][yid].count] = sprite;
					trailmap[xid][yid].count++;
					sprite->SetOriginSequence(tiles->at(j) - 1);
				}

				if (currLayer.GetCollision())
				{
					if (currLayer.GetName().Has("room"))
					{
						//Magenta bowl
						if (tiles->at(j) == 21 || tiles->at(j) == 22 || tiles->at(j) == 37 || tiles->at(j) == 38 || tiles->at(j) == 4)
						{
							Collider* collider = go->AddComponent<Collider>();
							DebugDraw* debug = go->AddComponent<DebugDraw>();
							collider->SetTransform(transform);
							collider->SetHandler(PuzzleCollisionHandler);
							collider->coldata = "puzzle";

							PuzzleNode* pzN = go->AddComponent<PuzzleNode>();
							pzN->Init(PuzzleNode::Type::magenta);
							std::vector<Puzzle*>* pzs = PzManage::pzm->GetPuzzles();
							Puzzle* pz = pzs->at(PzManage::pzm->numpuzzles);
							pz->AddNode(pzN);
							Puzzle* parent = pzN->GetParent();
							parent = pz;

							bowlmap[xid][yid] = 1;
						}
						//Cyan
						if (tiles->at(j) == 5)
						{
							Collider* collider = go->AddComponent<Collider>();
							DebugDraw* debug = go->AddComponent<DebugDraw>();
							collider->SetTransform(transform);
							collider->SetHandler(PuzzleCollisionHandler);
							collider->coldata = "puzzle";

							PuzzleNode* pzN = go->AddComponent<PuzzleNode>();
							pzN->Init(PuzzleNode::Type::cyan);
							std::vector<Puzzle*>* pzs = PzManage::pzm->GetPuzzles();
							Puzzle* pz = pzs->at(PzManage::pzm->numpuzzles);
							pz->AddNode(pzN);

							bowlmap[xid][yid] = 1;
						}

					}
					else if (currLayer.GetName().Has("col_mid") || currLayer.GetName().Has("col_top"))
					{
					}
					else if (currLayer.GetName().Has("floor_markings"))
					{

					}
					else if (currLayer.GetName().Has("hint"))
					{
						Collider* collider = go->AddComponent<Collider>();
						DebugDraw* debug = go->AddComponent<DebugDraw>();

						collider->SetTransform(transform);
						collider->SetHandler(HintCollisionHandler);
						collider->coldata = "hint";
					}
					else if (currLayer.GetName().Has("up_mid"))
					{

					}
					else if (currLayer.GetName().Has("up_base"))
					{
						Collider * collider = go->AddComponent<Collider>();
						DebugDraw * debug = go->AddComponent<DebugDraw>();
						collider->SetTransform(transform);
						collider->SetHandler(TransferCollisionHandler);
						collider->coldata = "stairs";
					}
					else if (currLayer.GetName().Has("endgam"))
					{
						Collider * collider = go->AddComponent<Collider>();
						DebugDraw * debug = go->AddComponent<DebugDraw>();
						collider->SetTransform(transform);
						collider->SetHandler(EndgameCollisionHandler);
						collider->coldata = "endgame";
					}
					else
					{

						Collider * collider = go->AddComponent<Collider>();
						DebugDraw * debug = go->AddComponent<DebugDraw>();

						collider->SetTransform(transform);


						collider->SetHandler(TileCollisionHandler);

						collider->coldata = "tile";

						wallmap[xid][yid] = 1;

					}

				}
				if (go->name.Has("door"))
				{
					PzManage::dorm->AddDoor(go);
					if (go->name.Has("door_up") || go->name.Has("door_down") || go->name.Has("door_left_base") || go->name.Has("door_right_base"))
					{
						MultiEmitter *me = go->AddComponent<MultiEmitter>();
						me->addEmitter("TTdust1.json");
						me->addEmitter("TTdust2.json");
						me->addEmitter("TTstones.json");
						
						auto e = me->getEmitters();
						e->at(0)->setAlpha(0.0f, 0.0f);
						e->at(1)->setAlpha(0.0f, 0.0f);
						e->at(2)->setAlpha(0.0f, 0.0f);
						e->at(0)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
						e->at(1)->Init(e->at(1)->getParticleCount(), e->at(1)->getLifetime());
						e->at(2)->Init(e->at(2)->getParticleCount(), e->at(2)->getLifetime());
						
						//Sprite* temps = go->GetComponent<Sprite>();
						//temps->setPrior(6);
					}
				}
				if (go->name.Has("emitter"))
				{
					MultiEmitter* me = go->AddComponent<MultiEmitter>();
					me->addEmitter("PuzzleAmb.json");
					me->addEmitter("PuzzleBurst.json");
					auto e = me->getEmitters();
					e->at(0)->setAlpha(0.0f, 0.0f);
					e->at(1)->setAlpha(0.0f, 0.0f);
					e->at(0)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
					e->at(1)->Init(e->at(1)->getParticleCount(), e->at(1)->getLifetime());

					PzManage::pzm->AddEmitter(me);
					
				}

				/**/
				//LIGHT RECIEVER HANDLING / TILE SPECIFIC THINGS

				int levelNum = 1;
				if (filepath.Has("Two"))
				{
						levelNum = 2;
						LP->R(.376f);
						LP->G(.125f);
						LP->B(.17f);
				}
				if (filepath.Has("Three"))
				{
						levelNum = 3;
						LP->R(-.06f);
						LP->G(.15f);
						LP->B(.25f);
				}
				if (filepath.Has("Four"))
				{
						levelNum = 4;
						LP->R(.0f);
						LP->G(.05f);
						LP->B(.05f);
				}

				LightReciever* LR = go->AddComponent<LightReciever>();
				TileDictionary::tileName name = TileDictionary::getName(tiles->at(j));
				LR->SetColorsFromTileName(name, levelNum);
				LR->CreateLightFromTileName(name, levelNum);
				LR->CreateWallFromTileName(name);//Lighting walls, (not colliders)
				LR->setZ(zHeight);
				if ((tiles->at(j) - 1) % SPRITESHEETWIDTH > SPRITESHEETWIDTH / 2)
				{
					LR->setIsWall(1);
				}
				/**/
			}
		}

	}
}
