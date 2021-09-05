/******************************************************************************/
/*!
\file   DebugDraw.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
DebugDraw class, which handles drawing debug line

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <iostream>
#include "DebugDraw.h"
#include "Shader.h"
#include "../gameobject/gameobject.h"
#include "../physics/Transform.h"
#include "../physics/Physics.h"
#include "../settings/settings.h"
#include "../system/common_defines.h"
#include "SpriteOrder.h"

#define COLLIDER_TEST


/**************************************************************************/
/*!
	\brief Default constructor
*/
/**************************************************************************/
DebugDraw::DebugDraw()
{
	sID = Shader_BasicColor;
	//render = new Render(Shader_BasicColor); // test
	//render->loadMesh(0);
	changecolor = false;
}
/**************************************************************************/
/*!
	\brief Destructor
*/
/**************************************************************************/
DebugDraw::~DebugDraw()
{
	//render->unloadMesh();
	//delete render;
}

/**************************************************************************/
/*!
	\brief Initializer
*/
/**************************************************************************/
void DebugDraw::Init()
{
	sprite = owner->GetComponent<Sprite>();
	collider = owner->GetComponent<Collider>();
	rcenter.x = sprite->GetPosition().x;
	rcenter.y = sprite->GetPosition().y;

	rsize.x = sprite->GetSize().x;
	rsize.y = sprite->GetSize().y;

	rrate = sprite->GetRotation();

	rcolor = glm::vec3(1.0f, 1.0f, 1.0f);

	mode = 0;
}
/**************************************************************************/
  /*!
	\brief common Update function
	\param dt
		time value
  */
  /**************************************************************************/
void DebugDraw::Update(float dt)
{
	/*
	float disX = abs(rcenter.x - mainCam.center().x);
	float disY = abs(rcenter.y - mainCam.center().y);

	if (Settings::culling)
	{
		if (disX <= 896.0f && disY <= 576.0f)
	*/
#ifdef _SUBMISSION
	if (Settings::debugLines)
#else
	if (Settings::debug)
#endif // _SUBMISSION
	{
		if (collider)
		{
			if (mode == 1)
			{
				rcenter.x = sprite->GetPosition().x;
				rcenter.y = sprite->GetPosition().y;

				rsize.x = sprite->GetSize().x;
				rsize.y = sprite->GetSize().y;

				rrate = sprite->GetRotation();
			}
			if (mode == 0)
			{
				rcenter.x = collider->Center()[0];
				rcenter.y = collider->Center()[1];

				rsize.x = collider->Radius() * 2;
				rsize.y = collider->Radius() * 2;

				rrate = sprite->GetRotation();

			}
			if (mode == 2)
			{
				Transform* transform = owner->GetComponent<Transform>();
				Physics* physics = owner->GetComponent<Physics>();
				float dt = 0.0167f;

				float x = physics->Velocity()[0] * dt * 10;
				float y = physics->Velocity()[1] * dt * 10;

				rcenter.x = sprite->GetPosition().x;
				rcenter.y = sprite->GetPosition().y;


				if (x && !y)
				{
					rsize.x = sprite->GetSize().x * x;
					rsize.y = sprite->GetSize().y * x;

					rrate = sprite->GetRotation();
				}
				else if (!x && y)
				{
					rsize.x = sprite->GetSize().x * y;
					rsize.y = sprite->GetSize().y * y;

					rrate = sprite->GetRotation() + 1.5708f;
				}
				else
				{
					rsize.x = sprite->GetSize().x * x;
					rsize.y = sprite->GetSize().y * x;
				}
			}
		}
		//render->setRotate(rrate);
		//render->setScale(rsize.x, rsize.y);
		//render->setTransform(rcenter.x, rcenter.y);

		//render->setModelToWorld(mainCam);

		//glUseProgram(render->getShader());
		if (changecolor == true)
		{
			AddLine(sID, mID, rcenter, rsize, rrate, tempcolor);
			//render->displayEdges(tempcolor);
			changecolor = false;
		}
		else
		{
			AddLine(sID, mID, rcenter, rsize, rrate, rcolor);
		}
	}
}
/**************************************************************************/
  /*!
	\brief common Draw function
  */
  /**************************************************************************/
void DebugDraw::Draw()const
{
}

/**************************************************************************/
  /*!
	\brief set position of debug sprite
	\param x
		x coorinate
	\param y
		y coorinate
  */
  /**************************************************************************/
void DebugDraw::SetPosition(float x, float y)
{
	rcenter.x = x;
	rcenter.y = y;
}
/**************************************************************************/
  /*!
	\brief set size of sprite
	\param x
		width
	\param y
		height
  */
  /**************************************************************************/
void DebugDraw::SetSize(float x, float y)
{
	rsize.x = x;
	rsize.y = y;
}
/**************************************************************************/
  /*!
	\brief set rotation of debug sprite
	\param x
		rotation rate
  */
  /**************************************************************************/
void DebugDraw::SetRotation(float x)
{
	rrate = x;
}
/**************************************************************************/
  /*!
	\brief set debug mode
	\param m
		mode id
  */
  /**************************************************************************/
void DebugDraw::SetMode(int m)
{
	mode = m;
	if (m == 2)
	{
		mID = 1;
		//render->loadMesh(1);
	}
	else
	{
		mID = 0;
		//render->loadMesh(0);
	}
}
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
void DebugDraw::SetTempColor(float a, float b, float c)
{
	tempcolor.x = a;
	tempcolor.y = b;
	tempcolor.z = c;
	changecolor = true;
}
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
void DebugDraw::SetOriginalColor(float a, float b, float c)
{

	rcolor.x = a;
	rcolor.y = b;
	rcolor.z = c;
}
/**************************************************************************/
  /*!
	\brief return mode id

	\return int
		return mode
  */
  /**************************************************************************/
int DebugDraw::GetMode()
{
	return mode;
}