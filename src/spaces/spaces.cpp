/******************************************************************************/
/*!
\file   spaces.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Spaces class, which acts as a container of game objects, similar to a scene or
  gamestate. The space is independent from any other spaces and can live on its
  own or at the same time as others. All spaces must have unique names.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <iostream>

#include "spaces.h"
#include "../graphics/Sprite.h"
#include "../settings/settings.h"

#undef GetObject

Space::~Space()
{
	std::vector<std::string> names; // vector of names to avoid pointer invalidation
	for (auto obj : objects)
	{
		names.push_back(obj.first);
	}
	for (std::string nm : names)
	{
		RemoveObject(nm);
	}
}

GameObject * Space::AddObject(const std::string& name)
{
	GameObject * obj = reinterpret_cast<GameObject*>(new char[sizeof(GameObject)]);
	objects[name] = obj;
	obj = new(obj)GameObject;
	obj->space = this;
	obj->name = name;
	Trace::Message(std::string("Destroying object: " + name + " in space: " + this->name));
	return obj;
}

void Space::RemoveObject(const std::string& name)
{
	Trace::Message(std::string("Destroying object: " + name + " in space: " + this->name));
	if (objects.count(name))
	{
		GameObject* o = objects[name];
		delete  o;
		objects.erase(name);
	}
}

void Space::MarkObjectForRemoval(const std::string& name)
{
	Trace::Message(std::string("Marking " + name + " for removal"));
	if (objects.count(name))
	{
		objects[name]->removing_ = true;
		objectsNeedingRemoval_ = true;
	}
}

GameObject * Space::GetObject(const std::string& name) const
{
	if (objects.count(name))
	{
		return objects.at(name);
	}
	else return nullptr;
}

std::unordered_map<std::string, GameObject *>* Space::GetObjectMap()
{
		return &objects;
}

void Space::Update(float dt)
{
	if (objectsNeedingRemoval_)
	{
		std::vector<std::string> names;
		for (auto obj : objects)
		{
			if (obj.second->removing_)
			{
				names.push_back(*(const_cast<std::string *>(&obj.second->name)));
			}
		}
		for (const std::string s : names)
		{
			RemoveObject(s);
		}
		names.clear();
		objectsNeedingRemoval_ = false;
	}

	if (active_)
	{
		//drawSpaceWindow(*this);
		for (auto obj : objects)
		{
			obj.second->Update(dt);
		}
	}
}

void Space::Draw() const
{
  for (auto obj : objects)
  {
    obj.second->Draw();
  }
}

void Space::Pause()
{
	active_ = false;
}

void Space::Unpause()
{
	active_ = true;
}

bool Space::Active()
{
	return active_;
}

void Space::SetAllAlpha(float a)
{
	for (auto o : objects)
	{
		Sprite * s = o.second->GetComponent<Sprite>();
		if (s)
		{
			s->setAlpha(a);
		}
	}
}
