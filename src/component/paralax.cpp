/******************************************************************************/
/*!
\file   paralax.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  derived component for the parallax effect on the main menu, gives the object
  attached a constant movement. Yes I spelled Parallax wrong in the class name

\copyright    All content (c) 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "paralax.h"
#include "../settings/settings.h"
#include "../gameobject/gameobject.h"
#include "../physics/Transform.h"
#include "../system/common_defines.h"

Paralax::Paralax() : Component<Paralax>()
{
}

Paralax::~Paralax()
{
}

void Paralax::Update(float dt)
{
	interval_ -= dt;
	if (interval_ <= 0.0f)
	{
		interval_ = DELTA_TIME;
		Vector2D v = dynamic_cast<Transform *>(owner->GetComponent<Transform>())->Translation();
		switch (dir_)
		{
		case pDir::X:
			v[0] += speed_;
			break;
		case pDir::Y:
			v[1] += speed_;
			break;
		case pDir::Diag:
			v[0] += speed_;
			v[1] += speed_;
			break;
		default:
			break;
		}
		dynamic_cast<Transform *>(owner->GetComponent<Transform>())->Translation(v);
	}
}

void Paralax::Draw() const
{
	if (Settings::graphicDebug)
	{
		// do stuff
	}
}

void Paralax::Speed(float s)
{
	speed_ = s;
}

float Paralax::Speed() const
{
	return speed_;
}

void Paralax::Direction(Paralax::pDir d)
{
  dir_ = d;
}

Paralax::pDir Paralax::Direction() const
{
  return dir_;
}

float Paralax::Interval() const
{
	return interval_;
}
