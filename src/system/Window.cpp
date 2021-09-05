/******************************************************************************/
/*!
\file   Window.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Window function, which handle creating window

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Window.h"
#include "Engine.h"
#include "sysinfo.h"

#include "imgui.h"
#include "../IMGUI/imgui_impl_glfw.h"
#include "../IMGUI/imgui_impl_opengl3.h"
#include "../settings/settings.h"
#include "../graphics/Camera.h"
#include "../spaces/spacemanager.h"
#include <stb_image.h>
#include "../audio/audiomanager.h"
#include "countdowntimer.h"
#include "timermanager.h"


static GLFWwindow* window;
static void fbsize_cb(GLFWwindow *pwin, int width, int height);
static void wclose_cb(GLFWwindow *pwin);

static std::string apppath;
static bool screenshotflag;
static int gamewidth, gameheight;

static bool fullscreen = true;
static bool firstcheck = true;

static GLFWmonitor* monitor;
static const GLFWvidmode * mode;

bool fullscreentext = false;

int resx = 0;
int resy = 0;

void GetMinMonitorResolution(int* w, int* h)
{
	int count;
	const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

	int minWidth = 9999;
	int minHeight = 9999;
	for (int i = 0; i < count; i++)
	{
		if (modes[i].width > 1100)
		{
			if (modes[i].width < minWidth)
			{
				minWidth = modes[i].width;
				minHeight = modes[i].height;
			}
			//if (modes[i].height < minHeight)
		}
	}

	*w = minWidth;
	*h = minHeight;
}

static void window_focus_cb(GLFWwindow * window, int focused)
{
	if (!focused)
	{
		InputEvent e;
		e.action = "keydown";
		e.key = "esc";
		Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &e);
		Audio::aManager->SetSnapshot("Pause");
		Settings::sfxMuted = true;
	}
	else
	{
		Audio::aManager->SetSnapshot("UnPause");
		Settings::sfxMuted = false;
	}
}

/**************************************************************************/
  /*!
	\brief  Initialize the Window (Windows, Event Handlers).
  */
  /**************************************************************************/
void WindowInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	bool startFullscreen = false;
	fullscreen = false;
	fullscreentext = false;

#ifdef _SUBMISSION
	startFullscreen = true;
	fullscreen = true;
	fullscreentext = true;
#endif

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	GetMinMonitorResolution(&resx, &resy);

	if (startFullscreen)
		window = glfwCreateWindow(resx, resy, "I am Gooey", monitor, NULL);
	else
		window = glfwCreateWindow(resx, resy, "I am Gooey", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return;
	}
	glfwSetFramebufferSizeCallback(window, fbsize_cb);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();

	if (startFullscreen)
	{
		//1600 900 miss you 1280?
		fbsize_cb(window, resx, resy);
		glfwSetWindowSize(window, resx, resy);
	}
	else
	{
		fbsize_cb(window, resx, resy);
		glfwSetWindowSize(window, resx, resy);
	}

	/* Initialize IMGUI which needs the window information*/
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(NULL);
	ImGui::StyleColorsDark();
	System::gInfo = new System::gpuinfo();

	glfwSetWindowIconifyCallback(window, Spaces::window_iconify_cb);
	glfwSetWindowCloseCallback(window, wclose_cb);
	glfwSetWindowFocusCallback(window, window_focus_cb);

	screenshotflag = false;

	//glfwSwapBuffers(window);
	const char * icon_name = "./assets/resources/soulstare.png";
	GLFWimage icons[1];
	icons[0].pixels = stbi_load(icon_name, &icons[0].width, &icons[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, icons);
	stbi_image_free(icons[0].pixels);
}


/**************************************************************************/
  /*!
	\brief  Update the Window (Windows, Event Handlers).
	\param dt
		time value
  */
  /**************************************************************************/
void WindowUpdate(float dt)
{
	printf("%d %d \n", resx, resy);
	float xpos, ypos;
	GetCursorPosition(&xpos, &ypos);

	/*if (Settings::debug)
	{
	  printf("%d, %d \n", (int)xpos, (int)ypos);
	}*/
	if (screenshotflag == true)
	{
		SaveScreenshot(apppath);
		screenshotflag = false;
	}

	/* Swap front and back buffers */
	if (firstcheck)
	{
		firstcheck = false;
	}
	else
	{
		glfwSwapBuffers(window);
	}

	/* Poll for and process events */
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
	{
		delete pEngine;
		pEngine = nullptr;
	}
}
void GetCursorPosition(float* x, float* y)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	mainCam.GetPosition()[1];

	if (xpos < 0 || ypos < 0 || xpos > width || ypos > height)
	{
		*x = 9999;
		*y = 9999;
		return;
	}
	int halfw = width / 2;
	int halfh = height / 2;

	float xratio = 1728.0f / (float)gamewidth;
	float yratio = 1120.0f / (float)gameheight;

	if (xpos == halfw && ypos == halfh)
	{
		*x = 0 + mainCam.GetPosition()[0];
		*y = 0 + mainCam.GetPosition()[1];
	}
	else if (xpos < halfw && ypos < halfh)
	{
		*x = -(halfw - (int)xpos) * xratio + mainCam.GetPosition()[0];
		*y = (halfh - (int)ypos) * yratio + mainCam.GetPosition()[1];
	}
	else if (xpos > halfw && ypos < halfh)
	{
		*x = ((int)xpos - halfw) * xratio + mainCam.GetPosition()[0];
		*y = (halfh - (int)ypos) * yratio + mainCam.GetPosition()[1];
	}
	else if (xpos > halfw && ypos > halfh)
	{
		*x = ((int)xpos - halfw) * xratio + mainCam.GetPosition()[0];
		*y = -((int)ypos - halfh) * yratio + mainCam.GetPosition()[1];
	}
	else if (xpos < halfw && ypos > halfh)
	{
		*x = -(halfw - (int)xpos) * xratio + mainCam.GetPosition()[0];
		*y = -((int)ypos - halfh) * yratio + mainCam.GetPosition()[1];
	}

}

/**************************************************************************/
  /*!
	\brief  Shutdown the Window (Windows, Event Handlers).
  */
  /**************************************************************************/
void WindowShutdown()
{
	glfwTerminate();
	if (System::gInfo)
	{
		delete System::gInfo;
		System::gInfo = nullptr;
	}
}

/**************************************************************************/
  /*!
	\brief  Get the pointer to the window
	\return GLFWwindow*
		window pointer
  */
  /**************************************************************************/
GLFWwindow* GetWindow()
{
	return window;
}

const GLFWvidmode* GetMode()
{
	return mode;
}
GLFWmonitor* GetMonitor()
{
	return monitor;
}
bool checkFullscreen()
{
	return fullscreen;
}

static void wclose_cb(GLFWwindow *pwin)
{
	if (quitconfirmflag == 0 && menuconfirmflag == 0)
	{
		quitconfirmflag = 1;
	}
	glfwSetWindowShouldClose(window, GL_FALSE);
}

/**************************************************************************/
  /*!
	\brief  size check call back function (change size w.r.t ratio)
	\param pwin
		pointer to window
	\param width
		width
	\param height
		height
  */
  /**************************************************************************/
static void fbsize_cb(GLFWwindow *pwin, int width, int height) {
	//Aspect Ratio stuff: whit
	float aspectRatio = 1728.0f / 1120.0f;
	int Vx, Vy, Vw, Vh; //View x, y, width, and height (to be calculated acoording to the aspect ratio and the window size)

	float windowRatio = (float)width / (float)height;
	if (fullscreen)
	{
		if (width % 2)
			width--;
		if (height % 2)
			height--;
		glViewport(0, 0, width, height);
		gamewidth = width;
		gameheight = height;
		return;
	}
	if (aspectRatio < windowRatio)//Window is wider than the viewport
	{
		Vh = height; //The heights are the same, since the width is the difference
		Vy = 0;      //Same logic applies to the y position

		Vw = (int)(Vh * aspectRatio); //The width is the height multiplied by the ratio
		Vx = (width - Vw) / 2; //The X position is the difference between the widths / 2

		if (Vw % 2)
			Vw--;
		if (Vh % 2)
			Vh--;

		glViewport(Vx, Vy, Vw, Vh);
		gamewidth = Vw;
		gameheight = Vh;
	}
	else if (aspectRatio > windowRatio)//Window is taller than the viewport
	{
		Vw = width; //The widths are the same, since the height is the difference
		Vx = 0;     //Same logic applies to the x position

		Vh = (int)(Vw / aspectRatio);
		Vy = (height - Vh) / 2;

		if (Vw % 2)
			Vw--;
		if (Vh % 2)
			Vh--;

		glViewport(Vx, Vy, Vw, Vh);
		gamewidth = Vw;
		gameheight = Vh;
	}
	else//Window ratio matches aspect ratio
	{
		if (width % 2)
			width--;
		if (height % 2)
			height--;
		glViewport(0, 0, width, height);
		gamewidth = width;
		gameheight = height;
	}

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


bool SaveScreenshot(std::string filename)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w * h * 3;

	// First let's create our buffer, 3 channels per Pixel
	char* dataBuffer = (char*)malloc(nSize * sizeof(char));

	if (!dataBuffer) return false;

	// Let's fetch them from the backbuffer	
	glReadPixels((GLint)0, (GLint)0,
		(GLint)w, (GLint)h,
		GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);

	//Now the file creation
	FILE *filePtr = fopen(filename.c_str(), "wb");
	if (!filePtr) return false;


	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6] = { (unsigned char)(w % 256), (unsigned char)(w / 256),
							   (unsigned char)(h % 256), (unsigned char)(h / 256), (unsigned char)24, (unsigned char)0 };
	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);

	free(dataBuffer);

	return true;
}

void MakeScreenshot(std::string filename)
{
	if (screenshotflag == false)
	{
		apppath = filename;
		screenshotflag = true;
	}
}
void ToggleFullScreen()
{
	if (fullscreen)
	{
		fullscreen = false;
		fullscreentext = false;
		glfwSetWindowMonitor(window, fullscreen ? monitor : NULL, 100, 100, resx, resy, GLFW_DONT_CARE);
		fbsize_cb(window, resx, resy);
		//glfwSetWindowSize(window, 1024, 768);
	}
	else
	{
		fullscreen = true;
		fullscreentext = true;
		glfwSetWindowMonitor(window, fullscreen ? monitor : NULL, 0, 0, resx, resy, GLFW_DONT_CARE); //1728 1120
		fbsize_cb(window, resx, resy);
		//glfwSetWindowSize(window, 1024, 768);
		//glfwSetWindowSize(window, 800, 600);
		//1600 900, 1366 768
	}

}