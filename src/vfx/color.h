/******************************************************************************/
/*!
\file   color.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for Color component, which is responsible for setting
				uniforms for certain shaders

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../component/component.h"

class Color : public Component<Color>
{
public:
		COMPONENT_NAME(Color);

		//Component Functions
		//Pretty useless here...
		Color();
		~Color();
		void Update(float dt);
		void Draw() const;

		//Getters
		float r();
		float g();
		float b();

		//Setters
		void r(float f);
		void g(float f);
		void b(float f);

		//change per second, positive subtracts
		void SetColorChange(float dr, float dg, float db);
		void SetColorMax(float r, float g, float b) { maxR = r; maxG = g; maxB = b; }

		//Utility
		void setUniforms();

		std::string uniformName;

private:
		float red;    //!< The red value of the color
		float green;  //!< The green value of the color
		float blue;   //!< The blue value of the color

		float dR; //!< change in Red value
		float dG; //!< change in Green value
		float dB; //!< change in Blue value

		//Max color values
		float maxR, maxG, maxB;
};
