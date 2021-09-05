/******************************************************************************/
/*!
\file   spacemanager.cpp
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
Spacemanager class, handles all creation and deletion of spaces

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#define MENU_FADE_TIME 1500

#include <iostream>
#include <Windows.h>

#include "../system/common_defines.h"
#include "spacemanager.h"
#include "../messaging/messaging.h"
#include "../settings/settings.h"
#include "../graphics/DebugWindow.h"
#include "../audio/audiomanager.h"
#include "../system/Engine.h"
#include "../system/timermanager.h"
#include "../vfx/LightManager.h"
#include "../system/TrueType.h"
#include "../puzzles/newpuzzle.h"
#include "../graphics/Fade.h"
#include "../component/button.h"
#include "../system/countdowntimer.h"
#include "../component/controller.h"
#include "../gameobject/GameObjectSerialize.h"
#include "../utils/utils.h"
#include "../system/Window.h"

// TODO
// Have all of the spaces become json files, that when we create the spaces we can just parse data from the 
// file instead of having it all coded in like shit

static float digialpha = 0.0f;
static bool digiflag = true;

SpaceManager::SpaceManager()
{
	Messaging::messHandler->Attach<SpaceManager, InputEvent, &SpaceManager::SpaceInputHandler>(this, "InputEvent");
	Messaging::messHandler->Attach<SpaceManager, ButtonEvent, &SpaceManager::SpaceButtonHandler>(this, "ButtonEvent");

	spaces.max_load_factor(0.1f);
}

SpaceManager::~SpaceManager()
{
	std::vector<std::string> names;
	for (auto sp : spaces)
	{
		names.push_back(const_cast<char *>(sp.first.c_str()));
	}
	for (auto nm : names)
	{
		RemoveSpace(nm);
	}
	spaces.clear();
	Messaging::messHandler->Detach<SpaceManager>(this, "InputEvent");
	Messaging::messHandler->Detach<SpaceManager>(this, "ButtonEvent");
	delete splashTimer;
	delete creditsTimer;
}

Space * SpaceManager::AddSpace(const std::string& name)
{
	Space * sp = reinterpret_cast<Space*>(new char[sizeof(Space)]);
	sp = new(sp)Space;
	spaces[name] = sp;
	Messaging::messHandler->HandleMessage("space event", &std::string("created " + name));
	sp->name = name;
	Trace::Message(std::string("Creating space: " + name));
	return sp;
}

void SpaceManager::RemoveSpace(const std::string& name)
{
	if (SpacesContains(name))
	{
		Messaging::messHandler->HandleMessage("space event", &std::string("destroyed " + name));
		Trace::Message(std::string("Destroying space: " + name));
		Space * s = GetSimilarSpace(name);
		if (s)
		{
			delete s;
			spaces.erase(name);
		}
	}
}

/**************************************************************************/
	/*!
	\brief Marks a space to be removed, rather than immediately removing
	  which could invalidate iterators
	\param name
	  Name of the space to be removed
	*/
	/**************************************************************************/
void SpaceManager::MarkSpaceForRemoval(const std::string& name)
{
	if (SpacesContains(name))
	{
		Trace::Message(std::string("Marking " + name + " for removal"));

		for (auto s : spaces)
		{
			if (s.first.Has(name))
			{
				s.second->removing_ = true;
			}
		}

		spacesNeedingRemoval_ = true;
	}
}

/**************************************************************************/
	/*!
	\brief Gets a space, by name, from the spaces map
	\param name
	  Name of the space to get
	\return The desired space if it exists, else returns nullptr
	*/
	/**************************************************************************/
Space * SpaceManager::GetSpace(const std::string& name)
{
	if (SpacesContains(name))
	{
		return spaces[name];
	}
	else return nullptr;
}

/**************************************************************************/
	/*!
	\brief Gets a space with a "similar" name, meaning the spaces name
	  contains the name given to the function
	\param name
	  The name that must be contained in the space's name
	\return The first occurence of a space with a name that contains
	  the passed name
	*/
	/**************************************************************************/
Space * SpaceManager::GetSimilarSpace(const std::string& name)
{
	// linear complexity
	for (auto sp : spaces)
	{
		if (sp.first.Has(name) && sp.second)
		{
			return sp.second;
		}
	}
	return nullptr;
}

void SpaceManager::SetAllAlpha(const std::string& name, float a)
{
	Space * s = GetSimilarSpace(name);
	if (s)
	{
		s->SetAllAlpha(a);
	}
}

void SpaceManager::MarkAllSpacesForRemoval()
{
	for (auto s : spaces)
	{
		s.second->removing_ = true;
	}
}

void SpaceManager::RemoveAllSpaces()
{
	std::vector<std::string> names;
	for (auto s : spaces)
	{
		names.push_back(std::string(const_cast<char *>(s.first.c_str())));
	}
	for (auto nm : names)
	{
		RemoveSpace(nm);
	}
	names.clear();
}

void SpaceManager::ExecuteNextFrame(std::function<void(void)> f)
{
	func_queue.push(f);
}

/**************************************************************************/
	/*!
	\brief Removes any spaces marked for removal and updates the SpaceManager.
	\param dt
	  time since last frame
	*/
	/**************************************************************************/
void SpaceManager::Update(float dt)
{
	if (currentScreen_ == Spaces::Screens::Splash && digialpha < 1.0f && digiflag == true)
	{
		digialpha += 0.01f;
		Sprite* digitemps = (Sprite*)digisprite;
		digitemps->setAlpha(digialpha);
		if (digialpha >= 1.0f)
		{
			CountdownTimer* digitempc = (CountdownTimer*)digitimer;
			digitempc->Start();
			digiflag = false;
		}
	}
	else
	{

	}

	// run the func queue
	while (!func_queue.empty())
	{
		func_queue.front()();
		func_queue.pop();
	}
	// update the event queue
	while (!event_queue.empty())
	{
		event_queue.front()(); // call the first event in the queue
		event_queue.pop();
	}

	if (startGame_)
	{
		startGame_ = false;
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Music_Title"));
		QueueClearEvent();
		QueueBuildEvent(Spaces::BuildLevel1);
	}
	if (returnToMenu_)
	{
		returnToMenu_ = false;
		//Data::Shutdown();
		//Data::Init();
		PzManage::Shutdown();
		PzManage::Init();
		Audio::EventData * data = new Audio::EventData();
		data->eventName = "Music_Title";
		data->paramName = "First Play";
		data->val = 0.0f;
		Audio::EditParam(data);
		delete data;
		RemoveAllSpaces();
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Main_Menu"));
		Spaces::BuildMainMenu();
	}
	if (win_)
	{
		win_ = false;
		RemoveAllSpaces();
		Spaces::BuildWinSpace();
	}
	if (reset_)
	{
		ResetSpace();
	}
	// remove the marked spaces
	if (spacesNeedingRemoval_)
	{
		std::vector<std::string> names;
		// gather names to avoid pointer/iterator invalidation
		for (auto sp : spaces)
		{
			if (sp.second->removing_)
			{
				names.push_back(*(const_cast<std::string *>(&sp.second->name)));
			}
		}
		for (const std::string s : names)
		{
			RemoveSpace(s);
		}
		names.clear();
		spacesNeedingRemoval_ = false;
	}

	for (auto sp : spaces)
	{
		sp.second->Update(dt);
	}

	if (currentScreen_ == Spaces::Screens::Credits || currentScreen_ == Spaces::Screens::MainMenu || currentScreen_ == Spaces::Screens::Splash)
	{
		Settings::culling = false;
	}
}

void SpaceManager::QueueClearEvent()
{
	SpaceEvent e(SpaceEvent::Type::Clear);
	event_queue.push(e);
}

void SpaceManager::QueueBuildEvent(std::function<void(void)> f)
{
	SpaceEvent e(SpaceEvent::Type::Build, f);
	event_queue.push(e);
}

/**************************************************************************/
	/*!
	\brief Draws all spaces
	*/
	/**************************************************************************/
void SpaceManager::Draw()
{
	for (auto sp : spaces)
	{
		sp.second->Draw();
	}
	if (Settings::graphicDebug)
	{
		DebugWindow::drawSpaceWindow(spaces, *this);
		DebugWindow::drawFPSWindow();
		DebugWindow::drawWindows();
	}
}

/**************************************************************************/
	/*!
	\brief Gets the map of spaces
	\return Pointer to the map of spaces
	*/
	/**************************************************************************/
std::unordered_map<std::string, Space *> * SpaceManager::GetSpaces()
{
	return &spaces;
}

/**************************************************************************/
	/*!
	\brief Handles input events
	\param data
	  Data on the input event
	*/
	/**************************************************************************/
void SpaceManager::SpaceInputHandler(InputEvent * data)
{
	if (data->action == "keydown")
	{
		if (quitconfirmflag == 1 || menuconfirmflag == 1)
		{
			if (data->key == "a" || data->key == "lleft")
			{
				yesorno = 1;
			}
			if (data->key == "d" || data->key == "rright")
			{
				yesorno = 0;
			}
			if (data->key == "y")
			{
				ButtonEvent what(noEvent);
				if (quitconfirmflag == 1)
				{
					quitconfirmflag = 0;
					what.eventType = quit;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuconfirmflag == 1)
				{
					menuconfirmflag = 0;
					yesorno = 0;
					what.eventType = menu;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
			}
			else if (data->key == "n")
			{
				quitconfirmflag = 0;
				menuconfirmflag = 0;
				data->key = "";
			}
		}
	}

	if (data->key == "F11" && data->action == "keydown")
	{
		ToggleFullScreen();
	}
	else if (data->key == "x" && data->action == "keydown")
	{
		Settings::graphicDebug = !Settings::graphicDebug;
		ShowCursor(Settings::graphicDebug);
	}
	else if (data->key == "n" && data->action == "keyup")
	{
		if (quitconfirmflag == 0)
		{
			if (currentScreen_ == Spaces::Screens::Game)
			{
				LoadNextLevel();
			}
		}
	}
	else if (data->key == "r" && data->action == "keydown")
	{
		if (quitconfirmflag == 0)
		{
			if (currentScreen_ != Spaces::Screens::Splash && currentScreen_ != Spaces::Screens::Credits && currentScreen_ != Spaces::Screens::Win && currentScreen_ != Spaces::Screens::MainMenu && currentScreen_ != Spaces::Screens::PauseMenu)
			{
				Audio::aManager->Shutdown();
				Audio::aManager->Init();
				QueueClearEvent();
				QueueBuildEvent(Spaces::level_loaders[c_level]);
			}
		}
	}
	else if (data->key == "/" && data->action == "keyup")
	{
		if (quitconfirmflag == 0 && menuconfirmflag == 0)
		{
			c_level = Spaces::Levels::WinState;
			QueueClearEvent();
			QueueBuildEvent(Spaces::level_loaders[c_level]);
			Audio::aManager->StopAllEvents();
		}
	}
	else if (data->key == "esc" && data->action == "keydown")
	{
		if (currentScreen_ == Spaces::Screens::Game || currentScreen_ == Spaces::Screens::PauseMenu)
		{
			if (quitconfirmflag != 1 && menuconfirmflag != 1)
			{
				for (auto i : spaces)
				{
					if (i.second->name != "777 pausespace")
					{
						if (i.second->Active())
						{
							i.second->Pause();
						}
						else if (!i.second->Active())
						{
							i.second->Unpause();
						}
					}
					if (i.second->name == "777 pausespace")
					{
						if (i.second->Active())
						{
							i.second->Pause();
							// set screen to pause
							currentScreen_ = Spaces::Screens::PauseMenu;
							pausecheck = false;
						}
						else if (!i.second->Active())
						{
							i.second->Unpause();
							currentScreen_ = Spaces::Screens::Game;
							pausecheck = true;
						}
					}
				}
			}
			if (currentScreen_ == Spaces::Screens::Game)
			{
				currentScreen_ = Spaces::Screens::PauseMenu;
				Audio::aManager->SetSnapshot("Pause");
				//Audio::PauseAll(nullptr);
				//CountdownTimer * tm = TimerManager::NewCountdownTimer();
				//tm->SetLimitMilliseconds(500);
				//FreeFunctionWrapper<void, void> * func = new FreeFunctionWrapper<void, void>(Audio::PauseAll, nullptr);
				//tm->SetTrigger(func);
				//tm->Start();
				//TimerManager::AddTimer(tm);
			}
			else if (currentScreen_ == Spaces::Screens::PauseMenu)
			{
				currentScreen_ = Spaces::Screens::Game;
				Audio::aManager->SetSnapshot("UnPause");
				//Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("unpause all"));
				//CountdownTimer * tm = TimerManager::NewCountdownTimer();
				//tm->SetLimitMilliseconds(MENU_FADE_TIME);
				//FreeFunctionWrapper<void, void> * func = new FreeFunctionWrapper<void, void>(Audio::ReturnToLevelSnap, nullptr);
				//tm->SetTrigger(func);
				//tm->Start();
				//TimerManager::AddTimer(tm);
			}
		}
	}
	else if (data->key == "enter" && data->action == "keyup")
	{
		if (quitconfirmflag == 1 || menuconfirmflag == 1)
		{
			if (yesorno == 1)
			{
				ButtonEvent what(noEvent);
				if (quitconfirmflag == 1)
				{
					quitconfirmflag = 0;
					what.eventType = quit;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuconfirmflag == 1)
				{
					menuconfirmflag = 0;
					yesorno = 0;
					what.eventType = menu;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
			}
			else
			{
				quitconfirmflag = 0;
				menuconfirmflag = 0;
			}
			data->key = "";
		}
	}
}

void SpaceManager::StartGameFromMenu()
{
	startGame_ = true;
}

void SpaceManager::StartMenuFromGame()
{
	returnToMenu_ = true;
}

void SpaceManager::WinGameFromGame()
{
	win_ = true;
}

//added to handle button events from the menu cause creating input events crashed game and cant attach multiple message handlers to the same function
void SpaceManager::SpaceButtonHandler(ButtonEvent * data)
{
	if (spaces.find("4 ButtonSpace") != spaces.end())
	{
		if (data->eventType == play)
		{
			LoadNextLevel();
			//StartGameFromMenu();
		}
	}
	// FIX THAT
	if (data->eventType == restart)
	{
		pausecheck = false;
		
		std::vector<std::string> names; // name of each space to avoid iterator invalidation
		for (auto s : spaces)
		{
			names.push_back(s.first);
		}
		for (auto n : names)
		{
			RemoveSpace(n);
		}
		Messaging::messHandler->HandleMessage<std::string>("world event", &std::string("reset"));
		names.clear();
		Spaces::BuildLevel1();
	}
	if (data->eventType == resume)
	{
		Audio::aManager->SetSnapshot("UnPause");

		for (auto i : spaces)
		{
			if (i.second->name != "777 pausespace")
			{
				if (i.second->Active())
				{
					i.second->Pause();
				}
				else if (!i.second->Active())
				{
					i.second->Unpause();
				}
			}
			if (i.second->name == "777 pausespace")
			{
				if (i.second->Active())
				{
					i.second->Pause();
					pausecheck = false;
				}
				else if (!i.second->Active())
				{
					i.second->Unpause();
					pausecheck = true;
				}
			}
		}
		currentScreen_ = Spaces::Screens::Game;
	}
	if (data->eventType == credits)
	{
		QueueClearEvent();
		QueueBuildEvent(Spaces::BuildCreditsSpace);
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Main_Menu"));
	}

	if (data->eventType == menu)
	{
		if (currentScreen_ == Spaces::Screens::Game || currentScreen_ == Spaces::Screens::PauseMenu || currentScreen_ == Spaces::Screens::Win)
		{
			QueueClearEvent();
			QueueBuildEvent(Spaces::BuildMainMenu);
			Audio::aManager->StopAllEvents();
			Audio::aManager->SetSnapshot("Level");
			Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Main_Menu"));
			c_level = Spaces::Levels::initial;
			currentScreen_ = Spaces::Screens::MainMenu;
			pausecheck = false;
		}
	}
	if (data->eventType == tutorial)
	{
		Space * pause = GetSpace("777 pausespace");
		if (pause)
		{
			pause->Pause();
			QueueBuildEvent(Spaces::BuildHowToPlaySpace);
		}
	}
	else if (data->eventType == tutback)
	{
		Space * pause = GetSpace("777 pausespace");
		Space * opt = GetSpace("6090 HowToPlay");
		if (pause && opt)
		{
			pause->Unpause();
			MarkSpaceForRemoval("6090 HowToPlay");
		}
	}
	if (data->eventType == win)
	{
		if (currentScreen_ == Spaces::Screens::Game)
		{
			WinGameFromGame();
		}
	}
	if (data->eventType == options)
	{
		QueueClearEvent();
		QueueBuildEvent(Spaces::BuildOptionMenu);
	}
	else if (data->eventType == optionsback)
	{
		QueueClearEvent();
		QueueBuildEvent(Spaces::BuildMainMenu);
	}
	if (data->eventType == poptions)
	{
		// bulid the options menu without destroying the game at the same time
		// pause the pause menu
		Space * pause = GetSpace("777 pausespace");
		if (pause)
		{
			pause->Pause();
			QueueBuildEvent(Spaces::BuildPoptionSpace);
		}
	}
	else if (data->eventType == poptionsback)
	{
		Space * pause = GetSpace("777 pausespace");
		Space * opt = GetSpace("7777 poptionspace");
		if (pause && opt)
		{
			pause->Unpause();
			MarkSpaceForRemoval("7777 poptionspace");
		}
	}
	if (data->eventType == quit)
	{
		pEngine->Shutdown();
	}
}

void SpaceManager::ResetSpace()
{
	Audio::aManager->Shutdown();
	Audio::aManager->Init();
	lightManager->DeleteAllLights();
	lightManager->DeleteAllWalls();
	TimerManager::Shutdown();
	TimerManager::Init();
	Settings::graphicDebug = false;
	reset_ = false;
	std::vector<std::string> names; // name of each space to avoid iterator invalidation
	for (auto& s : spaces)
	{
		names.push_back(s.first);
	}
	for (auto& n : names)
	{
		RemoveSpace(n);
	}
	Messaging::messHandler->HandleMessage<std::string>("world event", &std::string("reset"));
	names.clear();
	switch (currentScreen_)
	{
	case Spaces::Screens::MainMenu:
		Spaces::BuildMainMenu();
		break;
	case Spaces::Screens::Game:
		Spaces::BuildLevel1();
		break;
	default:
		break;
	}
}

void SpaceManager::LoadNextLevel(void *)
{
	LoadNextLevel();
}

void SpaceManager::LoadNextLevel()
{
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (bowlmap[i][j] != 0)
				bowlmap[i][j] = 0;
		}
	}
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (wallmap[i][j] != 0)
				wallmap[i][j] = 0;
		}
	}
	// if c_level is one less than the level count, we are on the last possible level, go to main menu
	if (c_level == (Spaces::Levels::Count - 1))
	{
		FadeIn(3, empty, this);
		c_level = Spaces::Levels::initial;
	}
	else
	{
		int lvl = c_level;
		++lvl;
		c_level = static_cast<Spaces::Levels>(lvl);
		FadeIn(2, empty, this, lvl);
	}
}

namespace Spaces
{
	SpaceManager * spaceManager = nullptr;

	std::array<LEVEL_LOADER_FUNC, Spaces::Levels::Count> level_loaders =
	{
		BuildLevel1,
		BuildLevel2,
		BuildLevel3,
		BuildLevel4,
		BuildWinSpace,

	};

	/**************************************************************************/
	/*!
		\brief Callback function for when the window is minimized
		\param win
			pointer to the window
		\param action
			the action happening to the window
	*/
	/**************************************************************************/
	void window_iconify_cb(GLFWwindow * win, int action)
	{

		if (action == GLFW_TRUE)
		{
			/*
			if (fullscreen)
			{
				glfwSetWindowMonitor(window, fullscreen ? monitor : NULL, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			*/

			InputEvent e;
			e.action = "keydown";
			e.key = "esc";
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &e);
			//Settings::masterMuted = true;
			Audio::aManager->SetSnapshot("Pause");
			Settings::sfxMuted = true;
		}
		else
		{
			/*
			if (fullscreen)
			{
				glfwSetWindowSize(window, 800, 600);
			}
			*/
			//Settings::masterMuted = false;
			Audio::aManager->SetSnapshot("UnPause");
			Settings::sfxMuted = false;
		}
	}

	/**************************************************************************/
	  /*!
	  \brief Builds the space manager and the main menu space
	  */
	  /**************************************************************************/
	void Init()
	{
		spaceManager = new SpaceManager();
#ifndef TESTING
		//BuildSplashSpace();
#endif // !TESTING

	}

	/**************************************************************************/
	  /*!
	  \brief Destroys the space manager
	  */
	  /**************************************************************************/
	void Shutdown()
	{
		delete spaceManager;
	}

	/**************************************************************************/
		/*!
		\brief Updates and draws the space manager
		\param dt
		  time since last frame
		*/
		/**************************************************************************/
	void Update(float dt)
	{
		spaceManager->Update(dt);
		spaceManager->Draw();
	}

	/**************************************************************************/
	/*!
		\brief Returns a vector containing the position the player should spawn
			in a given level
		\param path
			filepath to level file
		\return
			Returns vector with the appropriate position
	*/
	/**************************************************************************/
	static Vector2D GetSpawnPos(std::string path)
	{
		auto tokens = Utils::tokenize_file(path);
		int iy_pos, ix_pos;
		float y_pos, x_pos;
		std::stringstream ss;
		ss << tokens[0][0];
		ss >> ix_pos;
		ss.str(std::string());
		ss.clear();
		ss << tokens[0][1];
		ss >> iy_pos;
		x_pos = (tokens[0][2] == "-") ? float(~ix_pos + 1) : float(ix_pos);
		y_pos = (tokens[0][3] == "-") ? float(~iy_pos + 1) : float(iy_pos);
		return Vector2D(x_pos, y_pos);
	}

	/**************************************************************************/
	/*!
		\brief Gets the name of the json file for the level being loaded
		\param path
			filepath to the text file that contains the spawn locations, and
			the json files for building level
		\return
			Returns the name of the json file
	*/
	/**************************************************************************/
	static std::string GetLevelFile(std::string path)
	{
		auto tokens = Utils::tokenize_file(path);
		return tokens[1][0];
	}

	/**************************************************************************/
	  /*!
	  \brief Builds level 1
	  */
	  /**************************************************************************/
	void BuildLevel1()
	{
		// set the default start position for level 1
		Vector2D spawn_location = GetSpawnPos("./assets/rooms/Level/Level_One.txt");
		std::string level_file = GetLevelFile("./assets/rooms/Level/Level_One.txt");

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Music_Title"));
		spaceManager->SetSpace(Screens::Game);
		Settings::culling = true;
		BuildNewMapSpace(level_file);
		GameObject * player = BuildPlayerspace();
		Transform * pt = player->GetComponent<Transform>();
		pt->Translation(spawn_location);
		BuildPauseSpace();
		spaceManager->AddSpace("99999999999999 level1");
		spaceManager->SetSpace(Spaces::Screens::Game);

		static int dimX, dimY;
		dimX = (int)((spawn_location[0] + 864.0f) / 1728.0f);
		dimY = (int)((spawn_location[1] - 560.0f) / 1088.0f);
		mainCam.moveRight(((float)dimX*1728.0f) - mainCam.center().x);
		mainCam.moveUp(((float)dimY*1088.0f) - mainCam.center().y);
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Level_1"));

	}

	/**************************************************************************/
	/*!
		\brief Builds Level2
	*/
	/**************************************************************************/
	void BuildLevel2()
	{
		Vector2D spawn_location = GetSpawnPos("./assets/rooms/Level/Level_Two.txt");
		std::string level_file = GetLevelFile("./assets/rooms/Level/Level_Two.txt");

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Level_1"));
		Settings::culling = true;
		BuildNewMapSpace(level_file);
		GameObject * player = BuildPlayerspace();
		Transform * pt = player->GetComponent<Transform>();
		pt->Translation(spawn_location);
		BuildPauseSpace();
		spaceManager->AddSpace("99999999999999 Level2");
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Level_2"));
		static int dimX, dimY;
		dimX = (int)((spawn_location[0] + 864.0f) / 1728.0f);
		dimY = (int)((spawn_location[1] - 560.0f) / 1088.0f);
		mainCam.moveRight(((float)dimX*1728.0f) - mainCam.center().x);
		mainCam.moveUp(((float)dimY*1088.0f) - mainCam.center().y);
	}

	/**************************************************************************/
	/*!
		\brief Builds Level 3
	*/
	/**************************************************************************/
	void BuildLevel3()
	{
		Vector2D spawn_location = GetSpawnPos("./assets/rooms/Level/Level_Three.txt");
		std::string level_file = GetLevelFile("./assets/rooms/Level/Level_Three.txt");

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Level_2"));
		Settings::culling = true;
		BuildNewMapSpace(level_file);
		GameObject * player = BuildPlayerspace();
		Transform * pt = player->GetComponent<Transform>();
		pt->Translation(spawn_location);
		BuildPauseSpace();
		spaceManager->AddSpace("9999999999999999 Level3");
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Level_3"));
		static int dimX, dimY;
		dimX = (int)((spawn_location[0] + 864.0f) / 1728.0f);
		dimY = (int)((spawn_location[1] - 560.0f) / 1088.0f);
		mainCam.moveRight(((float)dimX*1728.0f) - mainCam.center().x);
		mainCam.moveUp(((float)dimY*1088.0f) - mainCam.center().y);
	}

	/**************************************************************************/
	/*!
		\brief Builds Level 4
	*/
	/**************************************************************************/
	void BuildLevel4()
	{
		Vector2D spawn_location = GetSpawnPos("./assets/rooms/Level/Level_Four.txt");
		std::string level_file = GetLevelFile("./assets/rooms/Level/Level_Four.txt");

		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("stop Level_3"));
		Settings::culling = true;
		BuildNewMapSpace(level_file);
		GameObject * player = BuildPlayerspace();
		Transform * pt = player->GetComponent<Transform>();
		pt->Translation(spawn_location);
		BuildPauseSpace();
		spaceManager->AddSpace("99999999999999999 Level4");
		Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Ambient_Wind"));
		static int dimX, dimY;
		dimX = (int)((spawn_location[0] + 864.0f) / 1728.0f);
		dimY = (int)((spawn_location[1] - 560.0f) / 1088.0f);
		mainCam.moveRight(((float)dimX*1728.0f) - mainCam.center().x);
		mainCam.moveUp(((float)dimY*1088.0f) - mainCam.center().y);
	}
}
