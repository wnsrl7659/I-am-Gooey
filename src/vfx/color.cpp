/******************************************************************************/
/*!
\file   color.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for Color component, which is responsible for setting 
				uniforms for certain shaders

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "color.h"
#include "../graphics/Shader.h"
#include "../graphics/Sprite.h"
#include "../gameobject/gameobject.h"

/*****************************************************************/
/*!
		\brief
				Constructs a color, with default color black
*/
/*****************************************************************/
Color::Color()
{
		uniformName = "uColor";
		red = 0;
		green = 0;
		blue = 0;
		dR = 0;
		dG = 0;
		dB = 0;

		maxR = 1.0f;
		maxG = 1.0f; 
		maxB = 1.0f;
}

/*****************************************************************/
/*!
		\brief
				Destroys a Color, nothing to destroy though...
*/
/*****************************************************************/
Color::~Color()
{
}

/*****************************************************************/
/*!
		\brief
				Updates a color, but nothing to update yet...
*/
/*****************************************************************/
void Color::Update(float dt)
{
		red -= dR * dt;
		green -= dG * dt;
		blue -= dB * dt;

		if (red > maxR)
				red = maxR;
		if (green > maxG)
				green = maxG;
		if (blue > maxB)
				blue = maxB;

		if (red > 1.f)
				red = 1.f;
		if (green > 1.f)
				green = 1.f;
		if (blue > 1.f)
				blue = 1.f;
}

/*****************************************************************/
/*!
		\brief
				Draws the color, but nothing to draw...
*/
/*****************************************************************/
void Color::Draw() const
{
}

/*****************************************************************/
/*!
		\brief
				Gets the red value
		\return
				the red value
*/
/*****************************************************************/
float Color::r()
{
		return red;
}
/*****************************************************************/
/*!
		\brief
				Gets the green value
		\return
				the green value
*/
/*****************************************************************/
float Color::g()
{
		return green;
}
/*****************************************************************/
/*!
		\brief
				Gets the blue value
		\return
				the blue value. maybe.
*/
/*****************************************************************/
float Color::b()
{
		return blue;
}

/*****************************************************************/
/*!
		\brief
				Sets the red value
		\param f
				The value to set it to
*/
/*****************************************************************/
void Color::r(float f)
{
		if (f > 1.f)
				f = 1.f;
		red = f;
}

/*****************************************************************/
/*!
		\brief
				Sets the green value
		\param f
				The value to set it to
*/
/*****************************************************************/
void Color::g(float f)
{
		if (f > 1.f)
				f = 1.f;
		green = f;
}

/*****************************************************************/
/*!
		\brief
				Sets the blue value
		\param f
				The value to set it to
*/
/*****************************************************************/
void Color::b(float f)
{
		if (f > 1.f)
				f = 1.f;
		blue = f;
}

/*****************************************************************/
/*!
		\brief
				Sets the uniforms for the Object the color is attached to
		\param shaderProgram
				The shader to set uniforms to (if the shader has the
				correct uniforms)
*/
/*****************************************************************/
void Color::setUniforms()
{
	if (owner)
	{
		Sprite* Spr = owner->GetComponent<Sprite>();
		if (Spr)
		{
			int index = Spr->getShaderIndex();
			if (index >= 0 && index < Shader_Last)
			{
				GLint loca = glGetUniformLocation(ShaderArray[index], uniformName.c_str());
				if (loca != -1)
					glUniform3f(loca, red, green, blue);
			}
		}
	}
}

void Color::SetColorChange(float dr, float dg, float db)
{
		dR = dr;
		dG = dg;
		dB = db;
}
