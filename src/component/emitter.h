/******************************************************************************/
/*!
\file   emitter.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for the particle emitter component

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../component/component.h"
#include "../vfx/particle.h"
#include "../vfx/Wave.h"
#include "../animation/animation.h"

class MultiEmitter; //Forward declaration

class Emitter : public Component<Emitter>
{
public:
		COMPONENT_NAME(Emitter); //!< Name of the component

		Emitter();

		void Init(int p_Count, float p_duration);
		void ResetToDefault();
		virtual ~Emitter();

		virtual void Update(float dt);
		virtual void Draw() const {}

				//Json functions
		void jsonLoad();
		void jsonSave();

				//Data only set on initialization
		int getParticleCount();
		float getLifetime();
		Particle* getParticles();
				//Float getters
		float getSizeScalar() { return sizeScalar; }
		float getSizeScaleChange() { return sizeScaleChange; }
		float getSizeScaleMax() { return sizeScaleMax; }
		float getScalar();
		float* getAngle();
		float* getxPos();
		float* getyPos();
		float* getForwardVel();
		float* getSideVel();
		float* getForwardAcc();
		float* getSideAcc();
		float* getRotationalSpeed();
		float* getAlphaSpeed();
		float* getSizeX(); 
		float* getSizeY(); 
		float* getAlpha();
		float* getColorR();
		float* getColorG();
		float* getColorB();
		float* getPosOffset();

				//Flag getters
		bool getIsAnchored();
		bool getIsRadial();
		bool getIsBurst();
		bool getDoLoop();
		bool getIsColor();
				//Wave getters
		Wave getWaveSide();
		Wave getWaveForward();
		Wave getWaveSize();
		Wave getWaveAlpha();

				//Other getters
		std::string getFileName();
		int getAnimIndex() { return animIndex; }

				//Float setters
		void setSizeScalar(float f) { sizeScalar = f; }
		void setSizeScaleChange(float f) { sizeScaleChange = f; }
		void setSizeScaleMax(float f) { sizeScaleMax = f; }
		void setScalar(float);
		void setAngle(float base, float range);
		void setxPos(float base, float range);
		void setyPos(float base, float range);
		void setForwardVel(float base, float range);
		void setSideVel(float base, float range);
		void setForwardAcc(float base, float range);
		void setSideAcc(float base, float range);
		void setRotationalSpeed(float base, float range);
		void setAlphaSpeed(float base, float range);
		void setSizeX(float base, float range);
		void setSizeY(float base, float range);
		void setAlpha(float base, float range);
		void setColorR(float base, float range);
		void setColorG(float base, float range);
		void setColorB(float base, float range);
		void setColorBases(float r, float g, float b);
		void setColorRanges(float r, float g, float b);
		void setPosOffset(float x, float y);

				//Flag setters
		void setIsAnchored(bool is);
		void setIsRadial(bool is);
		void setIsBurst(bool is);
		void setDoLoop(bool is);
		void setIsColor(bool is);

				//Wave setters
		void setWaveSide(Wave w);
		void setWaveForward(Wave w);
		void setWaveSize(Wave w);
		void setWaveAlpha(Wave w);
				//Other setters
		void setFileName(std::string filename);
		void setAnimIndex(int index) { animIndex = index; }

private:
		std::string FileName;
		//MultiEmitter* ParentMulti = nullptr;

//PARTICLE INITIALIZER VARIABLES
		//all float pairs, Base value + random range value
		int animIndex;
		float angle[2]; //!< Angle to point in initially
		float xPos[2]; //!< position range
		float yPos[2]; //!< position range
		float posOffset[2]; //!< x,y position offset
		float forwardVel[2]; //!< Initial velocity (in direction of the angle)
		float sideVel[2]; //!< Sideways velocity (perpidicular to angle)
		float forwardAcc[2]; //!< Forward acceleration (directed at angle)
		float sideAcc[2]; //!< Sidewats acceleration (perpidicular to angle)
		float rotationalSpeed[2]; //!< Rotation range of particles
		float alphaSpeed[2]; //!< Change of alpha
		float sizeX[2]; //!< X_scale
		float sizeY[2]; //!< Y_scale
		float alpha[2]; //!< Initial alpha (opacity)
		float colorR[2]; //!< Initial Red value (0 to 1)
		float colorG[2]; //!< Initial Green value
		float colorB[2]; //!< Initial Blue value

		//Waves which are used to scale certain things
		Wave waveSide;  //!< Sideways Velocity Wave
		Wave waveForward; //!< Forward Velocity Wave
		Wave waveSize; //!< Size Velocity Wave
		Wave waveAlpha; //!< Image Alpha Wave

//PARTICLE FLAGS
		bool isAnchored; //!< Moves the particles with the moving object, or lets the fall behind.
		bool isRadial; //!< Effects physics (forward and side physics) acoording to the angle
		bool isBurst; //!< If the particles all burst out
		bool isActive; //!< If you should draw and update things
		bool doLoop; //!< Loops the particles when they die
		bool isColor; //!< Color or Texture mode

//EMITTER VARIABLES
		int particleCount; //!< How many particles to make
		float lifetime; //!< How long a particle lasts, not ranged to preserve offset consistancy
		float timeScalar; //!< How quickly to scale to effects of time (basically scales the physics)
		float sizeScalar;
		float sizeScaleChange;
		float sizeScaleMax;

//PARTICLE INFORMATION STORAGE
		Particle *particles; //!< array of particles
		friend class Particle;  //!< lets particles access private data in the emitter
		friend class MultiEmitter;

//Optimization test
		bool firstHalf = true;
};
