/******************************************************************************/
/*!
\file   Wave.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for a wave variable, which returns a value from cos
				depending on the variables provided to it.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <string>

class Wave
{
public:
		Wave();
		void WaveSet(float centerHeight, float heightScale, float widthScale);
		float returnScalar(float timeInSeconds);

		float getCenter();
		float getHeight();
		float getWidth();
		std::string getName();

		void setCenter(float f);
		void setHeight(float f);
		void setWidth(float f);
		void setName(std::string n);

private:
		std::string name; //!< Name for saving and loading
		float center; //!< Where the center point is
		float height; //!< Height of wave, from center point
		float width; //!< Stretch factor
};
