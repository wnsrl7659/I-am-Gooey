/******************************************************************************/
/*!
\file	newpuzzle_r.cpp
\author Justice Mealer
\par    email: j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include "newpuzzle.h"
#ifdef R_FILE
#define _USE_MATH_DEFINES
#include "../component/MultiEmitter.h"
#include "../component/collider.h"
#include "../settings/settings.h"
#include "../graphics/Camera.h"
#include "../component/controller.h"
#include "../audio/audio.h"
#include "../graphics/DebugDraw.h"
#include "../spaces/spaces.h"
#include "../physics/Physics.h"
#include <math.h>

#define NODE_C play_brazier_correct
#define NODE_I play_brazier_incorrect
#define INCORRECT incorrect_movement
#define COMPLETE complete_puzzle
#define PCHECK check_puzzle_completed


static void play_brazier_correct()
{
	Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Brazier_Conditional"));
	Audio::EventData ed = { "Brazier_Conditional", "Brazier", 0.0f };
	Audio::EditParam(&ed);
}

static void play_brazier_incorrect()
{
	Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Brazier_Conditional"));
	Audio::EventData ed = { "Brazier_Conditional", "Brazier", 0.51f };
	Audio::EditParam(&ed);

}

void PuzzleManager::incorrect_movement()
{
	if (activeNode->parent->isCompleted == false)
	{
		activeNode->isActive = false;
		NODE_I(); // play node fail noise
	}
	nodeIsActive = false;
	activeNode = nullptr;
	ResetCurrentPuzzle();
}

void PuzzleManager::complete_puzzle()
{
	if (activeNode->parent->isCompleted == false)
	{
		// mark puzzle as completed and play audio
		PuzzleEvent pz(activeNode->parent->puzzleNumber, PuzzleEvent::State::completed);
		Messaging::messHandler->HandleMessage<PuzzleEvent>("PuzzleEvent", &pz);
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Brazier_Conditional"));
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Puzzle_Finish"));
		activeNode->parent->isCompleted = true;
		for (auto& me : puzzcompemitters)
		{
			glm::vec2 camPos = mainCam.center();
			float camW = mainCam.width(), camH = mainCam.height();
			if (me)
			{
				Transform* trans = me->owner->GetComponent<Transform>();
				if (abs(trans->Translation()[0] - camPos.x) > camW / 2 || abs(trans->Translation()[1] - camPos.y) > camH / 2)
				{
					continue;
				}
				else
				{
					auto e = me->getEmitters();
					e->at(0)->setAlpha(1.0f, 0.0f);
					e->at(1)->setAlpha(1.0f, 0.0f);
					e->at(0)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
					e->at(1)->Init(e->at(1)->getParticleCount(), e->at(1)->getLifetime());
					e->at(1)->setDoLoop(false);
				}
			}
			
		}
	}
	trailswitch = false; // stops drawing the trail
	trailbook = false;
	trailReset(); // removes the trail on the ground
}

bool PuzzleManager::check_puzzle_completed()
{
	// checking also that the character has made the loop
	// check that there is more than one node in puzzle
	if (activeNode->parent->nodes.size() > 1 && activeNode->isStart)
	{
		// check if all nodes have been activated
		if (activeNode->parent->Check())
		{
			COMPLETE();
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


PuzzleManager::PuzzleManager()
{
	//no active node, set it to nullptr
	nodeIsActive = false;
	activeNode = nullptr;
	//start at -1 so first puzzle is 0
	numpuzzles = -1;
	//hints start at none
	hints = 0;
	//attach input event guy
	Messaging::messHandler->Attach<PuzzleManager, PuzzleEvent, &PuzzleManager::EventCheck>(this, "PuzzleEvent");
	Messaging::messHandler->Attach<PuzzleManager, std::string, &PuzzleManager::WorldEvents>(this, "world event");
}
PuzzleManager::~PuzzleManager()
{
	for (unsigned int i = 0; i < puzzles.size(); i++)
	{
		delete puzzles.at(i);
	}
	puzzles.clear();

	//starts at negative 1, since it is the index for the puzzles, and negative 1 is no puzzles. increments whenever puzzle is added, so it starts at 0 for the first puzzle
	numpuzzles = -1;
	trailswitch = false;
	trailbook = false;
	//detach event guys
	Messaging::messHandler->Detach<PuzzleManager>(this, "PuzzleEvent");
	Messaging::messHandler->Detach<PuzzleManager>(this, "world event");
}
void PuzzleManager::AddPuzzle(Puzzle* pz)
{
	pz->parent = this;
	numpuzzles++;
	//add puzzle and make the manager the puzzle's parent
	puzzles.push_back(pz);

}
void PuzzleManager::ActivatedNode(PuzzleNode* pz)
{
	if (pz)
	{
		nodeIsActive = true;
		activeNode = pz;
	}
}
void PuzzleManager::Clear()
{
	nodeIsActive = false;
	activeNode = nullptr;
	for (unsigned int i = 0; i < puzzles.size(); i++)
	{
		delete puzzles.at(i);
	}
	puzzles.clear();
	puzzcompemitters.clear();
	numpuzzles = -1;
	trailswitch = false;
	trailbook = false;
}
std::vector<Puzzle*>* PuzzleManager::GetPuzzles()
{
	return &puzzles;
}
int PuzzleManager::CompletedPuzzles()
{
	int numcomplete = 0;
	for (auto &p : puzzles)
	{
		if (p->isCompleted)
		{
			numcomplete++;
		}
	}
	return numcomplete;
}

void PuzzleManager::Update(float dt)
{
	//if there is an active node
	if (nodeIsActive && activeNode)
	{
		// check that there is more than one node in puzzle
		// checking also that the character has made the loop
		if (PCHECK()) return;

		//if the bowl is cyan
		if (activeNode->nodeType == PuzzleNode::Type::cyan)
		{
			//repeat movement == gucci
			if (firstmove == secondmove)
			{
				// if this is the first node touched in the puzzle
				// turn on the trail drawing
				if (activeNode->isStart)
				{
					trailbook = true;
					//trailswitch = true;
					//pastmove = none;
				}
				//yo soy node logico bueno
				if (Settings::debug)
				{
					std::cout << "Successful Node" << std::endl;
				}
				// check if the puzzle is only one node and it's been activated correctly
				if (activeNode->parent->nodes.size() == 1 && activeNode->parent->Check())
				{
					COMPLETE();
					
				}
				if (activeNode->parent->isCompleted == false)
				{
					NODE_C(); // play brazier correct noise
					
				}

				nodeIsActive = false;
				activeNode = nullptr;
				return;
			}
			// player has made incorrect movement
			else if (secondmove != none)
			{
				INCORRECT();
				return;
			}
		}

		//bowl is magenta
		else if (activeNode->nodeType == PuzzleNode::Type::magenta)
		{
			//opp movement == gucci
			if (firstmove == up || firstmove == down)
			{
				if (secondmove == left || secondmove == right)
				{
					if (activeNode->isStart)
					{
						trailswitch = true;
						//pastmove = none;
					}

					//put node logic here xd
					if (Settings::debug)
					{
						std::cout << "Successful Node" << std::endl;
					}
					if (activeNode->parent->nodes.size() == 1 && activeNode->parent->Check())
					{
						COMPLETE();
					}

					if (activeNode->parent->isCompleted == false)
					{
						NODE_C();
					}
					nodeIsActive = false;
					activeNode = nullptr;
					return;
				}
				//bad move
				else if (secondmove != none)
				{
					INCORRECT();
					return;
				}
			}
			else if (firstmove == right || firstmove == left)
			{
				if (secondmove == up || secondmove == down)
				{
					if (activeNode->isStart)
					{
						trailswitch = true;
						//pastmove = none;
					}
					
					//put node logic here xd
					if (Settings::debug)
					{
						std::cout << "Successful Node" << std::endl;
					}
					if (activeNode->parent->nodes.size() == 1 && activeNode->parent->Check())
					{
						COMPLETE();
					}

					if (activeNode->parent->isCompleted == false)
					{
						NODE_C();
					}
					nodeIsActive = false;
					activeNode = nullptr;
					return;
				}
				//bad move, none does not count as a move
				else if (secondmove != none)
				{
					INCORRECT();
					return;
				}
			}
		}
	}
}

void PuzzleManager::ResetCurrentPuzzle()
{
	Puzzle* puzzle = puzzles.at(CompletedPuzzles());
	for (auto p : puzzle->nodes)
	{
		p->isActive = false;
		p->isCol = false;
		p->isStart = false;
	}
	trailReset();
	trailswitch = false;
	trailbook = false;
	/*for (auto& me : puzzcompemitters)
	{
		glm::vec2 camPos = mainCam.center();
		float camW = mainCam.width(), camH = mainCam.height();
		if (me)
		{
			Transform* trans = me->owner->GetComponent<Transform>();
			if (abs(trans->Translation()[0] - camPos.x) > camW / 2 || abs(trans->Translation()[1] - camPos.y) > camH / 2)
			{
				continue;
			}
			else
			{
				if (!me->owner->space->GetObject("Fail"))
				{
					GameObject* fail = me->owner->space->AddObject("Fail");
					Transform* t = fail->AddComponent<Transform>();
					t->Translation(trans->Translation());
					MultiEmitter* m = fail->AddComponent<MultiEmitter>();
					m->addEmitter("LevelFail.json");
					Physics* p = fail->AddComponent<Physics>();
					p->Velocity(Vector2D(0.0f, 20.0f));
				}

			}
		}

	}*/
	

}

void PuzzleManager::EventCheck(PuzzleEvent* pe)
{
	if (pe->currState == PuzzleEvent::failed)
	{
		puzzles.at(pe->puzzleNumber)->Invalidate();
		trailswitch = false;
		trailbook = false;
	}
}

void PuzzleManager::WorldEvents(std::string* we)
{
	if (strncmp("reset", we->c_str(), 6) == 0)
	{
		trailswitch = false;
		trailbook = false;
		for (auto &p : puzzles)
		{
			p->Invalidate();
		}
	}
}

Puzzle::Puzzle(std::string n, int num)
{
	name = n;
	puzzleNumber = num;
	isCompleted = false;

}
Puzzle::~Puzzle()
{
	nodes.clear();
}
void Puzzle::AddNode(PuzzleNode* node)
{
	node->parent = this;

	nodes.push_back(node);

}
bool Puzzle::Check()
{
	int count = 0;
	int startCount = 0;
	for (auto &v : nodes)
	{
		if (v->isActive)
		{
			count++;
			if (Settings::debug)
				std::cout << "Active Nodes: " << count << std::endl;
		}
		if (v->isStart)
		{
			startCount++;
			if (Settings::debug)
				std::cout << "Start: " << startCount << std::endl;
		}
	}
	if (count == nodes.size() && startCount == 1)
	{

		return true;
	}
	else
	{
		return false;
	}
}
std::vector<PuzzleNode*>* Puzzle::GetNodes()
{
	return &nodes;
}
void Puzzle::Invalidate()
{
	for (int i = 0; i < nodes.size(); ++i)
	{
		nodes.at(i)->isActive = false;
		nodes.at(i)->isStart = false;
		nodes.at(i)->isCol = false;
	}
}

PuzzleNode::PuzzleNode()
{
	isActive = false;
	isStart = false;
	isCol = false;
}
PuzzleNode::~PuzzleNode()
{

}
void PuzzleNode::Init(PuzzleNode::Type type)
{
	nodeType = type;
}
void PuzzleNode::Update(float dt)
{
	MultiEmitter* me = owner->GetComponent<MultiEmitter>();
	if (me)
	{
		if (!light)
		{
			light = lightManager->AddLight("puzzlenode");
			Transform* trs = owner->GetComponent<Transform>();
			if (nodeType == Type::cyan)
			{
				light->R(.0f);
				light->G(1.f);
				light->B(1.f);
			}
			if (nodeType == Type::magenta)
			{
				light->R(1.0f);
				light->G(0.f);
				light->B(1.f);
			}
			light->X(trs->Translation()[0]);
			light->Y(trs->Translation()[1]);
			light->Z(18.f);
		}
		if (isActive)
		{
			auto emitters = me->getEmitters();
			if (emitters)
			{
				int eSize = emitters->size();
				if (eSize > 2)
				{
					if ((*emitters)[eSize - 2])
						emitters->at(eSize - 2)->setAlpha(1.f, 0);
					if ((*emitters)[eSize - 1])
						emitters->at(eSize - 1)->setAlpha(1.f, 0);
				}
			}
			if (light)
				light->BaseRadius(360.f);
		}
		else
		{
			auto emitters = me->getEmitters();
			if (emitters)
			{
				int eSize = emitters->size();
				if (eSize > 2)
				{
					if ((*emitters)[eSize - 2])
						emitters->at(eSize - 2)->setAlpha(0.f, 0);
					if ((*emitters)[eSize - 1])
						emitters->at(eSize - 1)->setAlpha(0.f, 0);
				}
			}
			if (light)
				light->BaseRadius(96.f);

		}
	}

}
void PuzzleNode::Draw() const
{

}

void PuzzleNode::Activate()
{
	if (parent->isCompleted == false)
	{
		isActive = true;
		int activeCount = 0;
		for (auto &v : parent->nodes)
		{
			if (v != this)
			{
				if (v->isActive)
				{
					activeCount++;
				}
			}
		}
		if (activeCount == 0)
		{
			isStart = true;
		}
		if (isCol)
			parent->parent->ActivatedNode(this);
	}
}
Puzzle* PuzzleNode::GetParent()
{
	return parent;
}

DoorManager::DoorManager()
{
	Messaging::messHandler->Attach<DoorManager, PuzzleEvent, &DoorManager::Check>(this, "PuzzleEvent");
	Messaging::messHandler->Attach<DoorManager, std::string, &DoorManager::WorldEvents>(this, "world event");
}
DoorManager::~DoorManager()
{
	doors.clear();
	Messaging::messHandler->Detach<DoorManager>(this, "PuzzleEvent");
	Messaging::messHandler->Detach<DoorManager>(this, "world event");
}
void DoorManager::AddDoor(GameObject* go)
{
	doors.push_back(go);
}
std::list<GameObject*>* DoorManager::GetDoors()
{
	return &doors;
}
void DoorManager::Check(PuzzleEvent* pz)
{
	if (pz->currState == PuzzleEvent::State::completed)
	{
		//x - camx > camw / 2 || y - camy > camh / 2
		glm::vec2 camPos = mainCam.center();
		float camW = mainCam.width(), camH = mainCam.height();
		for (auto d : doors)
		{
			if (d)
			{
				Transform* trans = d->GetComponent<Transform>();
				if (trans)
				{
					if (abs(trans->Translation()[0] - camPos.x) > camW / 2 || abs(trans->Translation()[1] - camPos.y) > camH / 2)
					{
						continue;

					}
					else
					{
						Sprite* check = d->GetComponent<Sprite>();
						if (check)
						{
							d->RemoveComponent<Sprite>();
							d->RemoveComponent<Collider>();
							d->RemoveComponent<DebugDraw>();
							MultiEmitter* me = d->GetComponent<MultiEmitter>();
							if (me)
							{
								auto e = me->getEmitters();
								e->at(0)->setAlpha(.335f, 0.0f);
								e->at(1)->setAlpha(.092f, 0.14f);
								e->at(2)->setAlpha(1.0f, 0.0f);
								int xdiff = mainCam.center().x - trans->Translation()[0];
								int ydiff = mainCam.center().y - trans->Translation()[1];
								float angle = atanf(xdiff / ydiff);
								if (xdiff < 0)
									angle += M_PI;
								e->at(0)->setAngle(angle, e->at(0)->getAngle()[1]);
								e->at(1)->setAngle(angle, e->at(1)->getAngle()[1]);
								e->at(2)->setAngle(angle, e->at(2)->getAngle()[1]);
								e->at(0)->Init(e->at(0)->getParticleCount(), e->at(0)->getLifetime());
								e->at(1)->Init(e->at(1)->getParticleCount(), e->at(1)->getLifetime());
								e->at(2)->Init(e->at(2)->getParticleCount(), e->at(2)->getLifetime());
								e->at(0)->setDoLoop(false);
								e->at(1)->setDoLoop(false);
								e->at(2)->setDoLoop(false);
							}
						}
						
					}
				}
			}
		}
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Open_Door"));
	}
}
void DoorManager::WorldEvents(std::string* we)
{
	if (strncmp(we->c_str(), "reset", 6) == 0)
	{
		doors.clear();
	}
}
void DoorManager::Clear()
{
	doors.clear();
}



namespace PzManage
{
	PuzzleManager * pzm = nullptr;
	DoorManager * dorm = nullptr;
	void Init()
	{
		pzm = new PuzzleManager();
		dorm = new DoorManager();
	}

	void Update(float dt)
	{
		pzm->Update(dt);
	}

	void Shutdown()
	{
		delete pzm;
		delete dorm;
	}
}

#endif // R_FILE