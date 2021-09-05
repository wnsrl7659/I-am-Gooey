/******************************************************************************/
/*!
\file		MultiEmitter.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "MultiEmitter.h"

/*****************************************************************/
/*!
		\brief
				Updates all the emitters, and thus the particles inside
		\param dt
				time elapsed
*/
/*****************************************************************/
void MultiEmitter::Update(float dt)
{
		for (auto it : emitters)
		{
				it->Update(dt);
		}
}

/*****************************************************************/
/*!
		\brief
				Finds the given emitter inside the MultiEmitter, and 
				removes it from the vector. Does nothing if not inside
*/
/*****************************************************************/
void MultiEmitter::removeEmitter(Emitter* toRemove)
{
		for (unsigned i = 0; i < emitters.size(); i++)
		{
				if (emitters[i] == toRemove)
				{
						Emitter* toRemove = emitters[i];

						//Override the current position with the back one
						//and pop back that one
						emitters[i] = emitters.back();
						emitters.pop_back();

						//toRemove is nowhere in the vector, so just delete
						delete toRemove;
				}
		}
}

/*****************************************************************/
/*!
		\brief
				Adds an Emitter to the MultiEmitter
		\param filename
				The filename of the Emitter to load
*/
/*****************************************************************/
void MultiEmitter::addEmitter(std::string filename)
{
		emitters.push_back(new Emitter());

		Emitter * emitter = emitters.back();
		emitter->owner = owner;//Set the emitters owner to my owner
		emitter->setFileName(filename);
		emitter->jsonLoad();
}

/*****************************************************************/
/*!
		\brief
				Deletes all emitters inside the multiemitter
*/
/*****************************************************************/
MultiEmitter::~MultiEmitter()
{
		while (emitters.size())
		{
				delete emitters.back();
				emitters.pop_back();
		}
}