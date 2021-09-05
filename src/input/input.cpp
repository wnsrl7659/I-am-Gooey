/******************************************************************************/
/*!
\file   input.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Input Handler Module using GLFW

*/
/******************************************************************************/

#include "input.h"
#include "../trace/trace.h"
#include "../messaging/InputEvent.h"
#include "../system/Window.h"
#include "../messaging/messagehandler.h"
#include "../messaging/messaging.h"
//#include "../../external/include/SDL.h"
#include <iostream>
#include "../settings/settings.h"
#include "../component/controller.h"
#include "../graphics/Fade.h"

namespace Input
{
	//SDL_Window *window; //Window Pointer, Required for SDL to know what Window it can listen to for input
	//SDL_Event event; //Event Pointer, checks every update loop for queued events
	//SDL_GameController *controller = NULL; //Controller Pointer, set to Null by default
	//
	// /**************************************************************************/
	// /*!
	//  \brief Initializes SDL and sets up controllers, if there are any.
	// */
	///**************************************************************************/
	//void Init()
	//{
	//	//Initializes SDL Modules
	//	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) == -1))
	//	{
	//		//returns an error if they fail
	//		std::cout << "Could not initialize SDL\n";
	//		Trace::Message("SDL Init Error\n");
	//	}
	//	else
	//	{
	//		Trace::Message("SDL Init Successful\n");
	//	}
	//	//Temporarily Creates window, will instead assign window to OpenGL window
	//	window = SDL_CreateWindow("temp window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	//	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	//	for (int i = 0; i <= SDL_NumJoysticks(); i++)
	//	{
	//		//Game Controller Assignment Loop
	//		if (SDL_IsGameController(i))
	//		{
	//			//Controller gets assigned to first valid game controller plugged in to the computer
	//			controller = SDL_GameControllerOpen(i);
	//			if (controller)
	//			{
	//				Trace::Message("Controller Found\n");
	//				//breaks immediately after it is found, only 1 controller allowed
	//				break;
	//			}
	//			else
	//			{
	//				Trace::Message("Controller Erorr\n");
	//				std::cout << "Could not find game controller" << i << SDL_GetError() << std::endl;
	//			}
	//			
	//		}
	//		Trace::Message("No Controller Found\n");
	//	}
	//}
	///**************************************************************************/
	///*!
	//\brief Shuts down SDL
	//*/
	///**************************************************************************/
	//void Shutdown()
	//{
	//	//Calls SDL's Cleanup
	//	Trace::Message("Closing SDL\n");
	//	SDL_Quit();
	//}


	///**************************************************************************/
	///*!
	//\brief Checks for any SDL events in the event queue, and calls the handler for the corresponding event type
	//*/
	///**************************************************************************/
	//void GetInput()
	//{
	//	//While there are events in the queue
	//	while (SDL_PollEvent(&event))
	//	{
	//		//check for types of events
	//		if (event.type == SDL_KEYDOWN || event.type == SDL_CONTROLLERBUTTONDOWN)
	//		{
	//			//for key presses or button presses, call the input handler
	//			buttonInput();
	//		}
	//		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
	//		{
	//			mouseInput();
	//		}
	//		if (event.type == SDL_CONTROLLERAXISMOTION)
	//		{
	//			joyInput();
	//		}
	//		if (event.type == SDL_QUIT)
	//		{
	//			Shutdown();
	//		}
	//	}
	//}

	///**************************************************************************/
	///*!
	//\brief Handles keyboard and controller button input
	//*/
	///**************************************************************************/
	//void buttonInput()
	//{
	//	//key press switch statement block
	//	if (event.type == SDL_KEYDOWN)
	//	{
	//		switch (event.key.keysym.sym)
	//		{
	//		case SDLK_ESCAPE:
	//			//Brings up Menu
	//			std::cout << "ESC\n";
	//			break;
	//		case SDLK_w:
	//			//Moves Player Up
	//			std::cout << "W\n";
	//			break;
	//		case SDLK_s:
	//			//Moves Player Down
	//			std::cout << "S\n";
	//			break;
	//		case SDLK_a:
	//			//Moves Player Left
	//			std::cout << "A\n";
	//			break;
	//		case SDLK_d:
	//			//Moves Player Right
	//			std::cout << "D\n";
	//			break;
	//		case SDLK_j:
	//			//Plays String 0
	//			std::cout << "String 3\n";
	//			break;
	//		case SDLK_k:
	//			//Plays String 1
	//			std::cout << "String 4\n";
	//			break;
	//		case SDLK_l:
	//			//Plays String 2
	//			std::cout << "String 5\n";
	//			break;
	//		case SDLK_u:
	//			//Plays String 3
	//			std::cout << "String 0\n";
	//			break;
	//		case SDLK_i:
	//			//Plays String 4
	//			std::cout << "String 1\n";
	//			break;
	//		case SDLK_o:
	//			//Plays String 5
	//			std::cout << "String 2\n";
	//			break;
	//		}
	//	}
	//	//controller button switch statement block
	//	if (event.type == SDL_CONTROLLERBUTTONDOWN)
	//	{
	//		switch (event.cbutton.button)
	//		{
	//		case SDL_CONTROLLER_BUTTON_A:
	//			//A Button
	//			std::cout << "A is Pressed\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_B:
	//			//B Button
	//			std::cout << "B is Pressed\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_X:
	//			//X Button
	//			std::cout << "X is Pressed\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_Y:
	//			//Y Button
	//			std::cout << "Y is Pressed\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_DPAD_UP:
	//			//Up Dpad
	//			std::cout << "DPAD UP\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
	//			//Down Dpad
	//			std::cout << "DPAD DOWN\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
	//			//Left Dpad
	//			std::cout << "DPAD LEFT\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
	//			//Right Dpad
	//			std::cout << "DPAD RGIHT\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_START:
	//			//Start Button
	//			std::cout << "Start\n";
	//			break;

	//		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
	//			//rb
	//			std::cout << "RB\n";
	//			break;

	//		}
	//	}
	//}
	///**************************************************************************/
	///*!
	//\brief Handles Mouse Input
	//*/
	///**************************************************************************/
	//void mouseInput()
	//{
	//	if (event.type == SDL_MOUSEBUTTONDOWN)
	//	{
	//		switch (event.button.button)
	//		{
	//		case SDL_BUTTON_LEFT:
	//			std::cout << "M1 Down\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		case SDL_BUTTON_RIGHT:
	//			std::cout << "M2 Down\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		case SDL_BUTTON_MIDDLE:
	//			std::cout << "M3 Down\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		}
	//	}
	//	if (event.type == SDL_MOUSEBUTTONUP)
	//	{
	//		switch (event.button.button)
	//		{
	//		case SDL_BUTTON_LEFT:
	//			std::cout << "M1 up\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		case SDL_BUTTON_RIGHT:
	//			std::cout << "M2 up\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		case SDL_BUTTON_MIDDLE:
	//			std::cout << "M3 Down\n";
	//			std::cout << event.button.x << "," << event.button.y << "\n";
	//			break;
	//		}
	//	}
	//}
	///**************************************************************************/
	///*!
	//\brief Handles controller joystick input
	//*/
	///**************************************************************************/
	//void joyInput()
	//{
	//	//MAGIC NUMBER WARNING TURN BACK NOW
	//	//TODO: NO MORE MAGIC NUMBERS IM SORRY
	//	if (event.type == SDL_CONTROLLERAXISMOTION)
	//	{
	//		//controller axis event
	//		switch (event.caxis.axis)
	//		{
	//			//left stick x event
	//		case SDL_CONTROLLER_AXIS_LEFTX:
	//			if (event.caxis.value > 8000)
	//			{
	//				//positive axis is right
	//				std::cout << "Right\n";
	//				break;
	//			}
	//			else if (event.caxis.value < -8000)
	//			{
	//				//negative is left
	//				std::cout << "Left\n";
	//				break;
	//			}
	//			break;
	//			//left stick y event
	//		case SDL_CONTROLLER_AXIS_LEFTY:
	//			if (event.caxis.value > 8000)
	//			{
	//				//positive is down
	//				std::cout << "Down\n";
	//				break;
	//			}
	//			else if (event.caxis.value < -8000)
	//			{
	//				//negative is up
	//				std::cout << "Up\n";
	//				break;
	//			}
	//			break;
	//		}
	//	}
	//}

	/**************************************************************************/
	/*!
	\brief Handles keyboard input
	\param key
		What key is pressed
	\param action
		What is happening to the key
	*/
	/**************************************************************************/
	static bool inputActive = true;
	static void handleInput(int key, int action, GLFWwindow* window)
	{
		InputEvent event("none","none");
		
		switch (key)
		{
		case GLFW_KEY_Q:
			event.key = "q";
			break;
		case GLFW_KEY_E:
			event.key = "e";
			break;
		case GLFW_KEY_R:
			event.key = "r";
			break;
		case GLFW_KEY_T:
			event.key = "t";
			break;
		case GLFW_KEY_Y:
			event.key = "y";
			break;
		case GLFW_KEY_U:
			event.key = "u";
			break;
		case GLFW_KEY_I:
			event.key = "i";
			break;
		case GLFW_KEY_O:
			event.key = "o";
			break;
		case GLFW_KEY_P:
			event.key = "p";
			break;
		case GLFW_KEY_W:
			event.key = "w";
			break;
		case GLFW_KEY_F:
			event.key = "f";
			break;
		case GLFW_KEY_G:
			event.key = "g";
			break;
		case GLFW_KEY_H:
			event.key = "h";
			break;
		case GLFW_KEY_J:
			event.key = "j";
			break;
		case GLFW_KEY_K:
			event.key = "k";
			break;
		case GLFW_KEY_L:
			event.key = "l";
			break;
		case GLFW_KEY_S:
			event.key = "s";
			break;
		case GLFW_KEY_A:
			event.key = "a";
			break;
		case GLFW_KEY_D:
			event.key = "d";
			break;
		case GLFW_KEY_Z:
			event.key = "z";
			break;
		case GLFW_KEY_X:
			event.key = "x";
			break;
		case GLFW_KEY_C:
			event.key = "c";
			break;
		case GLFW_KEY_V:
			event.key = "v";
			break;
		case GLFW_KEY_B:
			event.key = "b";
			break;
		case GLFW_KEY_N:
			event.key = "n";
			break;
		case GLFW_KEY_M:
			event.key = "m";
			break;
		case GLFW_KEY_SEMICOLON:
			event.key = ";";
			break;
		case GLFW_KEY_APOSTROPHE:
			event.key = "'";
			break;
		case GLFW_KEY_COMMA:
			event.key = ",";
			break;
		case GLFW_KEY_PERIOD:
			event.key = ".";
			break;
		case GLFW_KEY_BACKSLASH:
			event.key = "\\";
			break;
		case GLFW_KEY_SLASH:
			event.key = "/";
			break;
		case GLFW_KEY_1:
			event.key = "1";
			break;
		case GLFW_KEY_2:
			event.key = "2";
			break;
		case GLFW_KEY_3:
			event.key = "3";
			break;
		case GLFW_KEY_4:
			event.key = "4";
			break;
		case GLFW_KEY_5:
			event.key = "5";
			break;
		case GLFW_KEY_6:
			event.key = "6";
			break;
		case GLFW_KEY_ESCAPE:
			event.key = "esc";
			break;
		case GLFW_KEY_ENTER:
			event.key = "enter";
			break;
		case GLFW_KEY_UP:
			//event.key = "up";
			event.key = "uup";
			break;
		case GLFW_KEY_DOWN:
			//event.key = "down";
			event.key = "ddown";
			break;
		case GLFW_KEY_LEFT:
			//event.key = "left";
			event.key = "lleft";
			break;
		case GLFW_KEY_RIGHT:
			//event.key = "right";
			event.key = "rright";
			break;
		case GLFW_KEY_SPACE:
			event.key = "space";
			break;
		case GLFW_KEY_F1:
			if(action == GLFW_RELEASE)
				inputActive = !inputActive;
			break;
		case GLFW_KEY_F11:
			event.key = "F11";
			break;
		}
		if (inputActive && IsFadeWorking() == false)
		{
			if (glfwGetKey(window, key) == GLFW_PRESS)
			{
				event.action = "keydown";
			}
			else if (glfwGetKey(window, key) == GLFW_RELEASE)
			{
				event.action = "keyup";
			}
			std::stringstream ss;
			ss << "Button " << event.key << " " << event.action;
			Trace::Message(ss.str());
			Messaging::messHandler->HandleMessage("InputEvent", &event);
		}
	}

	static void handleMouseInput(int button, int action, GLFWwindow* window)
	{
		InputEvent event("none", "none");
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			event.key = "mouse left";
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			event.key = "mouse right";
			break;
		}
		if (inputActive)
		{
			if (glfwGetMouseButton(window, button) == GLFW_PRESS)
			{
				event.action = "keydown";
			}
			else if (glfwGetMouseButton(window, button) == GLFW_RELEASE)
			{
				event.action = "keyup";
			}
			std::stringstream ss;
			ss << "Button " << event.key << " " << event.action;
			Trace::Message(ss.str());
			Messaging::messHandler->HandleMessage("InputEvent", &event);
		}
	}
	/**************************************************************************/
	/*!
	\brief The GLFW key callback, activates whenever an input event is got
	\param window
		GLFW window to check for input in
	\param key
		Key that is affected by action
	\param scancode
		placeholder text (idk what scancode is for lol)
	\param action
		key down, key up, etc
	\param mods
		things like Alt, Shift, Ctrl
	*/
	/**************************************************************************/
	bool is_A_down, is_B_down, is_A_up, is_B_up;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//GLFWwindow* windows = GetWindow();
		GLFWmonitor* monitor = GetMonitor();
		const GLFWvidmode * mode = GetMode();
		bool fullscreen = checkFullscreen();

		if (key == GLFW_KEY_LEFT_CONTROL) is_A_down = action == GLFW_PRESS;
		if (key == GLFW_KEY_LEFT_ALT) is_B_down = action == GLFW_PRESS;
		if (is_A_down && is_B_down)
		{
			if (fullscreen)
			{
				glfwSetWindowSize(window, mode->width, mode->height);
			}
		}
		if (key == GLFW_KEY_LEFT_CONTROL) is_A_up = action == GLFW_RELEASE;
		if (key == GLFW_KEY_LEFT_ALT) is_B_up = action == GLFW_RELEASE;
		if (is_A_up && is_B_up)
		{
			if (fullscreen)
			{
				glfwSetWindowSize(window, resx, resy);
			}
		}

		if (action == GLFW_PRESS || action == GLFW_RELEASE)
		{
			handleInput(key, action, window);
			plhandleInput(key, action, window);
		}
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
		{
			handleMouseInput(button, action, window);
		}
	}
	/**************************************************************************/
	/*!
	  \brief Initializes the Input Module, sets up the Key Callback function
	*/
	/**************************************************************************/
	void Init()
	{
		GLFWwindow * window = GetWindow();
		if (window)
		{
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
		}
	}
	/**************************************************************************/
	/*!
	\brief Polls for Events
	*/
	/**************************************************************************/
	void Update()
	{
		glfwPollEvents();
	}
	/**************************************************************************/
	/*!
	\brief Shuts down the input module
	*/
	/**************************************************************************/
	void Shutdown()
	{
	}
}
