/******************************************************************************/
/*!
\file   Sprite.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Sprite class, which handle drawing object through component using Render class

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Sprite.h"
#include "SpriteOrder.h"
#include <iostream>
#include "../gameobject/gameobject.h"
#include "../physics/Transform.h"
#include "../settings/settings.h"
#include "../vfx/color.h"
#include "../vfx/LightReciever.h"
#include "../graphics/Tilemap.h"
#include "Shader.h"
#include "../component/controller.h"

#define COLLIDER_TEST


/**************************************************************************/
  /*!
	\brief Default constructor
  */
  /**************************************************************************/
Sprite::Sprite()
{
	rdata = 0;
	rrate = 0;
	rmap[0] = 0;
	rmap[1] = 0;
	prior = 0;
	alpha = 1.0f;
	ignoreculling = false;
}

/**************************************************************************/
/*!
	\brief Initializer
	\param shader
	  shader id for sprite. 99 = defualt
*/
/**************************************************************************/
void Sprite::Init(int shader)
{
	if (shader != 99)
	{
		//render = new Render(shader); // test
		sID = shader;
	}

	//render->loadMesh(0);

#ifdef COLLIDER_TEST
  transform = owner->GetComponent<Transform>();
#endif
}

/**************************************************************************/
/*!
	\brief Destructor
*/
/**************************************************************************/
Sprite::~Sprite()
{
	//render->unloadMesh();
	//delete render;
}
/**************************************************************************/
/*!
	\brief link texture buffer with id
	\param id
	  texture buffer id for sprite
*/
/**************************************************************************/
void Sprite::SetTexture(const int id)
{
	//render->loadTexture(id);
	tID = id;
}
/**************************************************************************/
/*!
	\brief get texture buffer
	\return int
	  texture buffer variable
*/
/**************************************************************************/
int Sprite::GetTexture()
{
	return tID;
	//return render->getTexture();
}
/**************************************************************************/
/*!
	\brief set position of the sprite
	\param x
	  x coordinate
	\param y
	  y coordinate
*/
/**************************************************************************/
void Sprite::SetPosition(float x, float y)
{
#ifdef COLLIDER_TEST
  if(transform)
  {
    transform->Translation(Vector2D(x,y));
  }
#endif
	rcenter.x = x;
	rcenter.y = y;
}
/**************************************************************************/
/*!
	\brief get position of the sprite
	\return Point
	  position of the sprite
*/
/**************************************************************************/
glm::vec2 Sprite::GetPosition()
{
	return rcenter;
}
/**************************************************************************/
/*!
	\brief get shader program id
	\return int
	  shader program id
*/
/**************************************************************************/
int Sprite::GetShader()
{
	return sID;
	//return render->getShader();
}
/**************************************************************************/
/*!
	\brief set sprite size
	\param x
	  width
	\param y
	  height
*/
/**************************************************************************/
void Sprite::SetSize(float x, float y)
{
#ifdef COLLIDER_TEST
  if(transform)
  {
    transform->Scale(Vector2D(x,y));
  }
#endif
	rsize.x = x;
	rsize.y = y;
}
/**************************************************************************/
/*!
	\brief get size of the sprite
	\return Point
	  Point.x = width   Point.y = height
*/
/**************************************************************************/
glm::vec2 Sprite::GetSize()
{
	return rsize;
}
/**************************************************************************/
/*!
	\brief set sprite's camera
	\param camera
	  camera value
*/
/**************************************************************************/
void Sprite::SetCamera(Camera camera)
{
	rcamera = camera;
}
/**************************************************************************/
/*!
	\brief set sprite rotation
	\param rate
	  rotation rate
*/
/**************************************************************************/
void Sprite::SetRotation(float rate)
{
	rrate = rate;
}
/**************************************************************************/
/*!
	\brief get rotation rate
	\return float
	  rotation rate
*/
/**************************************************************************/
float Sprite::GetRotation()
{
	return rrate;
}
/**************************************************************************/
/*!
	\brief set shader from shader array
	\param id
	  order of shader array
*/
/**************************************************************************/
void Sprite::setShader(int id)
{
	sID = id;
	//render->setShader(id);
}
/**************************************************************************/
/*!
	\brief common update function
	\param dt
	  time value
*/
/**************************************************************************/
void Sprite::Update(float dt)
{

	//int dimX = (int)((transform->Translation()[0] + 832.0f) / 1664.0f);
	//int dimY = (int)((transform->Translation()[1] - 512.0f) / 1024.0f);
	//int dimX = (int)((rcenter.x + 832.0f) / 1664.0f);
	//int dimY = (int)((rcenter.y - 512.0f) / 1024.0f);

	//float disX = abs(rcenter.x - mainCam.center().x);
	//float disY = abs(rcenter.y - mainCam.center().y);

	if (!transform || transform->isDirty())
	{
		transform = owner->GetComponent<Transform>();
	}

	if (transform)
	{
		rcenter.x = transform->Translation()[0];
		rcenter.y = transform->Translation()[1];
		rrate = transform->Rotation();
		rsize.x = transform->Scale()[0];
		rsize.y = transform->Scale()[1];
	}

	//Some things will be culled
	if (Settings::culling && ignoreculling == false)
	{
		//It should appear
		//if (disX <= 896.0f && disY <= 576.0f)
		//{
		Animation* anim = owner->GetComponent<Animation>();
		LightReciever* LR = owner->GetComponent<LightReciever>();
		Color* color = owner->GetComponent<Color>();

		int sequence = 0;
		if (anim)
		{
			sequence = anim->getSequenceIndex() + 1;
		}

		if (prior == 3)
		{

		}
		else if (sequence == 21 || sequence == 22 || sequence == 23 || sequence == 24 || sequence == 37 || sequence == 38 || sequence == 39 || sequence == 40 || sequence == 4 || sequence == 5)
		{

		}
		else
		{
			if (trailmap[rmap[0]][rmap[1]].status != 0)
			{
				LR->setDoLava(1);
				if (trailmap[rmap[0]][rmap[1]].status > 5)
				{
					anim->setSequence(80 + (trailmap[rmap[0]][rmap[1]].status - 5));
				}
				else
				{
					anim->setSequence(64 + trailmap[rmap[0]][rmap[1]].status);
				}
			}
		}
		int newprior = prior;

		AddSprite(newprior, sID, 0, tID,
			rcenter, rsize, rrate, alpha, anim, LR, color, owner);
		//}
	}
	else //Everything is rendering
	{
		Animation* anim = owner->GetComponent<Animation>();
		LightReciever* LR = owner->GetComponent<LightReciever>();
		Color* color = owner->GetComponent<Color>();

		int sequence = 0;
		if (anim)
		{
			sequence = anim->getSequenceIndex() + 1;
		}
		//if (sequence == 21 || sequence == 22 || sequence == 37 || sequence == 38 || sequence == 4 || sequence == 26 || sequence == 42)
		if(prior == 3)
		{

		}
		else if (sequence == 21 || sequence == 22 || sequence == 23 || sequence == 24 || sequence == 37 || sequence == 38 || sequence == 39 || sequence == 40 || sequence == 4 ||sequence == 5)
		{

		}
		else
		{
			if (trailmap[rmap[0]][rmap[1]].status != 0)
			{
				LR->setDoLava(1);
				if (trailmap[rmap[0]][rmap[1]].status > 5)
				{
					anim->setSequence(80 + (trailmap[rmap[0]][rmap[1]].status - 5));
				}
				else
				{
					anim->setSequence(64 + trailmap[rmap[0]][rmap[1]].status);
				}
			}
		}
		AddSprite(prior, sID, 0, tID,
			rcenter, rsize, rrate, alpha, anim, LR, color, owner);
	}
}
/**************************************************************************/
/*!
	\brief common draw function
*/
/**************************************************************************/
void Sprite::Draw()const
{
}

/**************************************************************************/
/*!
	\brief return alpha value of the sprite
	\return float
	  sprite's alpha value
*/
/**************************************************************************/
float Sprite::getAlpha()
{ 
	return alpha;

	//return render->getAlpha(); 
}

/**************************************************************************/
/*!
	\brief set sprite's alpha value
	\param zeroToOne
	  sprite's alpha value
*/
/**************************************************************************/
void Sprite::setAlpha(float zeroToOne)
{
	if (zeroToOne < 0)
		zeroToOne = 0;
	if (zeroToOne > 1)
		zeroToOne = 1;
	//render->setAlpha(zeroToOne);
	alpha = zeroToOne;
}
/**************************************************************************/
/*!
	\brief set sprite's data if it's tile
	\param data
	  tile data
*/
/**************************************************************************/
void Sprite::setData(int data)
{
	rdata = data;
}
/**************************************************************************/
/*!
	\brief return sprite's data if its tile
	\return int
	  sprite's tile data
*/
/**************************************************************************/
int Sprite::getData()
{
	return rdata;
}


void Sprite::setMap(int mapx, int mapy)
{
	rmap[0] = mapx;
	rmap[1] = mapy;
}

int* Sprite::getMap()
{
	return rmap;
}

void Sprite::setPrior(int pri)
{
	prior = pri;
}
int Sprite::getPrior()
{
	return prior;
}

void Sprite::ResetSequence()
{
	if (origin != 0)
	{
			LightReciever* LR = owner->GetComponent<LightReciever>();
			LR->setDoLava(0);
		Animation* anim = owner->GetComponent<Animation>();
		if (anim)
		{
			anim->setSequence(origin);
		}
	}
}