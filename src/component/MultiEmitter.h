/******************************************************************************/
/*!
\file   MultiEmitter.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for the particle emitter component

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "emitter.h"

class MultiEmitter : public Component<MultiEmitter>
{
public:
		COMPONENT_NAME(MultiEmitter); //!< Name of the component

		MultiEmitter() {}

		virtual ~MultiEmitter();
		virtual void Update(float dt);
		virtual void Draw() const {}

		void addEmitter(std::string filename);
		void removeEmitter(Emitter* toRemove);

		std::vector<Emitter*>* getEmitters() { return &emitters; }

private:

		std::vector<Emitter*> emitters; //!< array of particles
		friend class Emitter;  //!< lets particles access private data in the emitter
};
