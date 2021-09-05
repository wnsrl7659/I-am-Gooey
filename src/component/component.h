/******************************************************************************/
/*!
\file   component.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  component base class, from which all component derive from

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <map>
#include <sstream>

#include "../trace/trace.h"
#include "../system/common_defines.h"

#define COMPONENT_NAME(x) static constexpr char const* name = #x

enum CompType
{
	ctAnimation = 0,
	ctController,
	ctPlayerController,
	ctParalax,
	ctTeleporter,
	ctPhysics,
	ctTransform,
	ctCollider,
	ctOWCollider,
	ctButton,
	ctAcorn,
	ctNumber,
	ctSelector,
	ctSprite,
	ctEmitter,
	ctFlower,
	ctDebugDraw,
	ctDebugPath,
	ctFontRender,
  ctp_script,

	ctCompCount,
};

static std::map<std::string, CompType> comps 
{
{"Animation", CompType::ctAnimation},
{"Controller", CompType::ctController},
{"PlayerController", CompType::ctPlayerController},
{"Paralax", CompType::ctParalax},
{"Teleporter", CompType::ctTeleporter},
{"Physics", CompType::ctPhysics},
{"Transform", CompType::ctTransform},
{"Collider", CompType::ctCollider},
{"OneWayCollider", CompType::ctOWCollider},
{"Button", CompType::ctButton},
{"Acorn", CompType::ctAcorn},
{"Number", CompType::ctNumber},
{"Selector", CompType::ctSelector},
{"Sprite", CompType::ctSprite},
{"Emitter", CompType::ctEmitter},
{"Flower", CompType::ctFlower},
{"DebugDraw", CompType::ctDebugDraw},
{"DebugPath", CompType::ctDebugPath},
{"FontRender", CompType::ctFontRender},
{"p_script", CompType::ctp_script},
};


struct ComponentComparator
{
	bool operator()(std::string a, std::string b) const
	{
		return comps[a] < comps[b];
	}
};

class GameObject; // forward reference to game objects

class BaseComponent
{
public:
	const std::string name;

	GameObject * owner;

	BaseComponent(const char * name_) : name(name_) {};
	BaseComponent() = delete;
	virtual ~BaseComponent() {};

	virtual void Init() {};
	virtual void Update(float dt) = 0;
	virtual void Draw() const = 0;
};

template<typename T>
class Component : public BaseComponent
{
public:
	Component() : BaseComponent(T::name) {};
  virtual void Update(float dt) = 0;
  virtual void Draw() const = 0;

};

#endif