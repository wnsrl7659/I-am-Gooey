/******************************************************************************/
/*!
\file   timermanager.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	stores and updates all the countdown timers, so that we don't have to worry about that

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <vector>

class Timer;
class CountdownTimer;

namespace TimerManager
{
	void Init();

	void AddTimer(Timer *);

	void Update();

	void RemoveTimer(unsigned index);

	Timer * GetTimer(unsigned index);

	void Shutdown();

	CountdownTimer * NewCountdownTimer() noexcept(false);

	std::vector<Timer* > * GetTimers();
}

#endif // !TIMERMANAGER_H
