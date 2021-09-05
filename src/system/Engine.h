/******************************************************************************/
/*!
\file   Engine.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Engine class, which handle every object's constructer, destructer, update

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "../spaces/spacemanager.h"
#include "common_defines.h"
#include "../timer/timer.h"

#pragma once

class Engine
{
public:
	/**************************************************************************/
	  /*!
		\brief  Initializer
	  */
	  /**************************************************************************/
	Engine();
	/**************************************************************************/
	  /*!
		\brief  desturcter
	  */
	  /**************************************************************************/
	~Engine();
	/**************************************************************************/
	  /*!
		\brief  Update
		\param dt
			time value
	  */
	  /**************************************************************************/
	void Update(float dt);
	/**************************************************************************/
	  /*!
		\brief  common Shutdown function
	  */
	  /**************************************************************************/
	void Shutdown() {
		running = false;
	}

#ifdef TESTING
	static void Test();
#endif // TESTING

	bool running = true;
	bool reset = false;

	Timer run_timer;
	Timer frame_timer;

	float dt_ = 0.0f;

private:
};

extern Engine * pEngine;

