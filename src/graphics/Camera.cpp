/******************************************************************************/
/*!
\file   Camera.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Camera class, which handles movement and others for using camera matrix.

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Camera.h"
#include "../settings/settings.h"
#include <glm/gtx/rotate_vector.hpp>

Camera mainCam(glm::vec2(0, 0), glm::vec2(0, 1), 1728.0f, 1120.0f);

/**************************************************************************/
  /*!
	\brief Default constructor
  */
  /**************************************************************************/
Camera::Camera(void)
{
	//original value initialize
	rect_width = 2.0f;
	rect_height = 2.0f;

	right_vector.x = 1;
	up_vector.y = 1;
}
/**************************************************************************/
/*!
	\brief Destructor
*/
/**************************************************************************/
Camera::~Camera(void)
{
}

/**************************************************************************/
  /*!
	\brief initializing constructor
	\param C
		center of camera
	\param v
		up vector of camera
	\param W
		camera width
	\param H
		camera height
  */
  /**************************************************************************/
Camera::Camera(const glm::vec2& C, const glm::vec2& v, float W, float H)
{
	center_point = C;

	up_vector = v;

	// -90 degree = -1.5708f
	// u = r(-90degree)*v;
	if (v.x == 0)
	{
		right_vector.x = up_vector.y;
		right_vector.y = up_vector.x;
	}
	else
	{
		right_vector = glm::rotate(v, -1.5708f);
	}

	rect_width = W;
	rect_height = H;
}

void Camera::Update(float dt)//Currently called in Engine Update
{
	//Camera shake
	if (shakeDuration > 0)
	{
		static glm::vec2 tempPos;
		shakeDuration -= dt;
		if (shakeBegan)//If shaking just started
		{
			//Set began to false
			shakeBegan = false;
			//Store the position the camera was on
			tempPos = center_point;
		}
		//Do shake
		float rand1 = ((float)rand() / RAND_MAX) - .5f;
		float rand2 = ((float)rand() / RAND_MAX) - .5f;
		float intensity = .0005f; //shake intensity
		moveRight(rand1 * intensity);
		moveUp(rand2 * intensity);

		//(center_point.x + rand1 * intensity, center_point.y + rand2 * intensity);

		if (shakeDuration <= 0)//If it's done
		{
			//Go back to the center point
			center_point = tempPos;
		}
	}
}

/**************************************************************************/
/*!
	\brief get the camera center coordiate
	\return Point
		return coordiante(center of the camera)
*/
/**************************************************************************/
glm::vec2 Camera::center(void) const
{
	return center_point;
}

/**************************************************************************/
/*!
	\brief get the camera right vector
	\return Vector
		return right vector of camera
*/
/**************************************************************************/
glm::vec2 Camera::right(void) const
{
	return right_vector;
}

/**************************************************************************/
/*!
	\brief get the camera up vector
	\return Vector
		return up vector of camera
*/
/**************************************************************************/
glm::vec2 Camera::up(void) const
{
	return up_vector;
}

/**************************************************************************/
/*!
	\brief get the camera width
	\return float
		return float value width of camera
*/
/**************************************************************************/
float Camera::width(void) const
{
	return rect_width;
}

/**************************************************************************/
/*!
	\brief get the camera height
	\return float
		return float value height of camera
*/
/**************************************************************************/
float Camera::height(void) const
{
	return rect_height;
}

/**************************************************************************/
/*!
	\brief movement function for camera
	\param x
		distance for movement
	\return Camera&
		return moved camera pointer
*/
/**************************************************************************/
Camera& Camera::moveRight(float x)
{
	//c<-c+xu
	center_point = center_point + x * right_vector;
	return *this;
}

/**************************************************************************/
/*!
	\brief movement function for camera
	\param y
		distance for movement
	\return Camera&
		return moved camera pointer
*/
/**************************************************************************/
Camera& Camera::moveUp(float y)
{
	//c<-c+xv
	center_point = center_point + y * up_vector;
	return *this;
}

/**************************************************************************/
/*!
	\brief movement function for camera
	\param t
		rotation for movement
	\return Camera&
		return rotated camera pointer
*/
/**************************************************************************/
Camera& Camera::rotate(float t)
{
	//u<-r(t)*u
	//v<-r(t)*v
	right_vector = glm::rotate(right_vector, t);
	up_vector = glm::rotate(up_vector, t);
	return *this;
}

/**************************************************************************/
/*!
	\brief movement function for camera
	\param f
		zoom late for movement
	\return Camera&
		return zoomed camera pointer
*/
/**************************************************************************/
Camera& Camera::zoom(float f)
{
	//W<-fW
	//W<-fH
	rect_width = f * rect_width;
	rect_height = f * rect_height;
	return *this;
}


/**************************************************************************/
/*!
	\brief change position of camera
	\param x
		new x coordinate
	\param y
		new y coordinate
*/
/**************************************************************************/
void Camera::SetPosition(float x, float y)
{
	center_point.x = x;
	center_point.y = y;
}
float* Camera::GetPosition()
{
	float pos[2];
	pos[0] = center_point.x;
	pos[1] = center_point.y;
	return pos;
}

/**************************************************************************/
/*!
	\brief change rotation of camera
	\param r
		new rotation rate
*/
/**************************************************************************/
void Camera::SetRotation(float r)
{
	right_vector = glm ::rotate(glm::vec2(1, 0), r);
	up_vector = glm::rotate(glm::vec2(0, 1), r);
}
/**************************************************************************/
/*!
	\brief change zoom of camera
	\param f
		new zoom late
*/
/**************************************************************************/
void Camera::SetZoom(float f)
{
	rect_width = f * 1728.0f;
	rect_height = f * 1120.0f;
}

/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from camera to world
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 cameraToWorld(const Camera& cam)
{
	//Mc = [u|v|c]
	glm::mat3 L = {};

	L[0][0] = cam.right().x;
	L[0][1] = cam.right().y;
	L[0][2] = 0;
	L[1][0] = cam.up().x;
	L[1][1] = cam.up().y;
	L[1][2] = 0;
	L[2][0] = cam.center().x;
	L[2][1] = cam.center().y;
	L[2][2] = 1;

	return L;
}

/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from world to camera
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 worldToCamera(const Camera& cam)
{
	//V = Mc^-1
	glm::mat3 L = glm::inverse(cameraToWorld(cam));
	return L;
}

/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from camera to NDC
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 cameraToNDC(const Camera& cam)
{
	glm::mat3 L = {};
	L[0][0] = 2.0f / cam.width();
	L[1][1] = 2.0f / cam.height();
	L[2][2] = 1;

	return L;
}
/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from NDC to camera
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 NDCToCamera(const Camera& cam)
{
	glm::mat3 L = {};
	L[0][0] = cam.width() / 2.0f;
	L[1][1] = cam.height() / 2.0f;
	L[2][2] = 1;
	return L;
}

/**************************************************************************/
/*!
	\brief check player's movement and position
	\param dimX
		player's map dimension x coordinate
	\param dimY
		player's map dimension x coordinate
	\return bool
		return true if player is in camera
*/
/**************************************************************************/
bool checkPM(float dimX, float dimY)
{
	if (Settings::camfollowing)
	{
		if ((((float)dimX*1728.0f) - mainCam.center().x) == 0 && (((float)dimY*1088.0f) - mainCam.center().y) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
	
}
/**************************************************************************/
/*!
	\brief camera smooth scroll movement function depend on player
	\param dimX
		player's map dimension x coordinate
	\param dimY
		player's map dimension x coordinate
*/
/**************************************************************************/
void camScroll(float dimX, float dimY)
{
	
	if (Settings::camfollowing)
	{
		
		//check earthquake
		if ((((float)dimX*1728.0f) - mainCam.center().x) > 0 && (((float)dimX*1728.0f) - mainCam.center().x) < 26)
		{
			mainCam.SetPosition((float)dimX*1728.0f, mainCam.center().y);
		}
		if ((((float)dimY*1088.0f) - mainCam.center().y) > 0 && (((float)dimY*1088.0f) - mainCam.center().y) < 16)
		{
			mainCam.SetPosition(mainCam.center().x, (float)dimY*1088.0f);
		}


		if ((((float)dimX*1728.0f) - mainCam.center().x) != 0)
		{
			if ((((float)dimX*1728.0f) - mainCam.center().x) > 0)
			{
				//mainCam.moveRight(26.0f);
				mainCam.SetPosition(mainCam.center().x + 32.0f, mainCam.center().y);
			}
			else
			{
				mainCam.SetPosition(mainCam.center().x - 32.0f, mainCam.center().y);
			}
		}
		if ((((float)dimY*1088.0f) - mainCam.center().y) != 0)
		{
			if ((((float)dimY*1088.0f) - mainCam.center().y) > 0)
			{
				//mainCam.moveUp(16.0f);
				mainCam.SetPosition(mainCam.center().x, mainCam.center().y + 16.0f);
			}
			else
			{
				//mainCam.moveUp(-16.0f);
				mainCam.SetPosition(mainCam.center().x, mainCam.center().y - 16.0f);
			}
		}
	}
}