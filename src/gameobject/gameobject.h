/******************************************************************************/
/*!
\file   gameobject.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  GameObject class, which contains components (where the real gameplay is)

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <unordered_map>
#include <queue>

#include "../component/component.h"

//Culling things
#include "../physics/Transform.h"
#include "../graphics/Camera.h"
#include "../system/common_defines.h"
#include "../component/button.h"
#include "../component/paralax.h"
#include "../component/pscript.h"

class Space;

class GameObject
{
	struct CompAdd
	{
		BaseComponent * comp;
		std::string name;
	};

public:
	std::string name;
	Space * space = nullptr;
	bool removing_ = false;

	GameObject()
	{
		components.max_load_factor(0.1f);
	}

	~GameObject()
	{
		std::vector<std::string> names; // name vector to avoid the invalidation of pointers
		// gather the name of each component
		for (auto c : components)
		{
			names.push_back(const_cast<char *>(c.first.c_str()));
		}
		// release the data for all the components
		// we can't use the generic removecomponent function
		// because it's templated and we don't know each type
		// attached
		for (std::string nm : names)
		{
			if (components.count(nm))
			{
				BaseComponent* c = components[nm];
				delete c;
				components.erase(nm);
			}
		}
		components.clear();
		names.clear();
	}

	template<typename T>
	T* AddComponent()
	{
		BaseComponent* component = reinterpret_cast<BaseComponent*>(new char[sizeof(T)]);
		component->owner = this;
		components[T::name] = component;
		component = new((T*)component)T;
		component->Init();
		return (T*)component;
	}

	void AddPreallocatedComponent(CompAdd c)
	{
		components[c.name] = c.comp;
		c.comp->Init();
	}

	template<typename T>
	T * AddComponentNextFrame()
	{
		BaseComponent * component = reinterpret_cast<BaseComponent*>(new char[sizeof(T)]);
		component->owner = this;
		component = new((T*)component)T;
		CompAdd c;
		c.comp = component;
		c.name = T::name;
		adding_.push(c);
    return (T*)component;
	}

	template<typename T>
	void RemoveComponent()
	{
		if (components.count(T::name))
		{
			delete components[T::name];
			components.erase(T::name);
		}
	}

  template<typename T>
  void RemoveComponentNextFrame()
  {
    if (components.count(T::name))
    {
      removing_.push(T::name);
    }
  }

  void RemoveComp(std::string comp_name)
  {
    if (components.count(comp_name))
    {
      delete components[comp_name];
      components.erase(comp_name);
    }
  }

	template<typename T>
	T * GetComponent()
	{
		if (components.count(T::name))
		{
			return dynamic_cast<T *>(components[T::name]);
		}
		else return nullptr;
	}

	void Update(float dt)
	{
    // add components
		while (!adding_.empty())
		{
			AddPreallocatedComponent(adding_.front());
			adding_.pop();
		}
    // remove components
    while (!comp_removing_.empty())
    {
      RemoveComp(comp_removing_.front());
      comp_removing_.pop();
    }

		bool isCulled = false;
    p_script * script = GetComponent<p_script>();
		Transform* trs = GetComponent<Transform>();
		UI* ui = GetComponent<UI>();
		Selector* sel = GetComponent<Selector>();
		Number* num = GetComponent<Number>();
		Paralax * para = GetComponent<Paralax>();
		if (trs)
		{
			glm::vec2 c = mainCam.center();
			float camerawidth = mainCam.width() / 2.f;
			float cameraheight = mainCam.height() / 2.f;

			Vector2D pos = trs->Translation();

			Vector2D spritescale = trs->Scale();

			//If the position of the object is outside the camera
			if (abs(pos[0] - c.x) - spritescale[0] / 2.f > camerawidth)
				isCulled = true;

			if (abs(pos[1] - c.y) - spritescale[1] / 2.f > cameraheight)
				isCulled = true;
		}

		//Is culled -- Update Transform, and maybe other components.
		if (isCulled && this->name != "player")
		{
      if (script) script->pre_update(dt);
			if (trs) trs->Update(dt);
			if (ui) ui->Update(dt);
			if (sel) sel->Update(dt);
			if (num) num->Update(dt);
			if (para) para->Update(dt);
		}
		//Not culled -- Update all components
		else
		{
      if (script) script->pre_update(dt);
			for (auto comp : components)
			{
				comp.second->Update(dt);
			}
		}
	}

	void Draw() const
	{
		for (auto comp : components)
		{
			comp.second->Draw();
		}
	}

	bool operator<(GameObject& other)
	{
		return z < other.z;
	}

	void Remove()
	{
		removing_ = true;
	}
	std::unordered_map<std::string, BaseComponent *> components;
	//std::map<std::string, BaseComponent *, ComponentComparator> components; // this cuts about 30 fps

	unsigned z = 0; // draw order, smaller number draws first



	std::queue<CompAdd> adding_;
  std::queue<std::string> comp_removing_;
};

#endif
