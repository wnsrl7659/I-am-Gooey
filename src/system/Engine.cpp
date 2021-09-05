/******************************************************************************/
/*!
\file   Engine.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Engine class, which handle every object's constructer, destructer, update

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Engine.h"
#include "Window.h"
#include "trace\trace.h"
#include "messaging/messaging.h"
#include "audio/audio.h"
#include "../graphics/Shader.h"
#include "../graphics/TextureManager.h"
#include "../graphics/MeshManager.h"
#include "../input/input.h"
#include "../settings/settings.h"
#include "../component/collfix.h"
#include "timermanager.h"

#include "../component/component.h"
#include "../graphics/Camera.h"
#include "../vfx/LightManager.h"
#include "../system/heatmap.h"
#include "../utils/playback.h"
#include "../graphics/TileParser.h"
#include "../utils/autoplay.h"
#include "../system/TrueType.h"
#include "../puzzles/newpuzzle.h"
#include "../graphics/SpriteOrder.h"
#include "../graphics/Fade.h"
#include "../system/countdowntimer.h"
#include "localize.h"
#ifdef TESTING
#include "exceptions.h"
#include <intrin.h>
#include <algorithm>
#include <boost/locale.hpp>
#include "sysinfo.h"
#include "../utils/utils.h"
#endif // TESTING



Engine * pEngine = nullptr;

#ifdef TESTING
static void sysinfotest();
static void spacetest();
static void graphicstest() {};
static void settingstest();
static void timertest();
static void maptest();
static void localetest();

void Engine::Test()
{
	Messaging::Init();
	sysinfotest();
	//spacetest();
	//settingstest();
	//timertest();
	//maptest();
	localetest();
	Messaging::Shutdown();
}

static void localetest()
{
	System::InitLocalization();

	std::stringstream ss;
	ss << Utils::GetGameDataFolder() << "/localetest.txt";
	std::wofstream f(ss.str());
	std::wstring s;
	s = boost::locale::translate(L"Hello World");
	f << s << L'\n';
	System::SetLocale("./localization", "ja", "ja_JP.UTF-8");
	s = boost::locale::translate(L"Hello World");
	f << s << L'\n';
	f.close();
}

static void timertest()
{
	Timer tm;
	std::cout << "Running timer for 5 seconds" << std::endl;
	tm.Start();
	while (tm.GetSeconds() < 5.0)
	{
		std::cout << tm.GetMilliseconds() << " (" << tm.GetSeconds() << ")." << std::endl;
	}
	tm.Stop();
}

static void settingstest()
{
  Settings::Init();
  auto i = Settings::GetInts();
  auto b = Settings::GetBools();

  std::cout << "Settings Test\n----------" << std::endl;
  std::cout << "Ints:\n----------" << std::endl;
  std::for_each(i->cbegin(), i->cend(), [](std::pair<std::string, int *> p) -> void {std::cout << p.first << " = " << *p.second << std::endl; });
  std::cout << "Bools:\n----------" << std::endl;
  std::for_each(b->cbegin(), b->cend(), [](std::pair<std::string, bool *> p) -> void {std::cout << p.first << " = " << std::boolalpha << *p.second << std::endl; });
}

static void sysinfotest()
{
	WindowInit();
	System::cpuinfo c;
	System::gpuinfo g;
	std::cout << c << std::endl;
	std::cout << g << std::endl;
	WindowShutdown();
}

static void spacetest()
{
	Spaces::Init();

	int objectnum = 4900000;
	int spacenum = 100;
	double timeM = 0.0;
	double timeS = 0.0;
	Space * tst = nullptr;

	std::cout << "Space Test\n----------" << std::endl;
	std::cout << "Building " << spacenum << " spaces" << std::endl;
	Timer tm;
	tm.Start();
	for (int i = 0; i < spacenum; ++i)
	{
		std::stringstream ss;
		ss << i;
		try
		{
			Space * sp = Spaces::spaceManager->AddSpace(ss.str());
		}
		catch (std::bad_alloc&)
		{
			throw SOLException(System::EType::OUT_OF_MEMORY, "Out of system memory!");
		}
	}
	timeM = tm.GetMilliseconds();
	timeS = tm.GetSeconds();
	tm.Stop();
	std::cout << "Time to complete: " << timeM << "ms" << " (" << timeS << "s)." << std::endl;
	std::cout << "Destroying " << spacenum << " spaces" << std::endl;
	tm.Start();
	Spaces::spaceManager->RemoveAllSpaces();
	timeM = tm.GetMilliseconds();
	timeS = tm.GetSeconds();
	tm.Stop();
	std::cout << "Time to complete: " << timeM << "ms" << " (" << timeS << "s)." << std::endl;
	std::cout << "Building " << objectnum << " empty objects" << std::endl;
	tm.Start();
	tst = Spaces::spaceManager->AddSpace("Test");
	for (int i = 0; i < objectnum; ++i)
	{
		std::stringstream ss;
		ss << i;
		try
		{
			GameObject * a = tst->AddObject(ss.str());
		}
		catch (std::bad_alloc&)
		{
			throw SOLException(System::EType::OUT_OF_MEMORY, "Out of system memory!");
		}
	}
	timeM = tm.GetMilliseconds();
	timeS = tm.GetSeconds();
	tm.Stop();
	std::cout << "Time to complete: " << timeM << "ms" << " (" << timeS << "s)." << std::endl;
	std::cout << "Destroying " << objectnum << " empty objects" << std::endl;
	tm.Start();
	Spaces::spaceManager->RemoveAllSpaces();
	timeM = tm.GetMilliseconds();
	timeS = tm.GetSeconds();
	tm.Stop();
	std::cout << "Time to complete: " << timeM << "ms" << " (" << timeS << "s)." << std::endl;

	Spaces::Shutdown();
}

static void maptest()
{
	std::cout << "Attempting to load map" << std::endl;
	LayerMap test("./assets/rooms/Level/LevelOneGoo.json");
	std::cout << "Map Loaded" << std::endl;
	test.PrintMapData();
	std::cout << "Finished Printing Map Data" << std::endl;
}

#endif // TESTING



/**************************************************************************/
  /*!
	\brief  Initializer
  */
  /**************************************************************************/
Engine::Engine()
{
	assert(comps.size() == CompType::ctCompCount);

	Trace::Init();
	Settings::Init();
	Messaging::Init();
	WindowInit(); //should first
	System::InitLocalization();
	TrueTypeInit();
	TimerManager::Init();
	MeshManagerInit();
	TextureManagerInit();
	LightManagerInit();
	ShaderInit();
	Input::Init();
	Audio::Init();
	//Data::Init();
	PzManage::Init();
	Spaces::Init();
	System::Init();
	FadeInit();
	SpriteOrderInit();
	InputCapture::Init();
	Playback::Init();
	Utils::InitTester();

	Spaces::BuildDigiSpace();
	//Spaces::BuildWinSpace();

	run_timer.Start();
}

/**************************************************************************/
  /*!
	\brief  desturcter
  */
  /**************************************************************************/
Engine::~Engine()
{
	Utils::ShutdownTester();
	Playback::Shutdown();
	InputCapture::Shutdown();
	SpriteOrderShutdown();
	FadeShutdown();
	System::Shutdown();
	System::ShutdownLocalization();
	Spaces::Shutdown();
	PzManage::Shutdown();
	//Data::Shutdown();
	Audio::Shutdown();
	Input::Shutdown();
	Messaging::Shutdown();
	Trace::Shutdown();
	LightManagerShutdown();
	ShaderShutdown();
	TextureManagerShutdown();
	TrueTypeShutdown();
	MeshManagerShutdown();
	TimerManager::Shutdown();
	WindowShutdown();
	run_timer.Stop();
}

/**************************************************************************/
  /*!
	\brief  Update
	\param dt
		time value
  */
  /**************************************************************************/
void Engine::Update(float dt)
{
	dt_ = dt;
	glClear(GL_COLOR_BUFFER_BIT);
	Audio::Update(dt);
	Input::Update();
	Playback::Update();
	Utils::atst->Update(dt);
	System::Update();
	Spaces::spaceManager->Update(dt);
	FadeUpdate(dt);
	SpriteOrderUpdate(dt);
	Spaces::spaceManager->Draw();
	//Data::Update(dt);
	mainCam.Update(dt);
	PzManage::Update(dt);
	LightManagerUpdate(dt);
	TrueTypeUpdate(dt);
	/* should call last */
	TimerManager::Update();
	WindowUpdate(dt);
	if (trailresetswitch == true)
	{
		trailresetcount++;
		if (trailresetcount == 2)
		{
			trailresetswitch = false;
			trailresetcount = 0;
		}
	}
}

