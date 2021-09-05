/******************************************************************************/
/*!
\file   LightSource.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for LightSources, which is responsible for 
				lighting up scenes

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "../gameobject/gameobject.h"

//NYI
enum lightTypes
{
		point = 0,
		linesegment,

		last
};

class LightSource
{
private:
		float r;  //!< The red value of the light
		float g;  //!< The green value of the light
		float b;  //!< The blue value of the light
		float x;  //!< X position of the light
		float y;  //!< Y position of the light
		float z;  //!< Z position of the light
		float radius;  //!< The radius of the light
		lightTypes type = point;  //!< Not yet implemented
		GameObject* owner;

		//Variables for flickering
		float baseradius; //!< The real radius, rather than momentary
		bool doFlicker;   //!< Does it flicker or not

public:
		//Component Functions
		//Pretty useless here...
		LightSource();
		~LightSource();

		//Updates certain variables of the light, such as radius
		void Update(float dt);

		//Getters
		GameObject* Owner() { return owner; }
		float R() { return r; }
		float G() { return g; }
		float B() { return b; }
		float X() { return x; }
		float Y() { return y; }
		float Z() { return z; }
		float Radius() { return radius; }
		float BaseRadius() { return baseradius; }
		bool DoFlicker() { return doFlicker;  }

		//Setters
		void Set(float red, float green, float blue, float rad)
		{
				r = red, g = green, b = blue, baseradius = rad;
		}
		void Owner(GameObject* obj) { owner = obj; }
		void R(float f) { r = f; }
		void G(float f) { g = f; }
		void B(float f) { b = f; }
		void X(float f) { x = f; }
		void Y(float f) { y = f; }
		void Z(float f) { z = f; }
		void Radius(float f) { radius = f; }
		void BaseRadius(float f) { baseradius = f; }
		void DoFlicker(bool is) { doFlicker = is; }

		std::string label;
};