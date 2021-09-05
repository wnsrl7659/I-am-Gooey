/******************************************************************************/
/*!
\file	playback.h
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Captures input events from player so we could play back the gameplay
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <list>
#include <chrono>
#include <fstream>

#include "../timer/timer.h"

class InputEvent;
class Timer;

class InputCatcher
{
public:
	/**************************************************************************/
	/*!
	\brief Constructor
	*/
	/**************************************************************************/
	InputCatcher();

	/**************************************************************************/
	/*!
	\brief Destructor
	*/
	/**************************************************************************/
	~InputCatcher();

	void Start();
	void Stop();

private:
	/**************************************************************************/
	/*!
	\brief Handles the received events
	\param
	*/
	/**************************************************************************/
	void Handler(InputEvent * data);

	std::list<std::pair<double, InputEvent>> events_;

	Timer tm_;

	std::ofstream f_;

	bool running_ = false;
};

class PlaybackMachine
{
public:
	PlaybackMachine();
	~PlaybackMachine();

	void LoadPlaybackFile(std::string file);
	void StartPlayback();
	void StopPlayback();

	std::string CurrentFile() const;

	void Update();

private:
	std::list<std::pair<double, InputEvent>> events_;

	bool running_ = false;

	Timer tm_;

	std::list<std::pair<double, InputEvent>>::iterator it_;

	std::string cfile_;
};

namespace InputCapture
{
	extern InputCatcher * icatcher;

	void Init();

	void Shutdown();
}

namespace Playback
{
	extern PlaybackMachine * ply;

	void Init();

	void Update();

	void Start();
	
	void Stop();

	void Shutdown();
}

#endif // !PLAYBACK_H
