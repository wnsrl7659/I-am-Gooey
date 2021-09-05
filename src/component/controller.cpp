/******************************************************************************/
/*!
\file   controller.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Controller, Player Controller, Camera Controller components

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "controller.h"
#include "../graphics/Sprite.h"
#include "../gameobject/gameobject.h"
#include <iostream>
#include "../messaging/ControllerEvent.h"
#include "../messaging/messaging.h"
#include "../messaging/messagehandler.h"
#include "../messaging/InputEvent.h"
#include "../animation/animation.h"
#include "../audio/audiomanager.h"
#include "../graphics/Camera.h"
#include "../settings/settings.h"
#include "../physics/Transform.h"
#include "../physics/Physics.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../graphics/DebugDraw.h"
#include "collfix.h"
#include "collider.h"
#include "../spaces/spacemanager.h"
#include "../trace/trace.h"
#include <glm\glm.hpp>
#include "../system/Window.h"
#include "../system/heatmap.h"
#include "../vfx/LightManager.h"
#include "../spaces/spacemanager.h"
#include "../system/TrueType.h"
#include "../system/Window.h"
#include "../graphics/Fade.h"
#include <filesystem>
#include <chrono>

MovementEvent nextmove;
MovementEvent pastmove = none;
MovementEvent move;
MovementEvent firstmove = none;
MovementEvent secondmove = none;

static int firstsecondcheck = 0;

static int captureflag = 0;
static bool tempculling = false;
static float temppos[2];
static float lightTimer = 1.f;

static glm::vec2 plresetpos = glm::vec2(1760,-2672);
static bool plresetcheck = false;
static bool plresetmove = false;

bool hitbywall = false;
bool trailswitch = false;
bool trailresetswitch = false;
bool trailbook = false;
int trailresetcount = 0;
glm::vec2 playerpos = glm::vec2(0.0f, 0.0f);

static bool plcontroller = false;
static bool isMoving;
static bool isAuto;

static int dynamicmove = 1;
static int dynamicswitch = 0;

bool pausecheck = false;

static int txid = 0;
static int tyid = 0;

/**************************************************************************/
/*!
\brief Controller Destructor
*/
/**************************************************************************/
Controller::~Controller()
{

}

/**************************************************************************/
/*!
\brief Controller Update
\param dt
	delta time passed into all updates
*/
/**************************************************************************/
void Controller::Update(float dt)
{

}

/**************************************************************************/
/*!
\brief Controller Move
*/
/**************************************************************************/
void Controller::Move()
{

}

/**************************************************************************/
/*!
\brief Controller Draw
*/
/**************************************************************************/
void Controller::Draw() const
{

}

/**************************************************************************/
/*!
\brief Player Controller Constructor, attaches a message handler, requires animation
*/
/**************************************************************************/
PlayerController::PlayerController() : Controller()
{
	Messaging::messHandler->Attach<PlayerController, InputEvent, &PlayerController::Move>(this, "InputEvent");
	move = none;
	idle = 3;
	Animation * anim = dynamic_cast<Animation *>(owner->GetComponent<Animation>());
	frameTimer = anim->GetFrameTimer();
	isMoving = false;
	isAuto = false;


	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (trailmap[i][j].status != 0)
				trailmap[i][j].status = 0;
		}
	}
	//trailswitch = false;
	pastmove = none;
	plcontroller = true;
}

/**************************************************************************/
/*!
\brief Player Controller Destructor
*/
/**************************************************************************/
PlayerController::~PlayerController()
{
	Messaging::messHandler->Detach<PlayerController>(this, "InputEvent");
	plcontroller = false;
}

/**************************************************************************/
/*!
\brief Update Player Controller, moving the player based on input received in the Move function
\param dt
	delta time because its an update function
*/
/**************************************************************************/
void PlayerController::Update(float dt)
{
	if ((lightTimer -= dt) < 0)
	{
		lightTimer = 0.5f;
		Vector2D pos = (owner->GetComponent<Transform>())->Translation();

		float r = .2f, g = .0f, b = .0f;

		LightSource* LS = lightManager->AddLight("Heat", true, r, g, b, 64.f * 3);
		LS->X(pos[0]);
		LS->Y(pos[1]);
		LS->Z(16);
	}
	if (captureflag == 1)
	{
		std::string apd = Trace::GetAppDataLocalLow();
		std::stringstream ss;
		ss << apd;
		ss << "\\DigiPen";
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		ss << "\\SongOfLife";
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		ss << "\\Heatmaps";
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		ss << "\\" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		ss << "\\screen.tga";
		MakeScreenshot(ss.str());
		captureflag = 2;
	}
	else if (captureflag == 2)
	{
		Settings::culling = tempculling;
		mainCam.SetZoom(1.0f);
		mainCam.SetPosition(temppos[0], temppos[1]);

		lightManager->MakeLightHidden("Heat");
		captureflag = 0;
	}
	Sprite * sprite = dynamic_cast<Sprite *>(owner->GetComponent<Sprite>());
	Animation * anim = dynamic_cast<Animation *>(owner->GetComponent<Animation>());
	Transform * transform = dynamic_cast<Transform *>(owner->GetComponent<Transform>());
	Physics * physics = dynamic_cast<Physics *>(owner->GetComponent<Physics>());
	Collider * collider = dynamic_cast<Collider *>(owner->GetComponent<Collider>());
	Vector2D vel;

	if (plresetmove == true)
	{
		transform->Translation(Vector2D(plresetpos[0], plresetpos[1]));
		plresetmove = false;
	}

	Vector2D newTransform(transform->Translation());
	newTransform[0] = (float)((int)(newTransform[0]) % 16);
	newTransform[1] = (float)((int)(newTransform[1]) % 16);
	static bool first = true;
	static int dimX, dimY;
	dimX = (int)((transform->Translation()[0] + 864.0f) / 1728.0f);
	dimY = (int)((transform->Translation()[1] - 560.0f) / 1088.0f);
	frameTimer -= dt;

	// set old transform to current position
	transform->OldRotation(transform->Rotation());
	transform->OldScale(transform->Scale());
	transform->OldTranslation(transform->Translation());

	// playerpos
	playerpos[0] = transform->Translation()[0];
	playerpos[1] = transform->Translation()[1];

	
	if (checkPM((float)dimX, (float)dimY) && pmflag)
	{
		switch (move)
		{
		case up:
			vel[1] = Physics::s_PlayerVelocity;
			physics->Velocity(vel);
			anim->setSequence(2);
			idle = 4;
			break;
		case down:
			vel[1] = -Physics::s_PlayerVelocity;
			physics->Velocity(vel);
			anim->setSequence(1);
			idle = 3;
			break;
		case left:
			vel[0] = -Physics::s_PlayerVelocity;
			physics->Velocity(vel);
			anim->setSequence(0);
			idle = 5;
			break;
		case right:
			vel[0] = Physics::s_PlayerVelocity;
			physics->Velocity(vel);
			anim->setSequence(7);
			idle = 6;
			break;
		case none:
			anim->setSequence(idle);
			//transform->Translation(transform->Translation() - newTransform);
			physics->Velocity(Vector2D());
			break;
		}
	}
	else
	{
		physics->Velocity(Vector2D());
	}

	if (firstsecondcheck == 1)
	{
		if (nextmove != none)
		{
			switch (nextmove)
			{
			case up:
				if (wallmap[txid][tyid - 1] == 0)
				{
					secondmove = nextmove;
					firstsecondcheck = 2;
				}
				break;
			case down:
				if (wallmap[txid][tyid + 1] == 0)
				{
					secondmove = nextmove;
					firstsecondcheck = 2;
				}
				break;
			case left:
				if (wallmap[txid - 1][tyid] == 0)
				{
					secondmove = nextmove;
					firstsecondcheck = 2;
				}
				break;
			case right:
				if (wallmap[txid + 1][tyid] == 0)
				{
					secondmove = nextmove;
					firstsecondcheck = 2;
				}
				break;
			}
		}
	}

	if ((int)abs(transform->Translation()[0] - 32 + vel[0]) % 64 == 0 && 
		(int)abs(transform->Translation()[1] + 48 + vel[1]) % 64 == 0)
	{
		int xid = (int)((transform->Translation()[0] + 864.0f + 448.0f + vel[0]) / 64.0f);
		int yid = abs((int)((transform->Translation()[1] - 560.0f - 320.0f + 32.f + vel[1]) / 64.0f));
		txid = xid;
		tyid = yid;
		if (plresetcheck == true)
		{
			plresetpos[0] = transform->Translation()[0] + vel[0];
			plresetpos[1] = transform->Translation()[1] + vel[1];

			plresetcheck = false;
		}
		
		if (bowlmap[xid][yid] == 1)
		{
			if (firstsecondcheck == 0)
			{
				if (move != none)
				{
					firstmove = move;
					if (nextmove != none)
					{
						switch (nextmove)
						{
						case up:
							if (wallmap[txid][tyid - 1] == 0)
							{
								secondmove = nextmove;
								firstsecondcheck = 2;
							}
							else
							{
								firstsecondcheck = 1;
							}
							break;
						case down:
							if (wallmap[txid][tyid + 1] == 0)
							{
								secondmove = nextmove;
								firstsecondcheck = 2;
							}
							else
							{
								firstsecondcheck = 1;
							}
							break;
						case left:
							if (wallmap[txid - 1][tyid] == 0)
							{
								secondmove = nextmove;
								firstsecondcheck = 2;
							}
							else
							{
								firstsecondcheck = 1;
							}
							break;
						case right:
							if (wallmap[txid + 1][tyid] == 0)
							{
								secondmove = nextmove;
								firstsecondcheck = 2;
							}
							else
							{
								firstsecondcheck = 1;
							}
							break;
						}
					}
					else
					{
						firstsecondcheck = 1;
					}

				}

			}
		}
		if (bowlmap[xid][yid] == 0)
		{
			if (trailbook == true)
			{
				trailswitch = true;
				trailbook = false;
			}
			firstsecondcheck = 0;
			firstmove = none;
			secondmove = none;
		}


		if (trailswitch == true)
		{
			if (hitbywall == false)
			{
				int trailtemp = 0;
				switch (move)
				{
				case up:
					if (trailmap[xid][yid + 1].status == 0)
					{
						switch (pastmove)
						{
						case up:
							trailmap[xid][yid + 1].status = 10;
							break;
						case down:
							//collision with trail
							break;
						case left:
							trailmap[xid][yid + 1].status = 8;
							break;
						case right:
							trailmap[xid][yid + 1].status = 9;
							break;
						case none:
							trailmap[xid][yid + 1].status = 2;
							break;
						}
					}
					break;
				case down:
					if (trailmap[xid][yid - 1].status == 0)
					{
						switch (pastmove)
						{
						case up:
							//collision with trail
							break;
						case down:
							trailmap[xid][yid - 1].status = 10;
							break;
						case left:
							trailmap[xid][yid - 1].status = 3;
							break;
						case right:
							trailmap[xid][yid - 1].status = 4;
							break;
						case none:
							trailmap[xid][yid - 1].status = 1;
							break;
						}
					}
					break;
				case left:
					if (trailmap[xid + 1][yid].status == 0)
					{
						switch (pastmove)
						{
						case up:
							trailmap[xid + 1][yid].status = 4;
							break;
						case down:
							trailmap[xid + 1][yid].status = 9;
							break;
						case left:
							trailmap[xid + 1][yid].status = 5;
							break;
						case right:
							//collision with trail
							break;
						case none:
							trailmap[xid + 1][yid].status = 7;
							break;
						}
					}
					break;
				case right:
					if (trailmap[xid - 1][yid].status == 0)
					{
						switch (pastmove)
						{
						case up:
							trailmap[xid - 1][yid].status = 3;
							break;
						case down:
							trailmap[xid - 1][yid].status = 8;
							break;
						case left:
							//collision with trail
							break;
						case right:
							trailmap[xid - 1][yid].status = 5;
							break;
						case none:
							trailmap[xid - 1][yid].status = 6;
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			else
			{
				hitbywall = false;
				move = none;
			}
			/*
			if (move != none)
			{
				pastmove = move;
			}
			*/

			if (trailmap[xid][yid].status != 0)
			{
				PzManage::pzm->ResetCurrentPuzzle();
				trailReset();
				playerResetPos();
				trailresetswitch = true;
			}
		}

		if (move != none)
		{
			pastmove = move;
		}

		if (nextmove != none)
		{
			move = nextmove;
		}
		else
		{
			isAuto = false;
			move = none;
			isMoving = false;
		}

		sprite->SetSize(64, 64);
		if (dynamicswitch == 1)
		{
			if (dynamicmove == 1)
			{
				dynamicmove = 2;
			}
			else
			{
				dynamicmove = 1;
			}
			dynamicswitch = 0;
		}
	}
	else
	{
		//sprite->SetSize(64, 64);
		dynamicswitch = 1;

		if (move == up || move == down)
		{
			int diff = (int)abs(transform->Translation()[1] + 48 + vel[1]) % 64;
			diff = diff / 2;
			if (diff < 16)
			{
				if (dynamicmove == 1)
				{
					sprite->SetSize((float)(64 + diff), (float)(64 - diff / 2));
				}
				else 
				{
					sprite->SetSize((float)(64 - diff), (float)(64 + diff / 2));
				}
			}
			else
			{
				if (dynamicmove == 1)
				{
					sprite->SetSize((float)(96 - diff), (float)(48 + diff / 2));
				}
				else
				{
					sprite->SetSize((float)(32 + diff), (float)(70 - diff / 2));
				}
			}
		}
		else
		{
			int diff = (int)abs(transform->Translation()[0] - 32 + vel[0]) % 64;
			diff = diff / 2;
			if (diff < 16)
			{
				if (dynamicmove == 1)
				{
					sprite->SetSize((float)(64 + diff), (float)(64 - diff / 2));
				}
				else
				{
					sprite->SetSize((float)(64 - diff), (float)(64 + diff / 2));
				}
			}
			else
			{
				if (dynamicmove == 1)
				{
					sprite->SetSize((float)(96 - diff), (float)(48 + diff / 2));
				}
				else
				{
					sprite->SetSize((float)(32 + diff), (float)(70 - diff / 2));
				}
			}
		}
		
	}
	
	if (move != none)
	{
		if (frameTimer <= -anim->GetAnimationSpeed())
		{
			frameTimer = anim->GetAnimationSpeed();
			Messaging::messHandler->HandleMessage("audio event", &std::string("play Moving"));
		}
	}

	//first initialize
	/*
	static int dimX, dimY;
	dimX = (int)((transform->Translation()[0] + 864.0f) / 1728.0f);
	dimY = (int)((transform->Translation()[1] - 560.0f) / 1088.0f);
	if (first)
	{
		mainCam.moveRight(((float)dimX*1728.0f) - mainCam.center().x);
		mainCam.moveUp(((float)dimY*1088.0f) - mainCam.center().y);
		first = false;
	}
	*/
	
	float tempx = mainCam.GetPosition()[0];
	float tempy = mainCam.GetPosition()[1];

	camScroll((float)dimX, (float)dimY);
	
	if (tempx != mainCam.GetPosition()[0] || tempy != mainCam.GetPosition()[1])
	{
		plresetcheck = true;
	}
}

/**************************************************************************/
/*!
\brief Player Controller Draw
*/
/**************************************************************************/
void PlayerController::Draw() const
{

}

/**************************************************************************/
/*!
\brief Changes the move enum based on Input
\param input
	Input Event to move player
*/
/**************************************************************************/
void PlayerController::Move(InputEvent * input)
{
	//if (Spaces::spaceManager->GetSimilarSpace("ui"))
	//{
	//	nextmove = none;
	//	isMoving = false;
	//	return;
	//}
	if (input->action == "keydown")
	{
		if (input->key == "1")
		{
			DebugDraw* debugdraw = owner->GetComponent<DebugDraw>();
			debugdraw->SetMode(0);
		}
		if (input->key == "2")
		{
			DebugDraw* debugdraw = owner->GetComponent<DebugDraw>();
			debugdraw->SetMode(1);
		}
		if (input->key == "3")
		{
			DebugDraw* debugdraw = owner->GetComponent<DebugDraw>();
			debugdraw->SetMode(2);
		}
		//if (input->key == "w" || input->key == "up")
		//{
		//	Trace::Message("Move Up");
		//	if (isAuto == false)
		//	{
		//		move = up;
		//		nextmove = up;
		//		isMoving = true;
		//		isAuto = true;
		//	}
		//	else
		//	{
		//		nextmove = up;
		//	}
		//}
		//if (input->key == "s" || input->key == "down")
		//{
		//	Trace::Message("Move Down");
		//	if (isAuto == false)
		//	{
		//		move = down;
		//		nextmove = down;
		//		isMoving = true;
		//		isAuto = true;
		//	}
		//	else
		//	{
		//		nextmove = down;
		//	}
		//}
		//if (input->key == "d" || input->key == "right")
		//{
		//	Trace::Message("Move Right");
		//	if (isAuto == false)
		//	{
		//		move = right;
		//		nextmove = right;
		//		isMoving = true;
		//		isAuto = true;
		//	}
		//	else
		//	{
		//		nextmove = right;
		//	}
		//}
		//if (input->key == "a" || input->key == "left")
		//{
		//	Trace::Message("Move Left");
		//	if (isAuto == false)
		//	{
		//		move = left;
		//		nextmove = left;
		//		isMoving = true;
		//		isAuto = true;
		//	}
		//	else
		//	{
		//		nextmove = left;
		//	}
		//}

		//TEMPORARY FUNCTION WILL PROBABLY BE MOVED PLS DONUT TOUCH
		//IF YOU PRESS p THEN IT MAKES AN EMPTY JSON FILE SO THATS PRETTY NEAT
		if (input->key == "p")
		{
			//Serial::SerializeEmpty();
			if (captureflag == 0)
			{
				tempculling = Settings::culling;
				Settings::culling = false;
				Settings::debug = false;
				mainCam.SetZoom(5.0f);
				temppos[0] = mainCam.GetPosition()[0];
				temppos[1] = mainCam.GetPosition()[1];
				mainCam.SetPosition(1728.0, -1088);
				captureflag = 1;
				lightManager->MakeLightVisible("Heat");
			}
		}
		if (input->key == "o")
		{
			//Serial::SaveSerialize(owner);
		}
	}
	if (input->action == "keyup")
	{
		//if ((input->key == "w" || input->key == "up") && move == up)
		//{
		//	if(nextmove == up)
		//		nextmove = none;
		//	if (isAuto == false)
		//	{
		//		move = none;
		//	}
		//}
		//if ((input->key == "s" || input->key == "down") && move == down)
		//{
		//	if (nextmove == down)
		//		nextmove = none;
		//	if (isAuto == false)
		//	{
		//		move = none;
		//	}
		//}
		//if ((input->key == "a" || input->key == "left") && move == left)
		//{
		//	if (nextmove == left)
		//		nextmove = none;
		//	if (isAuto == false)
		//	{
		//		move = none;
		//	}
		//}
		//if ((input->key == "d" || input->key == "right") && move == right)
		//{
		//	if (nextmove == right)
		//		nextmove = none;
		//	if (isAuto == false)
		//	{
		//		move = none;
		//	}
		//}

		/*if (input->key == "j")
		{
			if (tempflag == 2)
			{
				tempflag = 3;
			}
		}
		if (input->key == "k")
		{
			if (tempflag == 3)
			{
				tempflag = 4;
			}
		}
		if (input->key == "l")
		{
			if (tempflag == 4)
			{
				tempflag = 5;
			}
		}
		if (input->key == ";")
		{
			if (tempflag == 5)
			{
				tempflag = 6;
			}
		}*/
	}
	/*
	if (move == none)
	{
		isMoving = false;
	}
	*/
}



/**************************************************************************/
/*!
\brief Cam Controller constructor
*/
/**************************************************************************/
CameraController::CameraController() : Controller()
{
}

/**************************************************************************/
/*!
\brief Cam controller update
\param dt
	delta time
*/
/**************************************************************************/
void CameraController::Update(float dt)
{

}

/**************************************************************************/
/*!
\brief Cam Controller Draw
*/
/**************************************************************************/
void CameraController::Draw() const
{

}

/**************************************************************************/
/*!
\brief Cam Controller Move
*/
/**************************************************************************/
void CameraController::Move()
{

}


void playerResetPos()
{
	plresetmove = true;
}
void trailReset()
{
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (trailmap[i][j].status != 0)
			{
				trailmap[i][j].status = 0;
				for (int k = 0; k < trailmap[i][j].count; k++)
				{
					if (trailmap[i][j].sprite[k])
					{
						trailmap[i][j].sprite[k]->ResetSequence();
					}

				}
			}
		}
	}
	trailswitch = false;
	trailbook = false;
	pastmove = none;
	//Spaces::spaceManager->MarkForReset();
}

void plhandleInput(int key, int action, GLFWwindow* window)
{
	if (plcontroller && quitconfirmflag == 0 && pausecheck == false && IsFadeWorking() == false)
	{

		if (Spaces::spaceManager->GetSimilarSpace("ui"))
		{
			nextmove = none;
			isMoving = false;
			return;
		}

		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_W)
			{
				Trace::Message("Move Up");
				if (isAuto == false)
				{
					move = up;
					nextmove = up;
					isMoving = true;
					isAuto = true;
				}
				else
				{
					nextmove = up;
				}
			}
			if (key == GLFW_KEY_S)
			{
				Trace::Message("Move Down");
				if (isAuto == false)
				{
					move = down;
					nextmove = down;
					isMoving = true;
					isAuto = true;
				}
				else
				{
					nextmove = down;
				}
			}
			if (key == GLFW_KEY_A)
			{
				Trace::Message("Move Left");
				if (isAuto == false)
				{
					move = left;
					nextmove = left;
					isMoving = true;
					isAuto = true;
				}
				else
				{
					nextmove = left;
				}
			}
			if (key == GLFW_KEY_D)
			{
				Trace::Message("Move Right");
				if (isAuto == false)
				{
					move = right;
					nextmove = right;
					isMoving = true;
					isAuto = true;
				}
				else
				{
					nextmove = right;
				}
			}
		}
		if (action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_W)
			{
				if (nextmove == up)
					nextmove = none;
				if (isAuto == false)
				{
					move = none;
				}
			}
			if (key == GLFW_KEY_S)
			{
				if (nextmove == down)
					nextmove = none;
				if (isAuto == false)
				{
					move = none;
				}
			}
			if (key == GLFW_KEY_A)
			{
				if (nextmove == left)
					nextmove = none;
				if (isAuto == false)
				{
					move = none;
				}
			}
			if (key == GLFW_KEY_D)
			{
				if (nextmove == right)
					nextmove = none;
				if (isAuto == false)
				{
					move = none;
				}
			}
		}
		if (move == none)
		{
			isMoving = false;
		}
	}
}