/******************************************************************************/
/*!
\file   messagehandler.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  class that handles sending messages to other systems. But instead of the old
  version where a physical message was passed around to everyone, this handler
  has a map of delegate functions that get connected to certain messages, and are
  passed a data pointer.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <unordered_map>
#include <string>
#include "message.h"
#include "../trace/trace.h"


template<typename T, typename P, void(T::*func)(P*)>
void FunctionWrapper(T* inst, P* data)
{
	(inst->*func)(data);
}

class Delegate
{
public:
	void * inst;
	typedef void(*funcType)(void *, void *);
	funcType func;
	void operator()(void * d)
	{
		if (inst && func && d)
		{
			return func(inst, d);
		}
	}
};

class MessageHandler
{
public:
	/**************************************************************************/
	/*!
	*/
	/**************************************************************************/
	MessageHandler() {};

	/**************************************************************************/
  /*!

  */
  /**************************************************************************/
	~MessageHandler()
	{
		// free any unreleased data
	};

	/**************************************************************************/
	/*!

	*/
	/**************************************************************************/
	template<typename D>
	void HandleMessage(std::string const& event, D* data)
	{
		Trace::Message(event);
		_restart_message_loop:

		if (data)
		{
			if (map.count(event))
			{

				for (auto e : map[event])
				{
					if (dirty_)
					{
						dirty_ = false;
						goto _restart_message_loop;
					}
					if (e.second && e.second->inst && e.second->func)
					{
						(*e.second)(data);
					}
				}
			}
		}
	}

	/**************************************************************************/
	/*!

	*/
	/**************************************************************************/
	template<typename T, typename D, void(T::* f)(D*)>
	void Attach(T * inst, std::string const& event)
	{
		Delegate * delegate = new Delegate();
		delegate->func = (void(*)(void*, void*))&FunctionWrapper<T, D, f>;
		delegate->inst = inst;
		map[event][inst] = delegate;
		dirty_ = true;
	}

	/**************************************************************************/
	/*!

	*/
	/**************************************************************************/
	template<typename T>
	void Detach(T * inst, const std::string& event)
	{
		if (map.count(event))
		{
			dirty_ = true;
			if (map[event].count(inst))
			{
				delete map[event][inst];
				map[event].erase(inst);
			}
			if (!map.count(event))
			{
				map.erase(event);

			}
		}
	}

private:
	std::unordered_map<std::string, std::unordered_map<void *, Delegate *>> map;
	bool dirty_ = false;
};

#endif // !MESSAGEHANDLER_H
