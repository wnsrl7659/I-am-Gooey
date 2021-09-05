/******************************************************************************/
/*!
\file   timer.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  wrapper for the timer class made with SDL

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#define GETTIME(x, y, z) std::chrono::duration_cast<x>(y - z).count()

#include "timer.h"
#include <sstream>

std::ostream& operator<<(std::ostream& os, Timer const& t)
{
	std::stringstream ss;
	ss << t.GetSeconds() << "s, " << t.GetMilliseconds() << "ms, " << t.GetNanoseconds() << "ns ";
	os << ss.str();
	return os;
}

/**************************************************************************/
/*!
\brief Constructor for timer
*/
/**************************************************************************/
Timer::Timer()
{
	isPaused_ = false;
	isStarted_ = false;
}

bool Timer::Done() const
{
	return done_;
}

/**************************************************************************/
/*!
\brief Starts the timer
*/
/**************************************************************************/
void Timer::Start()
{
	start_ = std::chrono::high_resolution_clock::now();
	isStarted_ = true;
}

/**************************************************************************/
/*!
\brief stops and resets the timer
*/
/**************************************************************************/
void Timer::Stop()
{
	isStarted_ = false;
	isPaused_ = false;
}

void Timer::Restart()
{
	Stop();
	Start();
}

/**************************************************************************/
/*!
\brief Pauses the timer
*/
/**************************************************************************/
void Timer::Pause()
{
	if (isStarted_ && !isPaused_)
	{
		isPaused_ = true;
		pausedat_ = std::chrono::high_resolution_clock::now();
	}
}

/**************************************************************************/
/*!
\brief Unpauses the timer
*/
/**************************************************************************/
void Timer::Unpause()
{
	if (isStarted_ && isPaused_)
	{
		isPaused_ = false;
		// add time since the timer was paused to account for time not running
		start_ += (std::chrono::high_resolution_clock::now() - pausedat_);
	}
}

/**************************************************************************/
/*!
\brief Returns seconds since the timer was started
\return Seconds since start
*/
/**************************************************************************/
double Timer::GetSeconds() const
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_;
	return ms.count() / 1000.0;
}

/**************************************************************************/
/*!
\brief Returns milliseconds since the timer was started
\return ms since start
*/
/**************************************************************************/
double Timer::GetMilliseconds() const
{
	std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_;
	return ms.count();
}

long long Timer::GetNanoseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_).count();
}

template<typename _Duration>
long long Timer::GetDuration() const
{
	return std::chrono::duration_cast<_Duration>(std::chrono::high_resolution_clock::now() - start_).count();
}

/**************************************************************************/
/*!
\brief returns if the timer is paused
\return isPaused_ && isStarted_
if the timer is paused after being started
*/
/**************************************************************************/
bool Timer::IsPaused() const
{
	return isPaused_ && isStarted_;
}

/**************************************************************************/
/*!
\brief returns if the timer is running
\return isStarted_
if the is currently started and running
*/
/**************************************************************************/
bool Timer::IsStarted() const
{
	return isStarted_;
}
