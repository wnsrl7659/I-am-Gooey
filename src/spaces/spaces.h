/******************************************************************************/
/*!
\file   spaces.h
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

#ifndef SPACES_H
#define SPACES_H

#undef GetObject

#include <map>

#include "../gameobject/gameobject.h"

struct ObjComp
{
	bool operator()(const std::pair<std::string, GameObject *> &a, const std::pair<std::string, GameObject *> &b) const
	{
		return a.second < b.second;
	}
};

class Space
{
public:

	Space() { objects.max_load_factor(0.1f); };
	virtual ~Space();

	std::string name;

	GameObject * AddObject(const std::string& name);
	void RemoveObject(const std::string& name);
	void MarkObjectForRemoval(const std::string& name);
	GameObject * GetObject(const std::string& name) const;

	void Update(float dt);
	void Draw() const;
	bool Active();
	void Pause();
	void Unpause();

	void SetAllAlpha(float a);

	std::unordered_map<std::string, GameObject *>* GetObjectMap();
	bool removing_ = false;
private:
	std::unordered_map<std::string, GameObject *> objects;
	bool active_ = true;
	bool objectsNeedingRemoval_ = false;
};

namespace Spaces
{
	
}

#endif // !SPACES_H
