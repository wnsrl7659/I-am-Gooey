/******************************************************************************/
/*!
\file   GameObjectSerialize.h
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Serializes and Deserializes objects into/from JSON files using RapidJSON

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "gameobject.h"
#include "../graphics/Sprite.h"
#include "../animation/animation.h"
#include "../component/controller.h"
#include "../component/emitter.h"
#include "../graphics/Mesh.h"
#include <iostream>
#include <fstream>

namespace Serial
{
	/**************************************************************************/
	/*!
	\brief Game Object Serializer, takes in a game object, and makes a JSON from it
	\param gameObj
		Game Object to copy into a JSON file
	\param filename
		Name of the json file to copy data into or to create
	*/
	/**************************************************************************/
	void Serialize(GameObject * gameObj, const char* filename);

	/**************************************************************************/
	/*!
	\brief Creates an Empty Json file with all components set to zero. Makes a stub when called, basically.
	*/
	/**************************************************************************/
	void SerializeEmpty();

	/**************************************************************************/
	/*!
	\brief Game Object Deserializer, takes in a game object, and fills it with data from a json file
	\param gameObj
	Game Object to fill with JSON data, will overwrite all previously given components
	\param filename
	Name of the json file to pull components from
	*/
	/**************************************************************************/
	void Deserialize(const char * filename, GameObject * gameObj);

	/**************************************************************************/
	/*!
	\brief Saves the current state of a game object, possibly useful for save features?
	\param gameObj
	Game Object to save the data of
	*/
	/**************************************************************************/
	void SaveSerialize(GameObject * gameObj);
}

