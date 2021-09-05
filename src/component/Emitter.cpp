/******************************************************************************/
/*!
\file   Emitter.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for the particle emitter class

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "emitter.h"

#include "../system/FileReading.h"
#include <iostream>
#include <fstream>
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\prettywriter.h>
#include "../graphics/Sprite.h"
#include "../trace/trace.h"
#include "../graphics/SpriteOrder.h"
#include <filesystem>
#include "../graphics/Sprite.h"

using namespace rapidjson;

/*****************************************************************/
/*!
		\brief
				Constructs an emitter. Variables set here don't get reset
				from the Reset to Default function
*/
/*****************************************************************/
Emitter::Emitter()
{
		particles = NULL;
		FileName = "NewEmitter.json";
		particleCount = 1;
		lifetime = 1;
		ResetToDefault();
}

/*****************************************************************/
/*!
		\brief
				Resets emitter values to the Default values
*/
/*****************************************************************/
void Emitter::ResetToDefault()
{
		//Default values
		for (int i = 0; i < 2; i++)
		{
				posOffset[i] = 0;
				angle[i] = 0;  //Angle to point in (0 is right)
				xPos[i] = 0;   //X position range
				yPos[i] = 0;   //Y position range
				forwardVel[i] = 5; //Initial velocity (in direction of the angle)
				sideVel[i] = 0;    //Sideways velocity (perpidicular to angle)
				forwardAcc[i] = 0; //Forward acceleration (directed at angle)
				sideAcc[i] = 0;    //Sidewats acceleration (perpidicular to angle)
				rotationalSpeed[i] = 0; //Rotation range of particles
				alphaSpeed[i] = 0;      //Change of alpha
				sizeX[i] = 15;
				sizeY[i] = 15;

				alpha[i] = 1;
				colorR[i] = 1;
				colorG[i] = 1;
				colorB[i] = 1;
		}

		//RANGE DEFAULTS
		alpha[1] = 0;
		colorR[1] = 0;
		colorG[1] = 0;
		colorB[1] = 0;
		sizeX[1] = 0;
		sizeY[1] = 0;
		angle[1] = 6.28f;
		timeScalar = 1.f;
		sizeScalar = 1.f;
		sizeScaleChange = 0.f;
		sizeScaleMax = 10.f;
		animIndex = 0;

		//Bool defaults
		isRadial = true;
		isAnchored = false;
		isActive = true;
		isBurst = false;
		doLoop = true;
		isColor = false;

		//Wave Defaults (Default makes them return 1 on wave.returnScalar())
		waveAlpha.WaveSet(0, 0, 1);
		waveSize.WaveSet(0, 0, 1);
		waveForward.WaveSet(0, 0, 1);
		waveSide.WaveSet(0, 0, 1);
		lifetime = 1.f;
		//Init(particleCount, lifetime);
}

/*****************************************************************/
/*!
		\brief
				Allocates memory for the particle list, or reallocates
				incase variables important to allocation were changed
		\param p_Count
				amount of particles to allocate
		\param p_duration
*/
/*****************************************************************/
void Emitter::Init(int p_Count, float p_duration)
{
		//In case data already exists
		if (particles)
		{
			//delete[] particles;//Delete it
			pArray[pCount] = particles;
			pCount++;
			particles = NULL;
		}

		//Initialize new data

		particleCount = p_Count;
		lifetime = p_duration;
		particles = new Particle[particleCount];


		float timeStep = lifetime / particleCount;
		for (int i = 0; i < particleCount; i++)
		{
				if (isBurst == false)
						//Set their initial timer to evenly distribute all particles in the emitter
						particles[i].time = timeStep * i;
				else
						particles[i].time = 0;
				

				//Give them access to the parent
				particles[i].parent = this;

				particles[i].particleInit(this);
		}

}

/*****************************************************************/
/*!
		\brief
				Updates all the particles in the emitter
		\param dt
				time elapsed
*/
/*****************************************************************/
void Emitter::Update(float dt)
{
		sizeScalar += sizeScaleChange * dt;
		if (abs(sizeScalar) > sizeScaleMax)
				sizeScalar = sizeScaleMax;

		for (int i = 0; i < particleCount; i++)
		{
				particles[i].updateTime(dt);

				if (particles[i].ObjSpr->getAlpha() > .01 || particles[i].dAlpha < 0)
				{
						particles[i].update(dt);
						particles[i].object->Update(dt);
				}
						
		}
}

/*****************************************************************/
/*!
		\brief
				Loads data for a Wave variable from a json file
		\param object
				The json-object inside a currently opened json file
		\return
				The wave that was loaded
*/
/*****************************************************************/
Wave jsonLoadWave(Value& object)
{
		Wave wave;

		if(object.HasMember("center"))
				wave.setCenter(object["center"].GetFloat());
		if (object.HasMember("height"))
				wave.setHeight(object["height"].GetFloat());
		if (object.HasMember("width"))
				wave.setWidth(object["width"].GetFloat());

		return wave;
}

/*****************************************************************/
/*!
		\brief
				Loads Emitter variables from a Json file
*/
/*****************************************************************/
void Emitter::jsonLoad()
{
		std::stringstream path;
		path << "assets/emitters/" << FileName;

		char* jsonData = StringReadFromFile(path.str().c_str());

		//A filename was given
		if (jsonData == "error")
		{
				return;
		}

		Document document;
		if (document.Parse(jsonData).HasParseError())
				return;

		if (jsonData)
				delete[] jsonData;

		assert(document.IsObject());

		//Prior to loading values, reset values to default incase variables any aren't specified
		ResetToDefault();

		//PARTICLE INITIALIZER VARIABLES
		for (int i = 0; i < 2; i++)
		{
				std::string type = "Range";
				if (i == 0)
						type = "Base";
				
				if(document.HasMember(("Angle" + type).c_str()))
						angle[i] = document[("Angle"+ type).c_str()].GetFloat();
				if (document.HasMember(("xPos" + type).c_str()))
						xPos[i] = document[("xPos" + type).c_str()].GetFloat();
				if (document.HasMember(("yPos" + type).c_str()))
						yPos[i] = document[("yPos" + type).c_str()].GetFloat();
				if (document.HasMember(("forwardVel" + type).c_str()))
						forwardVel[i] = document[("forwardVel" + type).c_str()].GetFloat();
				if (document.HasMember(("sideVel" + type).c_str()))
						sideVel[i] = document[("sideVel" + type).c_str()].GetFloat();
				if (document.HasMember(("forwardAcc" + type).c_str()))
						forwardAcc[i] = document[("forwardAcc" + type).c_str()].GetFloat();
				if (document.HasMember(("sideAcc" + type).c_str()))
						sideAcc[i] = document[("sideAcc" + type).c_str()].GetFloat();
				if (document.HasMember(("rotationalSpeed" + type).c_str()))
						rotationalSpeed[i] = document[("rotationalSpeed" + type).c_str()].GetFloat();
				if (document.HasMember(("alphaSpeed" + type).c_str()))
						alphaSpeed[i] = document[("alphaSpeed" + type).c_str()].GetFloat();
				if (document.HasMember(("sizeX" + type).c_str()))
						sizeX[i] = document[("sizeX" + type).c_str()].GetFloat();
				if (document.HasMember(("sizeY" + type).c_str()))
						sizeY[i] = document[("sizeY" + type).c_str()].GetFloat();
				if (document.HasMember(("alpha" + type).c_str()))
						alpha[i] = document[("alpha" + type).c_str()].GetFloat();
				if (document.HasMember(("colorR" + type).c_str()))
						colorR[i] = document[("colorR" + type).c_str()].GetFloat();
				if (document.HasMember(("colorG" + type).c_str()))
						colorG[i] = document[("colorG" + type).c_str()].GetFloat();
				if (document.HasMember(("colorB" + type).c_str()))
						colorB[i] = document[("colorB" + type).c_str()].GetFloat();
		}

		//PARTICLE FLAGS
		if (document.HasMember("isAnchored"))
				isAnchored = document["isAnchored"].GetBool();
		if (document.HasMember("isRadial"))
				isRadial = document["isRadial"].GetBool();
		if (document.HasMember("isBurst"))
				isBurst = document["isBurst"].GetBool();
		if (document.HasMember("isActive"))
				isActive = document["isActive"].GetBool();
		if (document.HasMember("doLoop"))
				doLoop = document["doLoop"].GetBool();
		if (document.HasMember("isColor"))
				isColor = document["isColor"].GetBool();

		//EMITTER VARIABLES
		if (document.HasMember("particleCount"))
				particleCount = document["particleCount"].GetInt();
		if (document.HasMember("lifetime"))
				lifetime = document["lifetime"].GetFloat();
		if (document.HasMember("timeScalar"))
				timeScalar = document["timeScalar"].GetFloat();;

		if (document.HasMember("posOffsetX"))
				posOffset[0] = document["posOffsetX"].GetFloat();
		if (document.HasMember("posOffsetY"))
				posOffset[1] = document["posOffsetY"].GetFloat();

		//WAVE VARIABLES
		if (document.HasMember("waveAlpha"))
		{
				Value& object = document["waveAlpha"];
				waveAlpha = jsonLoadWave(object);
		}
		if (document.HasMember("waveForward"))
		{
				Value& object = document["waveForward"];
				waveForward = jsonLoadWave(object);
		}
		if (document.HasMember("waveSide"))
		{
				Value& object = document["waveSide"];
				waveSide = jsonLoadWave(object);
		}
		if (document.HasMember("waveSize"))
		{
				Value& object = document["waveSize"];
				waveSize = jsonLoadWave(object);
		}

		//OTHER VARIABLES
		if (document.HasMember("animIndex"))
				animIndex = document["animIndex"].GetInt();

		//Reinitialize the emitter
		Init(particleCount, lifetime);
}

/*****************************************************************/
/*!
		\brief
				Helper function for saving the emitters float data to json
		\param doc
				json document
		\param f
				the float to add
		\param name
				name of the member to add
		\param allocator
				the allocator for the json file
				
*/
/*****************************************************************/
static void SetFloatAndAddMember(Document& doc, float f, Document::StringRefType name, Document::AllocatorType& allocator)
{
		Value floatValue;
		floatValue.SetFloat(f);
		doc.AddMember(name, floatValue, allocator);
}

/*****************************************************************/
/*!
		\brief
				Saves a wave variable to json
		\param alloc
				allocator for the json file
		\param wave
				the wave to save
		\return
				the json value that was made
*/
/*****************************************************************/
Value jsonSaveWave(Document::AllocatorType& alloc, Wave& wave)
{
		Value object(kObjectType);
		Value floatValue;
		floatValue.SetFloat(wave.getCenter());
		object.AddMember("center", floatValue, alloc);
		floatValue.SetFloat(wave.getHeight());
		object.AddMember("height", floatValue, alloc);
		floatValue.SetFloat(wave.getWidth());
		object.AddMember("width", floatValue, alloc);
		return object;
}

/*****************************************************************/
/*!
		\brief
				Saves an emitter to a json file
*/
/*****************************************************************/
void Emitter::jsonSave()
{
		//Dont save if submission
		#ifndef _SUBMISSION
				Document document;
				document.SetObject();
				Document::AllocatorType& alloc = document.GetAllocator();

				Value Emitter(kObjectType);

				//PARTICLE INITIALIZER VARIABLES
				Value floatValue;
				//angle
				SetFloatAndAddMember(document, angle[0], "AngleBase", alloc);
				SetFloatAndAddMember(document, angle[1], "AngleRange", alloc);
				//xPos
				SetFloatAndAddMember(document, xPos[0], "xPosBase", alloc);
				SetFloatAndAddMember(document, xPos[1], "xPosRange", alloc);
				//yPos
				SetFloatAndAddMember(document, yPos[0], "yPosBase", alloc);
				SetFloatAndAddMember(document, yPos[1], "yPosRange", alloc);
				//forwardVel
				SetFloatAndAddMember(document, forwardVel[0], "forwardVelBase", alloc);
				SetFloatAndAddMember(document, forwardVel[1], "forwardVelRange", alloc);
				//sideVel
				SetFloatAndAddMember(document, sideVel[0], "sideVelBase", alloc);
				SetFloatAndAddMember(document, sideVel[1], "sideVelRange", alloc);
				//forwardAcc
				SetFloatAndAddMember(document, forwardAcc[0], "forwardAccBase", alloc);
				SetFloatAndAddMember(document, forwardAcc[1], "forwardAccRange", alloc);
				//sideAcc
				SetFloatAndAddMember(document, sideAcc[0], "sideAccBase", alloc);
				SetFloatAndAddMember(document, sideAcc[1], "sideAccRange", alloc);
				//rotationalSpeed
				SetFloatAndAddMember(document, rotationalSpeed[0], "rotationalSpeedBase", alloc);
				SetFloatAndAddMember(document, rotationalSpeed[1], "rotationalSpeedRange", alloc);
				//alphaSpeed
				SetFloatAndAddMember(document, alphaSpeed[0], "alphaSpeedBase", alloc);
				SetFloatAndAddMember(document, alphaSpeed[1], "alphaSpeedRange", alloc);
				//sizeX
				SetFloatAndAddMember(document, sizeX[0], "sizeXBase", alloc);
				SetFloatAndAddMember(document, sizeX[1], "sizeXRange", alloc);
				//sizeY
				SetFloatAndAddMember(document, sizeY[0], "sizeYBase", alloc);
				SetFloatAndAddMember(document, sizeY[1], "sizeYRange", alloc);
				//alpha
				SetFloatAndAddMember(document, alpha[0], "alphaBase", alloc);
				SetFloatAndAddMember(document, alpha[1], "alphaRange", alloc);
				//colorR
				SetFloatAndAddMember(document, colorR[0], "colorRBase", alloc);
				SetFloatAndAddMember(document, colorR[1], "colorRRange", alloc);
				//colorG
				SetFloatAndAddMember(document, colorG[0], "colorGBase", alloc);
				SetFloatAndAddMember(document, colorG[1], "colorGRange", alloc);
				//colorB
				SetFloatAndAddMember(document, colorB[0], "colorBBase", alloc);
				SetFloatAndAddMember(document, colorB[1], "colorBRange", alloc);

				//PARTICLE FLAGS
				Value boolValue;
				//isAnchored
				boolValue.SetBool(isAnchored);
				document.AddMember("isAnchored", boolValue, alloc);
				//isRadial
				boolValue.SetBool(isRadial);
				document.AddMember("isRadial", boolValue, alloc);
				//isBurst
				boolValue.SetBool(isBurst);
				document.AddMember("isBurst", boolValue, alloc);
				//isActive
				boolValue.SetBool(isActive);
				document.AddMember("isActive", boolValue, alloc);
				//doLoop
				boolValue.SetBool(doLoop);
				document.AddMember("doLoop", boolValue, alloc);
				//isColor
				boolValue.SetBool(isColor);
				document.AddMember("isColor", boolValue, alloc);

				//EMITTER VARIABLES
				Value intValue;
				intValue.SetInt(particleCount);
				document.AddMember("particleCount", intValue, alloc);
				floatValue.SetFloat(lifetime);
				document.AddMember("lifetime", floatValue, alloc);
				floatValue.SetFloat(timeScalar);
				document.AddMember("timeScalar", floatValue, alloc);

				floatValue.SetFloat(posOffset[0]);
				document.AddMember("posOffsetX", floatValue, alloc);
				floatValue.SetFloat(posOffset[1]);
				document.AddMember("posOffsetY", floatValue, alloc);

				//WAVES
				Value object(kObjectType);

				object = jsonSaveWave(alloc, waveAlpha);
				document.AddMember("waveAlpha", object, alloc);
				object = jsonSaveWave(alloc, waveForward);
				document.AddMember("waveForward", object, alloc);
				object = jsonSaveWave(alloc, waveSide);
				document.AddMember("waveSide", object, alloc);
				object = jsonSaveWave(alloc, waveSize);
				document.AddMember("waveSize", object, alloc);

				//OTHER VARIABLES
				intValue.SetInt(animIndex);
				document.AddMember("animIndex", intValue, alloc);

				//Saving Part

				StringBuffer buffer;
				PrettyWriter<StringBuffer> writer(buffer);
				document.Accept(writer);
				std::string filepath("assets/emitters/");
				filepath += FileName;
				std::ofstream out(filepath.c_str());
				out << buffer.GetString();
		#endif // !
}

/*****************************************************************/
/*!
		\brief
				deletes the particles attached to the emitter
*/
/*****************************************************************/
Emitter::~Emitter()
{
		if (particles)
				delete[] particles;
}

/*****************************************************************/
/*!
		\brief
				returns the particle count
		\return
				returns the particle count
*/
/*****************************************************************/
int Emitter::getParticleCount()
{
		return particleCount;
}
/*****************************************************************/
/*!
		\brief
				returns the lifetime of a particle
		\return
				returns the lifetime of a particle
*/
/*****************************************************************/
float Emitter::getLifetime()
{
		return lifetime;
}
/*****************************************************************/
/*!
		\brief
				returns the pointer to the particle list
		\return
				returns the pointer to the particle list
*/
/*****************************************************************/
Particle* Emitter::getParticles()
{
		return particles;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the time scalar
		\return
				returns the two floats for the time scalar
*/
/*****************************************************************/
float Emitter::getScalar()
{
		return timeScalar;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the initial angle
		\return
				returns the two floats for the initial angle
*/
/*****************************************************************/
float* Emitter::getAngle()
{
		return angle;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the x position
		\return 
				returns the two floats for the x position
*/
/*****************************************************************/
float* Emitter::getxPos()
{
		return xPos;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the y position
		\return
				returns the two floats for the y position
*/
/*****************************************************************/
float* Emitter::getyPos()
{
		return yPos;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the forward velocity
		\return
				returns the two floats for the forward velocity
*/
/*****************************************************************/
float* Emitter::getForwardVel()
{
		return forwardVel;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the sideways velocity
		\return
				returns the two floats for the sideways velocity
*/
/*****************************************************************/
float* Emitter::getSideVel()
{
		return sideVel;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the forward acceleration
		\return
				returns the two floats for the forward acceleration
*/
/*****************************************************************/
float* Emitter::getForwardAcc()
{
		return forwardAcc;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the sideways acceleration
		\return
				returns the two floats for the sideways acceleration
*/
/*****************************************************************/
float* Emitter::getSideAcc()
{
		return sideAcc;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the rotational speed
		\return
				returns the two floats for the rotational speed
*/
/*****************************************************************/
float* Emitter::getRotationalSpeed()
{
		return rotationalSpeed;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the change in alpha
		\return
				returns the two floats for the change in alpha
*/
/*****************************************************************/
float* Emitter::getAlphaSpeed()
{
		return alphaSpeed;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the sprites x scale
		\return
				returns the two floats for the sprites x scale
*/
/*****************************************************************/
float* Emitter::getSizeX()
{
		return sizeX;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the sprites y size
		\return
				returns the two floats for the sprites y size
*/
/*****************************************************************/
float* Emitter::getSizeY()
{
		return sizeY;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the initial alpha
		\return
				returns the two floats for the initial alpha
*/
/*****************************************************************/
float* Emitter::getAlpha()
{
		return alpha;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the red color
		\return
				returns the two floats for the red color
*/
/*****************************************************************/
float* Emitter::getColorR()
{
		return colorR;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the green color
		\return
				returns the two floats for the green color
*/
/*****************************************************************/
float* Emitter::getColorG()
{
		return colorG;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the blue color
		\return
				returns the two floats for the blue color
*/
/*****************************************************************/
float* Emitter::getColorB()
{
		return colorB;
}
/*****************************************************************/
/*!
		\brief
				returns the two floats for the position offset
		\return
				returns the two floats for the position offset
*/
/*****************************************************************/
float* Emitter::getPosOffset()
{
		return posOffset;
}
/*****************************************************************/
/*!
		\brief
				returns the flag for if the particles anchor to the emitter
				position
		\return
				returns the flag for if the particles anchor to the emitter
				position
*/
/*****************************************************************/
bool Emitter::getIsAnchored()
{
		return isAnchored;
}
/*****************************************************************/
/*!
		\brief
				returns the flag for if the particles away from the center
				of the emitter
		\return
				returns the flag for if the particles away from the center
				of the emitter
*/
/*****************************************************************/
bool Emitter::getIsRadial()
{
		return isRadial;
}
/*****************************************************************/
/*!
		\brief
				returns the flag for if the particles away from the center
				of the emitter
		\return
				returns the flag for if the particles away from the center
				of the emitter
*/
/*****************************************************************/
bool Emitter::getIsBurst()
{
		return isBurst;
}
/*****************************************************************/
/*!
		\brief
				returns the flag for if the particles loop after their
				lifetime expires
		\return
				returns the flag for if the particles loop after their
				lifetime expires
*/
/*****************************************************************/
bool Emitter::getDoLoop()
{
		return doLoop;
}
/*****************************************************************/
/*!
		\brief
				returns the flag for if the particles display a solid
				color
		\return
				returns the flag for if the particles display a solid
				color
*/
/*****************************************************************/
bool Emitter::getIsColor()
{
		return isColor;
}
//Wave getters
/*****************************************************************/
/*!
		\brief
				returns the wave variable for side velocity
		\return
				returns the wave variable for side velocity
*/
/*****************************************************************/
Wave Emitter::getWaveSide()
{
		return waveSide;
}
/*****************************************************************/
/*!
		\brief
				returns the wave variable for forward velocity
		\return
				returns the wave variable for forward velocity
*/
/*****************************************************************/
Wave Emitter::getWaveForward()
{
		return waveForward;
}
/*****************************************************************/
/*!
		\brief
				returns the wave variable for image size
		\return
				returns the wave variable for image size
*/
/*****************************************************************/
Wave Emitter::getWaveSize()
{
		return waveSize;
}
/*****************************************************************/
/*!
		\brief
				returns the wave variable for alpha change, NYI
		\return
				returns the wave variable for alpha change
*/
/*****************************************************************/
Wave Emitter::getWaveAlpha()
{
		return waveAlpha;
}
/*****************************************************************/
/*!
		\brief
				returns the file name to load a json for
		\return
				returns the file name to load a json for
*/
/*****************************************************************/
std::string Emitter::getFileName()
{
		return FileName;
}

/*****************************************************************/
/*!
		\brief
				Sets the scalar
		\param num
				number to set it to
*/
/*****************************************************************/
void Emitter::setScalar(float num)
{
		timeScalar = num;
}
/*****************************************************************/
/*!
		\brief
				Sets the two Angle floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setAngle(float base, float range)
{
		angle[0] = base;
		angle[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two x position floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setxPos(float base, float range)
{
		xPos[0] = base;
		xPos[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two y position floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setyPos(float base, float range)
{
		yPos[0] = base;
		yPos[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two forward velocity floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setForwardVel(float base, float range)
{
		forwardVel[0] = base;
		forwardVel[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two side velocity floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setSideVel(float base, float range)
{
		sideVel[0] = base;
		sideVel[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two forward acceleration floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setForwardAcc(float base, float range)
{
		forwardAcc[0] = base;
		forwardAcc[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two side acceleration floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setSideAcc(float base, float range)
{
		sideAcc[0] = base;
		sideAcc[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two rotational speed floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setRotationalSpeed(float base, float range)
{
		rotationalSpeed[0] = base;
		rotationalSpeed[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two alpha speed floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setAlphaSpeed(float base, float range)
{
		alphaSpeed[0] = base;
		alphaSpeed[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two image width floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setSizeX(float base, float range)
{
		sizeX[0] = base;
		sizeX[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two image height floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setSizeY(float base, float range)
{
		sizeY[0] = base;
		sizeY[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two initial alpha floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setAlpha(float base, float range)
{
		alpha[0] = base;
		alpha[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two red value floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setColorR(float base, float range)
{
		colorR[0] = base;
		colorR[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two green value floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setColorG(float base, float range)
{
		colorG[0] = base;
		colorG[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the two blue value floats
		\param base
				Sets the first float value
		\param range
				Sets the second floats value
*/
/*****************************************************************/
void Emitter::setColorB(float base, float range)
{
		colorB[0] = base;
		colorB[1] = range;
}
/*****************************************************************/
/*!
		\brief
				Sets the color floats
		\param r
				red value
		\param g
				green value
		\param b
				blue value
*/
/*****************************************************************/
void Emitter::setColorBases(float r, float g, float b)
{
		colorR[0] = r;
		colorG[0] = g;
		colorB[0] = b;
}
/*****************************************************************/
/*!
		\brief
				Sets the color ranges floats
		\param r
				red value
		\param g
				green value
		\param b
				blue value
*/
/*****************************************************************/
void Emitter::setColorRanges(float r, float g, float b)
{
		colorR[1] = r;
		colorG[1] = g;
		colorB[1] = b;
}
/*****************************************************************/
/*!
		\brief
				Sets the position offset
		\param x
				x offset
		\param y
				y offet
*/
/*****************************************************************/
void Emitter::setPosOffset(float x, float y)
{
		posOffset[0] = x;
		posOffset[1] = y;
}
/*****************************************************************/
/*!
		\brief
				Sets the flag for anchoring particles to the emitter
		\param is
				sets the bool to true or false
*/
/*****************************************************************/
void Emitter::setIsAnchored(bool is)
{
		isAnchored = is;
}
/*****************************************************************/
/*!
		\brief
				Sets the flag for emitting particles directionally away
				from the center
		\param is
				sets the bool to true or false
*/
/*****************************************************************/
void Emitter::setIsRadial(bool is)
{
		isRadial = is;
}
/*****************************************************************/
/*!
		\brief
				Sets the flag for bursting all the particles out at once
		\param is
				sets the bool to true or false
*/
/*****************************************************************/
void Emitter::setIsBurst(bool is)
{
		if (is != isBurst)
		{
				isBurst = is;
				//Reinitialize the emitter
				Init(particleCount, lifetime);
		}
}
/*****************************************************************/
/*!
		\brief
				Sets the flag for if particles loop after dying
		\param is
				sets the bool to true or false
*/
/*****************************************************************/
void Emitter::setDoLoop(bool is)
{
		if (is != doLoop)
		{
				doLoop = is;
				//Reinitialize the emitter
				//Init(particleCount, lifetime);
		}
}
/*****************************************************************/
/*!
		\brief
				Sets the flag for if the particles are solid colors
		\param is
				sets the bool to true or false
*/
/*****************************************************************/
void Emitter::setIsColor(bool is)
{
		if (is != isColor)
		{
				isColor = is;
				//Reinitialize the emitter
				Init(particleCount, lifetime);
		}
}
//Wave setters
/*****************************************************************/
/*!
		\brief
				Sets the wave data for side velocity
		\param w
				The wave to copy from
*/
/*****************************************************************/
void Emitter::setWaveSide(Wave w)
{
		waveSide = w;
}
/*****************************************************************/
/*!
		\brief
				Sets the wave data for forward velocity
		\param w
				The wave to copy from
*/
/*****************************************************************/
void Emitter::setWaveForward(Wave w)
{
		waveForward = w;
}
/*****************************************************************/
/*!
		\brief
				Sets the wave data for sprite size
		\param w
				The wave to copy from
*/
/*****************************************************************/
void Emitter::setWaveSize(Wave w)
{
		waveSize = w;
}
/*****************************************************************/
/*!
		\brief
				Sets the wave data for alpha change
		\param w
				The wave to copy from
*/
/*****************************************************************/
void Emitter::setWaveAlpha(Wave w)
{
		waveAlpha = w;
}
/*****************************************************************/
/*!
		\brief
				Sets the filename to load json from
		\param filename
				The name of the file, ex. boom.json
*/
/*****************************************************************/
void Emitter::setFileName(std::string filename)
{
		FileName = filename;
}
