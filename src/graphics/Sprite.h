/******************************************************************************/
/*!
\file   Sprite.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Sprite class, which handle drawing object through component using Render class

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef SPRITE_H
#define SPRITE_H

#include "../component/component.h"
#include "../system/Window.h"
#include "../physics/Transform.h"
#include "Render.h"
#include "Camera.h"
#include "Mesh.h"
 


class Sprite : public Component<Sprite>
{
public:
	COMPONENT_NAME(Sprite);
	/**************************************************************************/
	  /*!
		\brief Default constructor
	  */
	  /**************************************************************************/
	Sprite();

	/**************************************************************************/
	/*!
		\brief Destructor
	*/
	/**************************************************************************/
	~Sprite();
	/**************************************************************************/
	/*!
		\brief Initializer
		\param shader
		  shader id for sprite. 99 = defualt
	*/
	/**************************************************************************/
	void Init(int shader = 99);

	/**************************************************************************/
	/*!
		\brief link texture buffer with id
		\param id
		  texture buffer id for sprite
	*/
	/**************************************************************************/
	void SetTexture(const int id);
	/**************************************************************************/
	/*!
		\brief set position of the sprite
		\param x
		  x coordinate
		\param y
		  y coordinate
	*/
	/**************************************************************************/
	void SetPosition(float x, float y);
	/**************************************************************************/
	/*!
		\brief get position of the sprite
		\return Point
		  position of the sprite
	*/
	/**************************************************************************/
	glm::vec2 GetPosition();
	/**************************************************************************/
	/*!
		\brief get shader program id
		\return int
		  shader program id
	*/
	/**************************************************************************/
	int GetShader();
	/**************************************************************************/
	/*!
		\brief get texture buffer 
		\return int
		  texture buffer variable
	*/
	/**************************************************************************/
	int GetTexture();
	/**************************************************************************/
	/*!
		\brief get size of the sprite
		\return Point
		  Point.x = width   Point.y = height
	*/
	/**************************************************************************/
	glm::vec2 GetSize();
	/**************************************************************************/
	/*!
		\brief get rotation rate
		\return float
		  rotation rate
	*/
	/**************************************************************************/
	float GetRotation();
	/**************************************************************************/
	/*!
		\brief set sprite size
		\param x
		  width
		\param y
		  height
	*/
	/**************************************************************************/
	void SetSize(float x, float y);
	/**************************************************************************/
	/*!
		\brief set sprite's camera
		\param camera
		  camera value
	*/
	/**************************************************************************/
	void SetCamera(Camera camera);
	/**************************************************************************/
	/*!
		\brief set sprite rotation
		\param rate
		  rotation rate
	*/
	/**************************************************************************/
	void SetRotation(float rate);
	/**************************************************************************/
	/*!
		\brief common update function
		\param dt
		  time value
	*/
	/**************************************************************************/
	void Update(float dt);
	/**************************************************************************/
	/*!
		\brief common draw function
	*/
	/**************************************************************************/
	void Draw()const;
	/**************************************************************************/
	/*!
		\brief return alpha value of the sprite
		\return float
		  sprite's alpha value
	*/
	/**************************************************************************/
	float getAlpha();
	/**************************************************************************/
	/*!
		\brief set sprite's alpha value
		\param zeroToOne
		  sprite's alpha value
	*/
	/**************************************************************************/
	void setAlpha(float zeroToOne);
	/**************************************************************************/
	/*!
		\brief set sprite's data if it's tile
		\param data
		  tile data
	*/
	/**************************************************************************/
	void setData(int data);
	/**************************************************************************/
	/*!
		\brief set shader from shader array
		\param id
		  order of shader array
	*/
	/**************************************************************************/
	void setShader(int id);
	/**************************************************************************/
	/*!
		\brief return sprite's data if its tile
		\return int
		  sprite's tile data
	*/
	/**************************************************************************/
	int getData();
	/**************************************************************************/
	/*!
		\brief return sprite's data if its tile
		\return int
			sprite's tile data
	*/
	/**************************************************************************/
	void setMap(int mapx, int mapy);
	int* getMap();

	void setPrior(int pri);
	int getPrior();

	void IgnoreCulling()
	{
		ignoreculling = true;
	}

	int getShaderIndex()
	{
		return sID;
			//return render->getShaderIndex();
	}
	void checkTorch(bool temp)
	{
		isTorch = temp;
	}
	bool getTorch()
	{
		return isTorch;
	}
	void SetOriginSequence(int i)
	{
		origin = i;
	}
	void ResetSequence();

private:
	Render* render;
	glm::vec2 rcenter;
	glm::vec2 rsize;
	float rrate;
	Camera rcamera;
	int rdata;
	int rmap[2];
  Transform * transform;  /// Chyler: I recommend using a transform instead of individual values for center, size, etc.
                          ///         You don't need to use a pointer like I added, I needed that to test colliders
  Transform oldTransform;

  int tID = 0; // texture ID
  GLuint sID = 0;
  int prior;
  float alpha;
  bool ignoreculling;
  bool isTorch = false;
  int origin = 0;
};

#endif