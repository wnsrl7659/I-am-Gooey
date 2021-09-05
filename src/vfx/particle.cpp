/******************************************************************************/
/*!
\file   particle.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for the particle class

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "particle.h"
#include "../graphics/Sprite.h"
#include "../component/emitter.h"
#include "../graphics/Shader.h"
#include "../vfx/color.h"

/*****************************************************************/
/*!
		\brief
				Helper function to generate a random number from a base
				and a range around the base 
				(base - .5range) to (base + .5range)
		\param base
				the base of the random number
		\param range
				the range around the base number
		\return
				the random number that was generated
*/
/*****************************************************************/
static float floatRand(float base, float range)
{
		float zeroToOne = (float)(rand() / (RAND_MAX + 1.0));
		return base + (zeroToOne * range) - (range / 2);
}

/*****************************************************************/
/*!
		\brief
				Constructor for the particle. Sets data which only needs
				to be set once.
*/
/*****************************************************************/
Particle::Particle()
{
		time = 0;

		object = new GameObject;

		//Add key components to the Particles

		Color* color = object->AddComponent<Color>();
		Transform* trs = object->AddComponent<Transform>();
		Sprite* spr = object->AddComponent<Sprite>();
		ObjSpr = spr;
		Animation* anim = object->AddComponent<Animation>();

		trs->Init();
		spr->Init(Shader_AnimatedTexture);
		spr->SetTexture(Tex_particle);
		spr->SetSize(2, 2);

		int rowCount = 4, colCount = 4;
		anim->Init(rowCount,colCount,rowCount*colCount,1);
		for(int i = 0; i < rowCount*colCount; i++)
				(*anim)[i][0] = i;
		anim->setSequence(0);

		spr->setPrior(1);

		/**/
}

/*****************************************************************/
/*!
		\brief
				Initializes a particle. Gets called whenever a particle
				loops back to the beginning of the emitter
		\param emitter
				The emitter to derive range data from, to set key values
				for updating the particle
*/
/*****************************************************************/
void Particle::particleInit(Emitter *emitter)
{
		if (!object)
				return;

		//Can be called multiple times (no allocation here)

		//Set timer back to 0 (or at least try to)
		while (time >= emitter->lifetime)
				time -= emitter->lifetime;

		initialAngle = floatRand(emitter->angle[0], emitter->angle[1]);
		float COS = cosf(initialAngle);
		float SIN = sinf(initialAngle);

		initialAlpha = floatRand(emitter->alpha[0], emitter->alpha[1]);

		dAlpha = floatRand(emitter->alphaSpeed[0], emitter->alphaSpeed[1]);
		dAngle = floatRand(emitter->rotationalSpeed[0], emitter->rotationalSpeed[1]);

		if (!emitter->isRadial)
		{
				COS = 1.f;
				SIN = 0.f;
		}

		float vForward = floatRand(emitter->forwardVel[0], emitter->forwardVel[1]);
		float vSideway = floatRand(emitter->sideVel[0], emitter->sideVel[1]);

		float aForward = floatRand(emitter->forwardAcc[0], emitter->forwardAcc[1]);
		float aSideway = floatRand(emitter->sideAcc[0], emitter->sideAcc[1]);

		Transform* parenttrs = emitter->owner->GetComponent<Transform>(); //The object with the emitter's translation
		Vector2D parentPos = parenttrs->Translation();
		if (emitter->isAnchored)
		{
				parentPos[0] = 0;
				parentPos[1] = 0;
		}

		//X axis
		Pos[0] = parentPos[0] + floatRand(emitter->xPos[0], emitter->xPos[1]) * COS + floatRand(emitter->yPos[0], emitter->yPos[1]) * SIN;
		Vel[0] = (vForward * COS) + (vSideway * SIN); //At angle 0, forward velocity is completely horizontal rightward
		Acc[0] = (aForward * COS) + (aSideway * SIN); //Same logic applies to acceleration

		//Y axis
		Pos[1] = parentPos[1] + floatRand(emitter->yPos[0], emitter->yPos[1]) * COS + floatRand(emitter->xPos[0], emitter->xPos[1]) * SIN;
		Vel[1] = (vForward * SIN) + (vSideway * COS); //COS & SIN Oppisite of X axis
		Acc[1] = (aForward * SIN) + (aSideway * COS);

		//Image variables
		//size
		Sprite* spr = ObjSpr;
		if (!ObjSpr)
		{
				ObjSpr = object->GetComponent<Sprite>();
				spr = ObjSpr;
		}
				

		if (emitter->isColor)
		{
				//Set the particles colors
				red = floatRand(emitter->colorR[0], emitter->colorR[1]);
				green = floatRand(emitter->colorG[0], emitter->colorG[1]);
				blue = floatRand(emitter->colorB[0], emitter->colorB[1]);

				//Set the Shader
				spr->setShader(Shader_BasicTextureColorBlend);

				//Set the objects Color component
				Color* color =  object->GetComponent<Color>();
				if (color)
				{
						color->r(red);
						color->g(green);
						color->b(blue);
				}
		}	
		else
		{
				spr->setShader(Shader_AnimatedTexture);
		}
		(object->GetComponent<Animation>())->setSequence(emitter->animIndex);

		spr->setAlpha(floatRand(emitter->alpha[0], emitter->alpha[1]));
		Size[0] = floatRand(emitter->sizeX[0], emitter->sizeX[1]);
		Size[1] = floatRand(emitter->sizeY[0], emitter->sizeY[1]);
}

/*****************************************************************/
/*!
		\brief
				updates the particle's object with the variables that
				were set at particleInit
		\param dt
				time elapsed between frames
*/
/*****************************************************************/
void Particle::updateTime(float dt)
{
		//If it goes past the parent lifetime, reintialize it
		time += (dt * parent->timeScalar);
		if (time >= parent->lifetime && parent->doLoop)
				particleInit(parent);
}

/*****************************************************************/
/*!
		\brief
				updates the particle's object with the variables that
				were set at particleInit
		\param dt
				time elapsed between frames
*/
/*****************************************************************/
void Particle::update(float dt)
{
		//Update if active
		if (!parent->isActive)
				return;

		float tempTime = time * 10;

		//Position Waves
		float forwardScalar = parent->waveForward.returnScalar(time);
		float sideScalar = parent->waveSide.returnScalar(time);
		float alphaScalar = parent->waveAlpha.returnScalar(time);

		float SIN = sinf(initialAngle);//0
		float COS = cosf(initialAngle);//1

		//SIN = 0;
		//COS = 1;

		//Calculate the position of the particle, and put into a vec2d
		//(Uses the position equation)
		float wavScaleX = forwardScalar * COS - sideScalar * SIN;
		float xOffsetter = (Vel[0] * tempTime) + (Acc[0] * tempTime * tempTime) / 2;
		float positionX = Pos[0] + ((wavScaleX) * 100 + xOffsetter)* parent->sizeScalar;

		float wavScaleY = forwardScalar * SIN + sideScalar * COS;
		float yOffsetter = (Vel[1] * tempTime) + (Acc[1] * tempTime * tempTime) / 2;
		float positionY = Pos[1] + ((wavScaleY) * 100 + yOffsetter)* parent->sizeScalar;
		Vector2D pos(positionX, positionY);

		//Update the translation component now...
		GameObject* emittersObj = parent->owner;
		Transform* mytrs = object->GetComponent<Transform>();
		if (parent->isAnchored)
		{
				Transform* parenttrs = emittersObj->GetComponent<Transform>();
				mytrs->Translation(parenttrs->Translation() + pos + Vector2D(parent->posOffset[0], parent->posOffset[1]));
		}
		else
				mytrs->Translation(pos + Vector2D(parent->posOffset[0], parent->posOffset[1]));

		if (mytrs)
		{
				float angle = mytrs->Rotation();
				angle += dAngle * 10;
				mytrs->Rotation(angle);
		}
				
		Sprite* spr = object->GetComponent<Sprite>();
		if (spr)
		{
				spr->setAlpha(initialAlpha - dAlpha* (time * 10) + alphaScalar);

				float sizeScalar = parent->waveSize.returnScalar(time);
				float tempSizeX = Size[0] * (sizeScalar + 1.f) * parent->sizeScalar;
				float tempSizeY = Size[1] * (sizeScalar + 1.f) * parent->sizeScalar;
				spr->SetSize(tempSizeX, tempSizeY);
				
				Sprite* objspr = parent->owner->GetComponent<Sprite>();
				if (objspr)
				{
					int prior = objspr->getPrior();

					if (objspr->getTorch())
					{
						spr->setPrior(3);
					}
					else if (objspr->getData() == 7659)
					{
						spr->setPrior(3);
					}
					else if (prior == 6)
					{
						spr->setPrior(6);
					}
					else
					{
						spr->setPrior(1);
					}
				}
		}

		//object->Update(dt);//Update the object inside the particle
}

/*****************************************************************/
/*!
		\brief
				gets the emitter object
		\return
				The pointer to the emitter
*/
/*****************************************************************/
Emitter* Particle::getParent()
{
		return parent;
}

/*****************************************************************/
/*!
		\brief
				gets the particle object
		\return
				the particle object
*/
/*****************************************************************/
GameObject* Particle::getObject()
{
		return object;
}

/*****************************************************************/
/*!
		\brief
				Gets the current lifetime of the particle
		\return
				the current lifetime
*/
/*****************************************************************/
float Particle::getTime()
{
		return time;
}

/*****************************************************************/
/*!
		\brief
				deletes the gameobject
*/
/*****************************************************************/
Particle::~Particle()
{
		delete object;
}