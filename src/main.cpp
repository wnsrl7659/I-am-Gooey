
/**************************************************************************/
/*!
\file main.cpp
*/
/**************************************************************************/
#define NWORD
#include <thread>

#include "system/Engine.h"
#include "system/common_defines.h"
#include "timer/timer.h"
#include <iostream>
#include "settings/settings.h"
#ifdef TESTING
#include "system/exceptions.h"
#include "trace/trace.h"
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _SUBMISSION
// disables the console window in submission mode
// if a console is needed for whatever reason, it can be added during runtime
// with AllocConsole and freed with FreeConsole

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif // _SUBMISSION
#undef main

int main(int argc, char * argv[])
{
#ifdef _WIN32
#ifdef _SUBMISSION
	ShowCursor(false);
#endif // _SUBMISSION
#endif // _WIN32

#ifdef TESTING
	try
	{
		Engine::Test();
	}
	catch (SOLException const& e)
	{
		std::cout << e.what() << std::endl;
		std::string p;
		try
		{
			p = Trace::CreateCrashDump();
		}
		catch (std::exception const& e)
		{
			std::cout << "Could not create crashdump file!" << std::endl;
			std::cout << e.what() << std::endl;
			exit(-1);
		}
		std::cout << "Dump File located at " << p << std::endl;
		exit(-1);
	}
#else
	pEngine = new Engine();
	
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	double frame_time = DELTA_TIME;

	using ch = std::chrono::high_resolution_clock;

	while (pEngine && pEngine->running)
	{
		// start time
		start = ch::now();
		pEngine->Update(frame_time);
		// calculate time to do work
		end = ch::now();
		std::chrono::duration<double, std::milli> ms = end - start;
		frame_time = ms.count() / 1000.0;
		// if the time it took was less than the frame limit time
		// sleep until then
	}
	delete pEngine;
#endif // TESTING
	return 0;
}
