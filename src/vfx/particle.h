/******************************************************************************/
/*!
\file   particle.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Header for the particle class

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../gameobject/gameobject.h"
#include "../graphics/Sprite.h"

class Emitter;

class Particle
{
public:
		Particle();

		//Particle Functions
		void particleInit(Emitter *emitter);
		void update(float dt);
		void updateTime(float dt);
		float getTime();
		void addTime(float f) { time += f; }
		GameObject* getObject();		
		Emitter* getParent();
		~Particle();

private:
		friend class Emitter;  //!< Lets Emitters access the privates of this class
		Emitter *parent;  //!< A pointer to the Emitter which created this particle
		GameObject *object;  //!< A pointer to the game object which represents the particle

		//Time for equation: Position = P_initial + Velocity*time + .5*Accel*time^2
		float time;  //!< How much time has elapsed since the particle has been initialized

	//Constant Varialbes for a particle
		//Variables calculated using emitter arrays
		float initialAngle; //!< The initial angle of the object
		float initialAlpha; //!< Initial alpha of the particle
		float red; //!< The Red value for the particle, if emitter is color mode
		float green; //!< The Green value for the particle, if emitter is color mode
		float blue; //!< The Blue value for the particle, if emitter is color mode

		//variables with x and y directions
		//Not the same as the emitters base + range combinations
		float Pos[2];  //!< Initial X and Y position of the particle
		float Vel[2];  //!< Initial X and Y velocity of the particle
		float Acc[2];  //!< Initial X and Y acceleration of the particle
		float Size[2]; //!< Initial Size of the particle

		//Rate of change variables
		float dAlpha;  //!< Changes alpha of the particle every update
		float dAngle;  //!< Angular velocity of the particle

		//Object components
		Sprite* ObjSpr = nullptr;
};