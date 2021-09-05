/******************************************************************************/
/*!
\file   timermanager.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	stores and updates all the countdown timers, so that we don't have to worry about that

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "countdowntimer.h"
#include "timermanager.h"
#include "../trace/trace.h"

namespace TimerManager
{
	static std::vector<Timer *> timers_;

	void Init()
	{
	}

	void AddTimer(Timer * tm)
	{
		if (tm)
		{
			timers_.push_back(tm);
		}
	}
	
	std::vector<Timer *> * GetTimers()
	{
		return &timers_;
	}

	void Update()
	{
		for (unsigned int i = 0; i < timers_.size(); ++i)
		{
			timers_[i]->Update();
			if (timers_[i]->Done())
			{
				// check if it is a countdowntimer
				CountdownTimer * cnt = dynamic_cast<CountdownTimer *>(timers_[i]);
				if (cnt)
				{
					cnt->Trigger();
					RemoveTimer(i);
				}
			}
		}
	}

	void RemoveTimer(unsigned index)
	{
		if (index >= timers_.size())
		{
			//Trace::Message()
			return;
		}
		timers_.erase(timers_.begin() + index);
	}

	Timer * GetTimer(unsigned index)
	{
		if (index >= timers_.size())
		{
			// trace error
			return nullptr;
		}
		return timers_[index];
	}

	void Shutdown()
	{
		for (auto tm : timers_)
		{
			tm->Stop();
			delete tm;
		}
		timers_.clear();
	}

	CountdownTimer * NewCountdownTimer()
	{
		try
		{
			return new CountdownTimer();
		}
		catch (std::bad_alloc const& e)
		{
			Trace::Error(e.what());
		}
	}
}
