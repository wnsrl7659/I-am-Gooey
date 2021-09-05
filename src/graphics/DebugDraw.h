/******************************************************************************/
/*!
\file   DebugDraw.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
DebugDraw class, which handles drawing debug line

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "../system/Window.h"
#include "../component/component.h"
#include "../physics/Transform.h"
#include "../component/collider.h"
#include "Render.h"
#include "Camera.h"
 
#include "Sprite.h"
#include "Mesh.h"


class DebugDraw : public Component<DebugDraw>
{
public:
	COMPONENT_NAME(DebugDraw);
	/**************************************************************************/
	  /*!
		\brief Default constructor
	  */
	  /**************************************************************************/
	DebugDraw();

	/**************************************************************************/
	/*!
		\brief Destructor
	*/
	/**************************************************************************/
	~DebugDraw();

	/**************************************************************************/
	/*!
		\brief Initializer
	*/
	/**************************************************************************/
	void Init();
	/**************************************************************************/
	  /*!
		\brief common Update function
		\param dt
			time value
	  */
	/**************************************************************************/
	virtual void Update(float dt);
	/**************************************************************************/
	  /*!
		\brief common Draw function
	  */
	/**************************************************************************/
	virtual void Draw()const;
	/**************************************************************************/
	  /*!
		\brief set position of debug sprite
		\param x
			x coorinate
		\param y
			y coorinate
	  */
	/**************************************************************************/
	void SetPosition(float x, float y);
	/**************************************************************************/
	  /*!
		\brief set size of sprite
		\param x
			width
		\param y
			height
	  */
	  /**************************************************************************/
	void SetSize(float x, float y);
	/**************************************************************************/
	  /*!
		\brief set rotation of debug sprite
		\param x
			rotation rate
	  */
	  /**************************************************************************/
	void SetRotation(float x);
	/**************************************************************************/
	  /*!
		\brief set debug mode
		\param m
			mode id
	  */
	  /**************************************************************************/
	void SetMode(int m);
	/**************************************************************************/
	  /*!
		\brief set temporary color of sprite
		\param a
			color R value
		\param b
			color G value
		\param c
			color B value
	  */
	  /**************************************************************************/
	void SetTempColor(float a, float b, float c);
	/**************************************************************************/
	  /*!
		\brief set basic color of sprite
		\param a
			color R value
		\param b
			color G value
		\param c
			color B value
	  */
	  /**************************************************************************/
	void SetOriginalColor(float a, float b, float c);
	/**************************************************************************/
	  /*!
		\brief return mode id 

		\return int
			return mode 
	  */
	  /**************************************************************************/
	int GetMode();
private:
	Render* render;
	glm::vec2 rcenter;
	glm::vec2 rsize;
	float rrate;
	Sprite* sprite;
	Collider* collider;
	glm::vec3 rcolor;
	glm::vec3 tempcolor;
	bool changecolor;
	int mode;
	int sID = 0;
	int mID = 0;
};

#endif