/******************************************************************************/
/*!
\file   timer.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
header for the timer class made with SDL

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:

	/**************************************************************************/
	/*!
	\brief Constructor for timer
	*/
	/**************************************************************************/
	Timer();

	/**************************************************************************/
	/*!
	\brief Starts the timer
	*/
	/**************************************************************************/
	void Start();

	/**************************************************************************/
	/*!
	\brief stops and resets the timer
	*/
	/**************************************************************************/
	void Stop();

	void Restart();

	/**************************************************************************/
	/*!
	\brief Pauses the timer
	*/
	/**************************************************************************/
	void Pause();

	/**************************************************************************/
	/*!
	\brief Unpauses the timer
	*/
	/**************************************************************************/
	void Unpause();

	/**************************************************************************/
	/*!
	\brief Returns seconds since the timer was started
	\return Seconds since start
	*/
	/**************************************************************************/
	double GetSeconds() const;

	/**************************************************************************/
	/*!
	\brief Returns milliseconds since the timer was started
	\return ms since start
	*/
	/**************************************************************************/
	double GetMilliseconds() const;

	long long GetNanoseconds() const;

	template<typename _Duration>
	long long GetDuration() const;

	/**************************************************************************/
	/*!
	\brief returns if the timer is paused
	\return isPaused_ && isStarted_
	  if the timer is paused after being started
	*/
	/**************************************************************************/
	bool IsPaused() const;

	/**************************************************************************/
	/*!
	\brief returns if the timer is running
	\return isStarted_
	  if the is currently started and running
	*/
	/**************************************************************************/
	bool IsStarted() const;

	virtual void Update() {};

	bool Done() const;

private:
	std::chrono::high_resolution_clock::time_point start_;
	std::chrono::high_resolution_clock::time_point pausedat_;

	bool isPaused_; // if paused

	bool isStarted_; // if started
protected:
	bool done_ = false;

};

std::ostream& operator<<(std::ostream& os, Timer const& t);

#endif // !TIMER_H
