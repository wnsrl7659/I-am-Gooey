/******************************************************************************/
/*!
\file   spacemanager.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Spacemanager class, handles all creation and deletion of spaces

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef SPACEMANAGER_H
#define SPACEMANAGER_H

#define LEVEL_LOADER_FUNC std::function<void(void)>

#include <map>
#include <string>
#include <sstream>
#include <functional>
#include <queue>
#include <array>

#include "../graphics/Sprite.h"
#include "../graphics/DebugDraw.h"
#include "../graphics/Camera.h"
#include "../component/controller.h"
#include "../animation/animation.h"
#include "../graphics/Tilemap.h"
#include "spaces.h"
#include "../physics/Transform.h"
#include "../physics/physics.h"
#include "../messaging/messaging.h"
#include "../component/paralax.h"
#include "../component/teleporter.h"

#include "../component/button.h"
#include "../timer/timer.h"
#include "../messaging/InputEvent.h"
#include "../vfx/LightManager.h"
#include "../system/timermanager.h"
#include "../puzzles/newpuzzle.h"

class SpaceManager;

namespace Spaces
{
	extern SpaceManager * spaceManager; // extern pointer to the manager


	enum class Screens
	{
		Splash = 0,
		Logo,
		MainMenu,
		Game,
		PauseMenu,
		Credits,
		Win,
		PausePause,

		NoScr,
	};

	enum Levels
	{
		Level1 = 0,
		Level2,
		Level3,
		Level4,
		Win,

		// special
		Count,
		initial = -1,
		WinState = Win,
	};

	extern std::array<LEVEL_LOADER_FUNC, Levels::Count> level_loaders;

	struct SpaceOrderer
	{
		bool operator()(const std::string& a, const std::string& b) const
		{
			if (a.empty() || b.empty())
				return false;
			std::stringstream ss1, ss2;
			ss1 << a.substr(0, a.find_first_of(' '));
			ss2 << b.substr(0, b.find_first_of(' '));

			int anum = 0, bnum = 0;
			ss1 >> anum;
			ss2 >> bnum;
			return anum < bnum;
		}
	};

	void window_iconify_cb(GLFWwindow * window, int action);
}

class SpaceManager
{
public:
	/**************************************************************************/
	/*!
		\brief Constructor
	*/
	/**************************************************************************/
	SpaceManager();
	/**************************************************************************/
	/*!
		\brief Destructor
	*/
	/**************************************************************************/
	~SpaceManager();

	/**************************************************************************/
	/*!
		\brief
			Adds a space to the manager
		\param name
			Name of the space being added
		\return
			Pointer to the newly made space
	*/
	/**************************************************************************/
	Space * AddSpace(const std::string& name);

	/**************************************************************************/
	/*!
		\brief Removes a space from the manager
		\param name
			Name of the space to be removed
	*/
	/**************************************************************************/
	void RemoveSpace(const std::string& name);

	/**************************************************************************/
	/*!
		\brief Marks a space to be removed in the next frame
		\param name
			Name of the space being marked
	*/
	/**************************************************************************/
	void MarkSpaceForRemoval(const std::string& name);

	/**************************************************************************/
	/*!
		\brief Removes all spaces from the manager
	*/
	/**************************************************************************/
	void RemoveAllSpaces();

	/**************************************************************************/
	/*!
		\brief Marks a space for removal
		\param name
			pointer to the name of the space to be removed
	*/
	/**************************************************************************/
	void MarkSpaceForRemoval(std::string * name)
	{
		MarkSpaceForRemoval(*name);
	};

	/**************************************************************************/
	/*!
		\brief Marks all spaces in the manager for removal
	*/
	/**************************************************************************/
	void MarkAllSpacesForRemoval();

	/**************************************************************************/
	/*!
		\brief Gets a space from the manager
		\param name
			Name of the space to get
		\return
			Returns a pointer to the space, or nullptr if not found
	*/
	/**************************************************************************/
	Space * GetSpace(const std::string& name);

	/**************************************************************************/
	/*!
		\brief Gets a space with a name that contains @name
		\param name
			name to search for
		\return 
			Returns a pointer to the space, or nullptr if not found
	*/
	/**************************************************************************/
	Space * GetSimilarSpace(const std::string& name);

	/**************************************************************************/
	/*!
		\brief Resets the current space
	*/
	/**************************************************************************/
	void ResetSpace();

	/**************************************************************************/
	/*!
		\brief Sets the alpha of all objects in a given space
		\param name
			Name of the space
		\param a
			Value to set alpha to
	*/
	/**************************************************************************/
	void SetAllAlpha(const std::string& name, float a);

	/**************************************************************************/
	/*!
		\brief 
			Updates the spaces contained in the manager.

			Before anything is updated, the build/clear events are processed
			in the order that they were added in.
		\param dt
			Time since last frame
	*/
	/**************************************************************************/
	void Update(float dt);

	/**************************************************************************/
	/*!
		\brief Draws all the spaces contained
	*/
	/**************************************************************************/
	void Draw();

	/**************************************************************************/
	/*!
		\brief Marks the space manager for resetting
	*/
	/**************************************************************************/
	void MarkForReset() { reset_ = true; };

	/**************************************************************************/
	/*!
		\brief 
			Sets the current screen
		\param s
			Screen to be set to current
	*/
	/**************************************************************************/
	void SetSpace(Spaces::Screens s) { currentScreen_ = s; };

	/**************************************************************************/
	/*!
		\brief 
			Gets the current screen
		\return
			The current screen
	*/
	/**************************************************************************/
	Spaces::Screens GetScreen() { return currentScreen_; };

	/**************************************************************************/
	/*!
		\brief 
			Gets the previous screen
		\return
			The previous screen
	*/
	/**************************************************************************/
	Spaces::Screens GetPrevScreen() { return prevScreen_; };

	/**************************************************************************/
	/*!
		\brief 
			Sets the previous screen to the given value
		\param s
			Value to set prevScreen_ to
	*/
	/**************************************************************************/
	void SetPrevScreen(Spaces::Screens s) { prevScreen_ = s; };

	/**************************************************************************/
	/*!
		\brief
			Gets the maps of spaces
		\return
			Returns a pointer to the underlying map of spaces
	*/
	/**************************************************************************/
	std::unordered_map<std::string, Space *> * GetSpaces();

	/**************************************************************************/
	/*!
		\brief 
			Queues a clear event in the manager.

			A clear event removes all the spaces from the manager, as well as
			rebuilding the puzzle manager, light manager, and timer manager.
			Clear and build events are run at the beginning of the Space
			Manager's update function, before anything is updated but after
			the queue of next frame functions added by ExecuteNextFrame are
			processed.

			Use these functions to avoid iterator invalidation in the middle of
			a loop.
	*/
	/**************************************************************************/
	void QueueClearEvent();

	/**************************************************************************/
	/*!
		\brief
			Queues a build event in the manager.

			A build event is just a wrapper for a std::function that takes void
			and returns void. It can technically be anything, but the intention
			was for this to be building levels and such, especially right after
			a clear event is run. For non building events that need to be run
			at the beginning of the update loop to avoid invalidation, use
			SpaceManager::ExecuteNextFrame.

		\param f
			Function to be run when this event is executed.
	*/
	/**************************************************************************/
	void QueueBuildEvent(std::function<void(void)> f);

	/**************************************************************************/
	/*!
		\brief
			Queues a function to be run in the next frame.

			Functions added by ExecuteNextFrame are the first thing to be processed
			in SpaceManager::Update, before even the build and clear events. Use
			this function if you need to call a function that will invalidate
			the iterator updating the space manager

		\param f
			Function to be run at process time.
	*/
	/**************************************************************************/
	void ExecuteNextFrame(std::function<void(void)> f);

	/**************************************************************************/
	/*!
		\brief
			Loads the next level.

			If already on the last level, goes to the main menu
	*/
	/**************************************************************************/
	void LoadNextLevel();

	void LoadNextLevel(void *);

	Spaces::Screens currentScreen_ = Spaces::Screens::NoScr;

	Spaces::Levels clevel() const
	{
		return c_level;
	}

	void SetLevel(Spaces::Levels l)
	{
		c_level = l;
	}

private:
	// this struct is for clear and build events
	// for the space manager.
	// These are major events, not just for single spaces
	// but more for changing scenes.
	struct SpaceEvent
	{
		enum class Type
		{
			Clear = 0, // Clear event will remove all spaces and objects in spaces
			Build, // Build event will call the attached function
		};
		Type type_;
		std::function<void(void)> func_;

		SpaceEvent(Type t) : type_(t) {};
		SpaceEvent(Type t, std::function<void(void)> f) : type_(t), func_(f) {};

		void operator()()
		{
			switch (type_)
			{
			case Type::Clear:
				Spaces::spaceManager->RemoveAllSpaces();
				if (PzManage::pzm && PzManage::dorm)
				{
					PzManage::pzm->Clear();
					PzManage::dorm->Clear();
				}
				if (lightManager)
				{
					lightManager->DeleteAllLights();
					lightManager->DeleteAllWalls();
				}
				TimerManager::Shutdown();
				TimerManager::Init();
				break;
			case Type::Build:
				func_();
				break;
			}
		}
	};

	void SpaceInputHandler(InputEvent * data);
	void SpaceButtonHandler(ButtonEvent * data);

	std::queue<SpaceEvent> event_queue; // queue of the clear and build events
	std::queue < std::function<void(void)>> func_queue; // queue of function to execute at the next frame
	std::unordered_map<std::string, Space *> spaces;
	//std::map<std::string, Space *> spaces;
	bool spacesNeedingRemoval_ = false;
	bool startGame_ = false;
	bool returnToMenu_ = false;
	bool win_ = false;
	bool reset_ = false;
	void StartGameFromMenu();
	void StartMenuFromGame();
	void WinGameFromGame();
	Spaces::Screens prevScreen_ = Spaces::Screens::NoScr;
	Timer* splashTimer;
	Timer* creditsTimer;
	Spaces::Levels c_level = Spaces::Levels::initial;
};

namespace Spaces
{

	void Init();

	void Shutdown();

	void BuildLevel1();

	void BuildLevel2();

	void BuildLevel3();

	void BuildLevel4();

	void BuildAudioTest();

	GameObject * BuildPlayerspace();

	void BuildHouse();


	void BuildNewMapSpace(const std::string& filepath);

	void BuildUIspace();


	void BuildMainMenu();

	void BuildPauseSpace();

	void BuildCreditsSpace();

	void BuildSplashSpace();

	void BuildDigiSpace();

	void BuildRadiantSpace();

	void BuildEngineSpace();

	void BuildWinSpace();

	void BuildPoptionSpace();

	void BuildOptionMenu();

	void BuildHowToPlaySpace();
}

extern void* digitimer;
extern void* digisprite;
#endif // !SPACEMANAGER_H
