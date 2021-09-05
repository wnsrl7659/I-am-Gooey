/******************************************************************************/
/*!
\file   GameObjectSerialize.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Serializes and Deserializes objects into/from JSON files using RapidJSON

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "GameObjectSerialize.h"
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\prettywriter.h>
#include "../system/FileReading.h"
#include "../physics/Physics.h"
#include "../physics/Transform.h"
#include "../graphics/Shader.h"
#include "../component/paralax.h"
#include "../component/teleporter.h"
using namespace rapidjson;

namespace Serial
{
	int numEmpty = 0; //!< number of empty jsons serialized

	/**************************************************************************/
	/*!
	\brief Copies an existing GameObject into a JSON file
	\param gameObj
	Game Object to copy data from
	\param filename
	name of the json file to create/write into
	*/
	/**************************************************************************/
	void Serialize(GameObject * gameObj, const char * filename)
	{
		Document document;
		document.SetObject();
		Document::AllocatorType& allocator = document.GetAllocator();
		Value goName;
		goName.SetString(gameObj->name.c_str(), gameObj->name.size());
		document.AddMember("Game Object Name", goName, allocator);
		Sprite * goSpr = gameObj->GetComponent<Sprite>();
		Animation * goAnim = gameObj->GetComponent<Animation>();
		PlayerController * goPc = gameObj->GetComponent<PlayerController>();
		Physics * goPhys = gameObj->GetComponent<Physics>();
		Transform * goTransform = gameObj->GetComponent<Transform>();
		Paralax * goParalax = gameObj->GetComponent<Paralax>();
		Teleporter * goTele = gameObj->GetComponent<Teleporter>();
		if (goSpr)
		{
			Value sprite(kObjectType);
			Value shader;
			shader.SetInt(3);
			sprite.AddMember("Shader", shader, allocator);
			Value texture;
			texture.SetInt(-1);
			sprite.AddMember("Texture", texture, allocator);
			Value xpos;
			glm::vec2 pos = goSpr->GetPosition();
			xpos.SetFloat(pos.x);
			Value ypos;
			ypos.SetFloat(pos.y);
			sprite.AddMember("xpos", xpos, allocator);
			sprite.AddMember("ypos", ypos, allocator);
			Value xsize;
			glm::vec2 size = goSpr->GetSize();
			xsize.SetFloat(size.x);
			Value ysize;
			ysize.SetFloat(size.y);
			sprite.AddMember("xsize", xsize, allocator);
			sprite.AddMember("ysize", ysize, allocator);
			Value rot;
			rot.SetFloat(goSpr->GetRotation());
			sprite.AddMember("rot", rot, allocator);
			document.AddMember("Sprite", sprite, allocator);
		}
		if (goAnim)
		{
			Value animation(kObjectType);
			Value rowCount;
			rowCount.SetInt(goAnim->GetRowCount());
			animation.AddMember("RowCount", rowCount, allocator);
			Value colCount;
			colCount.SetInt(goAnim->GetColCount());
			animation.AddMember("ColCount", colCount, allocator);
			Value animCount;
			animCount.SetInt(goAnim->GetAnimationCount());
			animation.AddMember("AnimCount", animCount, allocator);
			Value animLength;
			animLength.SetInt(goAnim->GetAnimationLength());
			animation.AddMember("AnimLength", animLength, allocator);
			Value speed;
			speed.SetFloat(goAnim->GetAnimationSpeed());
			animation.AddMember("Speed", speed, allocator);
			Value animArray;
			animArray.SetArray();
			int* sequences = goAnim->GetAnimationSequence();
			for (int i = 0; i < (goAnim->GetAnimationLength()) * (goAnim->GetAnimationCount()); i++)
			{
				animArray.PushBack(sequences[i], allocator);
			}
			animation.AddMember("AnimationSequence", animArray, allocator);
			document.AddMember("Animation", animation, allocator);
		}
		if (goPc)
		{
			Value playerController(kObjectType);
			document.AddMember("PlayerController", playerController, allocator);
		}
		if (goPhys)
		{
			Value physics(kObjectType);
			Value velocity(kObjectType);
			Vector2D vel = goPhys->Velocity();
			Value XVel;
			Value YVel;
			XVel.SetFloat(vel[0]);
			YVel.SetFloat(vel[1]);
			velocity.AddMember("XVel", XVel, allocator);
			velocity.AddMember("YVel", YVel, allocator);
			physics.AddMember("Velocity", velocity, allocator);
			document.AddMember("Physics", physics, allocator);
		}
		if (goTransform)
		{
			Value transform(kObjectType);
			Value translation(kObjectType);
			Vector2D translate = goTransform->Translation();
			Value X;
			X.SetFloat(translate[0]);
			Value Y;
			Y.SetFloat(translate[1]);
			translation.AddMember("X", X, allocator);
			translation.AddMember("Y", Y, allocator);
			transform.AddMember("Translation", translation, allocator);
			Value scale(kObjectType);
			Vector2D scalar = goTransform->Scale();
			Value XScale;
			XScale.SetFloat(scalar[0]);
			Value YScale;
			YScale.SetFloat(scalar[1]);
			scale.AddMember("XScale", XScale, allocator);
			scale.AddMember("YScale", YScale, allocator);
			transform.AddMember("Scale", scale, allocator);
			Value rotation;
			rotation.SetFloat(goTransform->Rotation());
			transform.AddMember("Rotation", rotation, allocator);
			document.AddMember("Transform", transform, allocator);
			
		}
		if (goParalax)
		{
			Value paralax(kObjectType);
			Value speed;
			speed.SetFloat(goParalax->Speed());
			paralax.AddMember("Speed", speed, allocator);
			document.AddMember("Paralax", paralax, allocator);
		}
		if (goTele)
		{
			Value teleporter(kObjectType);
			Value trigger(kObjectType);
			Value triggerX;
			Value triggerY;
			Vector2D v(0,0);
			triggerX.SetFloat(v[0]);
			triggerY.SetFloat(v[1]);
			trigger.AddMember("triggerX", triggerX, allocator);
			trigger.AddMember("triggerY", triggerY, allocator);
			teleporter.AddMember("Trigger", trigger, allocator);
			Value reset(kObjectType);
			Value resetX;
			Value resetY;
			Vector2D v2(0, 0);
			resetX.SetFloat(v2[0]);
			resetY.SetFloat(v2[1]);
			reset.AddMember("resetX", resetX, allocator);
			reset.AddMember("resetY", resetY, allocator);
			teleporter.AddMember("Reset", reset, allocator);
			Value lessThan;
			Value xMatters;
			Value yMatters;
			lessThan.SetBool(false);
			xMatters.SetBool(true);
			yMatters.SetBool(true);
			teleporter.AddMember("LessThan", lessThan, allocator);
			teleporter.AddMember("xMatters", xMatters, allocator);
			teleporter.AddMember("yMatters", yMatters, allocator);

			document.AddMember("Teleporter", teleporter, allocator);
		}

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		document.Accept(writer);
		std::string filepath("assets/objects/");
		filepath.append(filename);
		std::ofstream of(filepath);
		of << buffer.GetString();
		

	}


	/**************************************************************************/
	/*!
	\brief Creates a blank JSON file with all components in it, all set to empty.
		   Used to create game objects completely from scratch without having to serialize it first
	*/
	/**************************************************************************/
	void SerializeEmpty()
	{
		Document document;
		document.SetObject();
		Document::AllocatorType& allocator = document.GetAllocator();
		Value goName;
		goName.SetString("EmptyStub, Not a Game Object");
		document.AddMember("Game Object Name", goName, allocator);
		Value sprite(kObjectType);
		Value shader;
		shader.SetInt(3);
		sprite.AddMember("Shader", shader, allocator);
		Value texture;
		texture.SetInt(-1);
		sprite.AddMember("Texture", texture, allocator);
		Value xpos;
		xpos.SetFloat(0);
		Value ypos;
		ypos.SetFloat(0);
		sprite.AddMember("xpos", xpos, allocator);
		sprite.AddMember("ypos", ypos, allocator);
		Value xsize;
		xsize.SetFloat(1);
		Value ysize;
		ysize.SetFloat(1);
		sprite.AddMember("xsize", xsize, allocator);
		sprite.AddMember("ysize", ysize, allocator);
		Value rot;
		rot.SetFloat(0);
		sprite.AddMember("rot", rot, allocator);
		document.AddMember("Sprite", sprite, allocator);
		
		
		
		Value animation(kObjectType);
		Value rowCount;
		rowCount.SetInt(3);
		animation.AddMember("RowCount", rowCount, allocator);
		Value colCount;
		colCount.SetInt(3);
		animation.AddMember("ColCount", colCount, allocator);
		Value animCount;
		animCount.SetInt(1);
		animation.AddMember("AnimCount", animCount, allocator);
		Value animLength;
		animLength.SetInt(3);
		animation.AddMember("AnimLength", animLength, allocator);
		Value speed;
		speed.SetFloat(1);
		animation.AddMember("Speed", speed, allocator);
		Value animArray;
		animArray.SetArray();
		for (int i = 0; i <= (3) * (1); i++)
			animArray.PushBack(-1, allocator);
		
		animation.AddMember("AnimationSequence", animArray, allocator);
		document.AddMember("Animation", animation, allocator);
				
		
		Value playerController(kObjectType);
		document.AddMember("PlayerController", playerController, allocator);
		Value physics(kObjectType);
		Value velocity;
		velocity.SetFloat(1);
		physics.AddMember("Velocity", velocity, allocator);
		document.AddMember("Physics", physics, allocator);
		
		Value transform(kObjectType);
		Value translation(kObjectType);
		Value X;
		X.SetFloat(0);
		Value Y;
		Y.SetFloat(0);
		translation.AddMember("X", X, allocator);
		translation.AddMember("Y", Y, allocator);
		transform.AddMember("Translation", translation, allocator);
		Value scale(kObjectType);
		Value XScale;
		XScale.SetFloat(1);
		Value YScale;
		YScale.SetFloat(1);
		scale.AddMember("XScale", XScale, allocator);
		scale.AddMember("YScale", YScale, allocator);
		transform.AddMember("Scale", scale, allocator);
		Value rotation;
		rotation.SetFloat(0);
		transform.AddMember("Rotation", rotation, allocator);
		document.AddMember("Transform", transform, allocator);

		Value paralax(kObjectType);
		Value paraspeed;
		paraspeed.SetFloat(0.0f);
		paralax.AddMember("Speed", paraspeed, allocator);
		document.AddMember("Paralax", paralax, allocator);

		Value teleporter(kObjectType);
		Value trigger(kObjectType);
		Value triggerX;
		Value triggerY;
		Vector2D v(0, 0);
		triggerX.SetFloat(v[0]);
		triggerY.SetFloat(v[1]);
		trigger.AddMember("triggerX", triggerX, allocator);
		trigger.AddMember("triggerY", triggerY, allocator);
		teleporter.AddMember("Trigger", trigger, allocator);
		Value reset(kObjectType);
		Value resetX;
		Value resetY;
		Vector2D v2(0, 0);
		resetX.SetFloat(v2[0]);
		resetY.SetFloat(v2[1]);
		reset.AddMember("resetX", resetX, allocator);
		reset.AddMember("resetY", resetY, allocator);
		teleporter.AddMember("Reset", reset, allocator);
		Value lessThan;
		Value xMatters;
		Value yMatters;
		lessThan.SetBool(false);
		xMatters.SetBool(true);
		yMatters.SetBool(true);
		teleporter.AddMember("LessThan", lessThan, allocator);
		teleporter.AddMember("xMatters", xMatters, allocator);
		teleporter.AddMember("yMatters", yMatters, allocator);
		document.AddMember("Teleporter", teleporter, allocator);
				
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		document.Accept(writer);
		std::string filepath("assets/objects/");
		filepath.append("empty" + std::to_string(numEmpty));
		filepath.append(".json");
		numEmpty++;
		std::ofstream of(filepath);
		of << buffer.GetString();

	}

	/**************************************************************************/
	/*!
	\brief Fills a GameObject with data from a JSON file
	\param filename
	Name of the JSON file to read data from
	\param gameObj
	Passed in game object pointer to assign with data from the json file. 
	Will overwrite all data that was put in before calling this function.
	*/
	/**************************************************************************/
	void Deserialize(const char * filename, GameObject * gameObj)
	{
		std::string filepath("assets/objects/");
		filepath.append(filename);
		char * jsonData = StringReadFromFile(filepath.c_str());
		Document document;
		if (document.Parse(jsonData).HasParseError())
			return;

		assert(document.IsObject());

		if (document.HasMember("Sprite"))
		{
			Sprite * sprite = gameObj->AddComponent<Sprite>();
			assert(document.HasMember("Sprite"));
			const Value& spriteComp = document["Sprite"];
			
			assert(spriteComp.HasMember("Shader"));
			assert(spriteComp["Shader"].IsInt());

			assert(spriteComp.HasMember("Texture"));
			assert(spriteComp["Texture"].IsInt());
			
			assert(spriteComp.HasMember("xpos"));
			assert(spriteComp["xpos"].IsFloat());
			
			assert(spriteComp.HasMember("ypos"));
			assert(spriteComp["ypos"].IsFloat());
			
			assert(spriteComp.HasMember("xsize"));
			assert(spriteComp["xsize"].IsFloat());
			
			assert(spriteComp.HasMember("ysize"));
			assert(spriteComp["ysize"].IsFloat());
			
			assert(spriteComp.HasMember("rot"));
			assert(spriteComp["rot"].IsFloat());

			sprite->Init(spriteComp["Shader"].GetInt());
			sprite->SetTexture(spriteComp["Texture"].GetInt());
			sprite->SetPosition(spriteComp["xpos"].GetFloat(), spriteComp["ypos"].GetFloat());
			sprite->SetSize(spriteComp["xsize"].GetFloat(), spriteComp["ysize"].GetFloat());
			sprite->SetRotation(spriteComp["rot"].GetFloat());
		}
		if (document.HasMember("Animation"))
		{
			Animation * anim = gameObj->AddComponent<Animation>();
			assert(document.HasMember("Animation"));
			const Value& animComp = document["Animation"];

			assert(animComp.HasMember("RowCount"));
			assert(animComp["RowCount"].IsInt());

			assert(animComp.HasMember("ColCount"));
			assert(animComp["ColCount"].IsInt());

			assert(animComp.HasMember("AnimCount"));
			assert(animComp["AnimCount"].IsInt());

			assert(animComp.HasMember("AnimLength"));
			assert(animComp["AnimLength"].IsInt());

			assert(animComp.HasMember("Speed"));
			assert(animComp["Speed"].IsFloat());
			anim->Init(animComp["RowCount"].GetInt(), animComp["ColCount"].GetInt(),animComp["AnimCount"].GetInt(), animComp["AnimLength"].GetInt() - 1, animComp["Speed"].GetFloat());
			int i = 0;
			int width = animComp["ColCount"].IsInt();
			assert(animComp.HasMember("AnimationSequence"));
			for (auto &v : animComp["AnimationSequence"].GetArray())
			{
				int row = i % width;
				int col = i / width;
				(*anim)[row][col] = v.GetInt();
				i++;
			}
		}
		if (document.HasMember("PlayerController"))
		{
			assert(document.HasMember("PlayerController"));
			PlayerController * pc = gameObj->AddComponent<PlayerController>();
		}
		if (document.HasMember("Physics"))
		{
			Physics * phys = gameObj->AddComponent<Physics>();
			assert(document.HasMember("Physics"));
			const Value& physComp = document["Physics"];

			assert(physComp.HasMember("Velocity"));
			const Value& velocity = physComp["Velocity"];
			assert(velocity.HasMember("XVel"));
			assert(velocity["XVel"].IsFloat());
			assert(velocity.HasMember("YVel"));
			assert(velocity["YVel"].IsFloat());
			Vector2D vel;
			vel[0] = velocity["XVel"].GetFloat();
			vel[1] = velocity["YVel"].GetFloat();
			phys->Velocity(vel);
			
		}
		if (document.HasMember("Transform"))
		{
			Transform * transform = gameObj->AddComponent<Transform>();
			assert(document.HasMember("Transform"));
			const Value& transComp = document["Transform"];
			assert(transComp.HasMember("Translation"));
			const Value& translation = transComp["Translation"];
			assert(transComp.HasMember("Scale"));
			const Value& scale = transComp["Scale"];

			assert(translation.HasMember("X"));
			assert(translation["X"].IsFloat());
			assert(translation.HasMember("Y"));
			assert(translation["Y"].IsFloat());

			assert(scale.HasMember("XScale"));
			assert(scale["XScale"].IsFloat());
			assert(scale.HasMember("YScale"));
			assert(scale["YScale"].IsFloat());

			assert(transComp.HasMember("Rotation"));
			assert(transComp["Rotation"].IsFloat());

			Vector2D tVec;
			tVec.Set(translation["X"].GetFloat(), translation["Y"].GetFloat());

			Vector2D tScale;
			tScale.Set(scale["XScale"].GetFloat(), scale["YScale"].GetFloat());

			transform->Translation(tVec);
			transform->Scale(tScale);
			transform->Rotation(transComp["Rotation"].GetFloat());
		}
		if (document.HasMember("Paralax"))
		{
			assert(document.HasMember("Paralax"));
			Paralax * paralax = gameObj->AddComponent<Paralax>();
			const Value& plax = document["Paralax"];
			assert(plax.HasMember("Speed"));
			assert(plax["Speed"].IsFloat());
			paralax->Speed(plax["Speed"].GetFloat());
		}
		if (document.HasMember("Teleporter"))
		{
			assert(document.HasMember("Teleporter"));
			Teleporter * teleporter = gameObj->AddComponent<Teleporter>();
			const Value& tele = document["Teleporter"];
			assert(tele.HasMember("Trigger"));
			const Value& trig = tele["Trigger"];
			assert(trig.HasMember("triggerX"));
			assert(trig["triggerX"].IsFloat());
			assert(trig.HasMember("triggerY"));
			assert(trig["triggerY"].IsFloat());
			Vector2D trigger(trig["triggerX"].GetFloat(), trig["triggerY"].GetFloat());
			assert(tele.HasMember("Reset"));
			const Value& rese = tele["Reset"];
			assert(rese.HasMember("resetX"));
			assert(rese["resetX"].IsFloat());
			assert(rese.HasMember("resetY"));
			assert(rese["resetY"].IsFloat());
			Vector2D reset(rese["resetX"].GetFloat(), rese["resetY"].GetFloat());
			assert(tele.HasMember("LessThan"));
			assert(tele.HasMember("xMatters"));
			assert(tele.HasMember("yMatters"));
			assert(tele["LessThan"].IsBool());
			assert(tele["xMatters"].IsBool());
			assert(tele["yMatters"].IsBool());
			teleporter->SetResetPosition(reset);
			teleporter->SetTriggerPosition(trigger, tele["LessThan"].GetBool(), tele["xMatters"].GetBool(), tele["yMatters"].GetBool());

		}
	}

	/**************************************************************************/
	/*!
	\brief Saves the current state of a GameObj to a file with the name of the object and "Last Save"
	\param gameObj
	Game Object to save. Probably the player.
	*/
	/**************************************************************************/
	void SaveSerialize(GameObject * gameObj)
	{
		Document document;
		document.SetObject();
		Document::AllocatorType& allocator = document.GetAllocator();
		Value goName;
		goName.SetString(gameObj->name.c_str(), gameObj->name.size());
		document.AddMember("Game Object Name", goName, allocator);
		Sprite * goSpr = gameObj->GetComponent<Sprite>();
		Animation * goAnim = gameObj->GetComponent<Animation>();
		PlayerController * goPc = gameObj->GetComponent<PlayerController>();
		Physics * goPhys = gameObj->GetComponent<Physics>();
		Transform * goTransform = gameObj->GetComponent<Transform>();
		Paralax * goParalax = gameObj->GetComponent<Paralax>();
		Teleporter * goTele = gameObj->GetComponent<Teleporter>();
		if (goSpr)
		{
			Value sprite(kObjectType);
			Value shader;
			shader.SetInt(3);
			sprite.AddMember("Shader", shader, allocator);
			Value texture;
			texture.SetInt(0);
			sprite.AddMember("Texture", texture, allocator);
			Value xpos;
			glm::vec2 pos = goSpr->GetPosition();
			xpos.SetFloat(pos.x);
			Value ypos;
			ypos.SetFloat(pos.y);
			sprite.AddMember("xpos", xpos, allocator);
			sprite.AddMember("ypos", ypos, allocator);
			Value xsize;
			glm::vec2 size = goSpr->GetSize();
			xsize.SetFloat(size.x);
			Value ysize;
			ysize.SetFloat(size.y);
			sprite.AddMember("xsize", xsize, allocator);
			sprite.AddMember("ysize", ysize, allocator);
			Value rot;
			rot.SetFloat(goSpr->GetRotation());
			sprite.AddMember("rot", rot, allocator);
			document.AddMember("Sprite", sprite, allocator);
		}
		if (goAnim)
		{
			Value animation(kObjectType);
			Value rowCount;
			rowCount.SetInt(goAnim->GetRowCount());
			animation.AddMember("RowCount", rowCount, allocator);
			Value colCount;
			colCount.SetInt(goAnim->GetColCount());
			animation.AddMember("ColCount", colCount, allocator);
			Value animCount;
			animCount.SetInt(goAnim->GetAnimationCount());
			animation.AddMember("AnimCount", animCount, allocator);
			Value animLength;
			animLength.SetInt(goAnim->GetAnimationLength());
			animation.AddMember("AnimLength", animLength, allocator);
			Value speed;
			speed.SetFloat(goAnim->GetAnimationSpeed());
			animation.AddMember("Speed", speed, allocator);
			Value animArray;
			animArray.SetArray();
			int* sequences = goAnim->GetAnimationSequence();
			for (int i = 0; i < (goAnim->GetAnimationLength()) * (goAnim->GetAnimationCount()); i++)
			{
				animArray.PushBack(sequences[i], allocator);
			}
			animation.AddMember("AnimationSequence", animArray, allocator);
			document.AddMember("Animation", animation, allocator);
		}
		if (goPc)
		{
			Value playerController(kObjectType);
			document.AddMember("PlayerController", playerController, allocator);
		}
		if (goPhys)
		{
			Value physics(kObjectType);
			Value velocity(kObjectType);
			Vector2D vel = goPhys->Velocity();
			Value XVel;
			Value YVel;
			XVel.SetFloat(vel[0]);
			YVel.SetFloat(vel[1]);
			velocity.AddMember("XVel", XVel, allocator);
			velocity.AddMember("YVel", YVel, allocator);
			physics.AddMember("Velocity", velocity, allocator);
			document.AddMember("Physics", physics, allocator);
		}
		if (goTransform)
		{
			Value transform(kObjectType);
			Value translation(kObjectType);
			Vector2D translate = goTransform->Translation();
			Value X;
			X.SetFloat(translate[0]);
			Value Y;
			Y.SetFloat(translate[1]);
			translation.AddMember("X", X, allocator);
			translation.AddMember("Y", Y, allocator);
			transform.AddMember("Translation", translation, allocator);
			Value scale(kObjectType);
			Vector2D scalar = goTransform->Scale();
			Value XScale;
			XScale.SetFloat(scalar[0]);
			Value YScale;
			YScale.SetFloat(scalar[1]);
			scale.AddMember("XScale", XScale, allocator);
			scale.AddMember("YScale", YScale, allocator);
			transform.AddMember("Scale", scale, allocator);
			Value rotation;
			rotation.SetFloat(goTransform->Rotation());
			transform.AddMember("Rotation", rotation, allocator);
			document.AddMember("Transform", transform, allocator);

		}
		if (goParalax)
		{
			Value paralax(kObjectType);
			Value speed;
			speed.SetFloat(goParalax->Speed());
			paralax.AddMember("Speed", speed, allocator);
			document.AddMember("Paralax", paralax, allocator);
		}
		if (goTele)
		{
			Value teleporter(kObjectType);
			Value trigger(kObjectType);
			Value triggerX;
			Value triggerY;
			Vector2D v(0, 0);
			triggerX.SetFloat(v[0]);
			triggerY.SetFloat(v[1]);
			trigger.AddMember("triggerX", triggerX, allocator);
			trigger.AddMember("triggerY", triggerY, allocator);
			teleporter.AddMember("Trigger", trigger, allocator);
			Value reset(kObjectType);
			Value resetX;
			Value resetY;
			Vector2D v2(0, 0);
			resetX.SetFloat(v2[0]);
			resetY.SetFloat(v2[1]);
			reset.AddMember("resetX", resetX, allocator);
			reset.AddMember("resetY", resetY, allocator);
			teleporter.AddMember("Reset", reset, allocator);
			Value lessThan;
			Value xMatters;
			Value yMatters;
			lessThan.SetBool(false);
			xMatters.SetBool(true);
			yMatters.SetBool(true);
			teleporter.AddMember("LessThan", lessThan, allocator);
			teleporter.AddMember("xMatters", xMatters, allocator);
			teleporter.AddMember("yMatters", yMatters, allocator);

			document.AddMember("Teleporter", teleporter, allocator);
		}

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		document.Accept(writer);
		std::string filepath("assets/objects/");
		filepath.append(gameObj->name.c_str());
		filepath.append("LastSave.json");
		std::ofstream of(filepath);
		of << buffer.GetString();


	}
}