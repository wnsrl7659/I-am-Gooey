/******************************************************************************/
/*!
\file   SpriteOrder.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef SPRITEORDER_H
#define SPRITEORDER_H

#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "../gameobject/gameobject.h"
#include "../component/component.h"
#include "../animation/animation.h"
#include "../vfx/LightReciever.h"
#include "../vfx/color.h"
#include "../vfx/particle.h"


struct NeedforSprite
{
	GLuint			shaderid;
	int				meshid;
	int				textureid;
	glm::vec2		pos;
	glm::vec2		scale;
	float			rotate;
	float			alpha;
	Animation*		anim;
	LightReciever*	LR;
	Color*			color;
	GameObject*     owner;
};

struct NeedforFont
{
	std::wstring	text;
	GLfloat			x;
	GLfloat			y;
	GLfloat			scale;
	glm::vec3		color;
	int				id;
	int				mode;
};

struct NeedforLine
{
	GLuint			shaderid;
	int				meshid;
	glm::vec2		pos;
	glm::vec2		scale;
	float			rotate;
	glm::vec3		color;
};

void SpriteOrderInit();

void SpriteOrderUpdate(float dt);

void SpriteOrderShutdown();

void AddSprite(
	int				prior, 
	GLuint			shaderid,
	int				meshid,
	int				textureid,
	glm::vec2		pos,
	glm::vec2		scale,
	float			rotate,
	float			alpha,
	Animation*		anim,
	LightReciever*	LR,
	Color*			color,
	GameObject*     owner);

void AddFont(std::wstring text, GLfloat x, GLfloat y, 
	         GLfloat scale, glm::vec3 color, int id, int mode);

void AddLine(GLuint	shaderid, int meshid, glm::vec2 pos,
			 glm::vec2 scale, float rotate, glm::vec3 color);


extern Particle* pArray[1000];
extern int pCount;


#endif

