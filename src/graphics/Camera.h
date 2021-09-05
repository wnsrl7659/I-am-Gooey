/******************************************************************************/
/*!
\file   Camera.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Camera class, which handles movement and others for using camera matrix.

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "../graphics/Mesh.h"


class Camera {
public:
	/**************************************************************************/
	  /*!
		\brief Default constructor
	  */
	  /**************************************************************************/
	Camera(void);
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
	Camera(const glm::vec2& C, const glm::vec2& v, float W, float H);

	/**************************************************************************/
	/*!
		\brief Destructor
	*/
	/**************************************************************************/
	~Camera(void);
	void Update(float dt);

	/**************************************************************************/
	/*!
		\brief get the camera center coordiate
		\return Point
			return coordiante(center of the camera)
	*/
	/**************************************************************************/
	glm::vec2 center(void) const;
	/**************************************************************************/
	/*!
		\brief get the camera right vector 
		\return Vector
			return right vector of camera
	*/
	/**************************************************************************/
	glm::vec2 right(void) const;
	/**************************************************************************/
	/*!
		\brief get the camera up vector
		\return Vector
			return up vector of camera
	*/
	/**************************************************************************/
	glm::vec2 up(void) const;
	/**************************************************************************/
	/*!
		\brief get the camera width
		\return float
			return float value width of camera
	*/
	/**************************************************************************/
	float width(void) const;
	/**************************************************************************/
	/*!
		\brief get the camera height
		\return float
			return float value height of camera
	*/
	/**************************************************************************/
	float height(void) const;
	/**************************************************************************/
	/*!
		\brief movement function for camera
		\param x
			distance for movement
		\return Camera&
			return moved camera pointer 
	*/
	/**************************************************************************/
	Camera& moveRight(float x);
	/**************************************************************************/
	/*!
		\brief movement function for camera
		\param y
			distance for movement
		\return Camera&
			return moved camera pointer
	*/
	/**************************************************************************/
	Camera& moveUp(float y);
	/**************************************************************************/
	/*!
		\brief movement function for camera
		\param t
			rotation for movement
		\return Camera&
			return rotated camera pointer
	*/
	/**************************************************************************/
	Camera& rotate(float t);
	/**************************************************************************/
	/*!
		\brief movement function for camera
		\param f
			zoom late for movement
		\return Camera&
			return zoomed camera pointer
	*/
	/**************************************************************************/
	Camera& zoom(float f);
	/**************************************************************************/
	/*!
		\brief change position of camera
		\param x
			new x coordinate
		\param y
			new y coordinate
	*/
	/**************************************************************************/
	void SetPosition(float x, float y);
	float* GetPosition();
	/**************************************************************************/
	/*!
		\brief change rotation of camera
		\param r
			new rotation rate
	*/
	/**************************************************************************/
	void SetRotation(float r);
	/**************************************************************************/
	/*!
		\brief change zoom of camera
		\param f
			new zoom late
	*/
	/**************************************************************************/
	void SetZoom(float f);
	void SetShake(float f) { shakeBegan = true; shakeDuration = f; }
private:
	float shakeDuration; //Camera shakes when this number > 0
	bool shakeBegan; //Is true when camera begins shaking, gets set to false right away

	float rect_width;
	float rect_height;

	glm::vec2 right_vector;
	glm::vec2 up_vector;

	glm::vec2 center_point;
};

/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from camera to world
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 cameraToWorld(const Camera& cam);
/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from world to camera
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 worldToCamera(const Camera& cam);
/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from camera to NDC
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 cameraToNDC(const Camera& cam);
/**************************************************************************/
/*!
	\brief make 3x3 affine matrix from NDC to camera
	\param cam
		camera pointer
	\return Affine
		return 3x3 affine matrix
*/
/**************************************************************************/
glm::mat3 NDCToCamera(const Camera& cam);

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
bool checkPM(float dimX, float dimY);
/**************************************************************************/
/*!
	\brief camera smooth scroll movement function depend on player
	\param dimX
		player's map dimension x coordinate
	\param dimY
		player's map dimension x coordinate
*/
/**************************************************************************/
void camScroll(float dimX, float dimY);

extern Camera mainCam;

#endif

