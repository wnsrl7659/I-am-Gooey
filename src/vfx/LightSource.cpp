/******************************************************************************/
/*!
\file		LightSource.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LightSource.h"
#include "../system/Utility.h" //Float Rand function
#include "../physics/Transform.h"

LightSource::LightSource()
{
		//defaults to a white light (standard normal coloration)
		r = 1.0;  
		g = 1.0;
		b = 1.0;
		x = 0.0;
		y = 0.0;
		z = 0.0;  
		radius = 64;

		label = "";

		//Variables for flickering
		baseradius = 64;
		doFlicker = false;
}
LightSource::~LightSource()
{
		//Lol x dee
}

//Updates certain variables of the light, such as radius
void LightSource::Update(float dt)
{
		if (doFlicker)
		{
				float range = .02f;
				float base = 1.0f + range / 2.f;
				radius *= floatRand(base, range);
				if (radius > baseradius*1.1f)
						radius = baseradius;
		}
		else
				radius = baseradius;

		if (owner)
		{
				Transform* trs = owner->GetComponent<Transform>();
				if (trs)
				{
						x = trs->Translation()[0];
						y = trs->Translation()[1];
				}
		}
}
