/******************************************************************************/
/*!
\file   Window.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Window function, which handle creating window

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include <Windows.h>

#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <GLFW/glfw3.h>
#include <string>


/**************************************************************************/
  /*!
	\brief  Initialize the Window (Windows, Event Handlers).
  */
  /**************************************************************************/
void WindowInit();

/**************************************************************************/
  /*!
	\brief  Update the Window (Windows, Event Handlers).
	\param dt
		time value
  */
  /**************************************************************************/
void WindowUpdate(float dt);

/**************************************************************************/
  /*!
	\brief  Shutdown the Window (Windows, Event Handlers).
  */
  /**************************************************************************/
void WindowShutdown();

/**************************************************************************/
  /*!
	\brief  Get the pointer to the window
	\return GLFWwindow*
	    window pointer
  */
  /**************************************************************************/
GLFWwindow* GetWindow();
GLFWmonitor* GetMonitor();
const GLFWvidmode* GetMode();
bool checkFullscreen();


bool SaveScreenshot(std::string filename);

void MakeScreenshot(std::string filename);

void GetCursorPosition(float* x, float* y);

void ToggleFullScreen();

extern bool fullscreentext;

extern int resx, resy;