/******************************************************************************/
/*!
\file   teleporter.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	used to teleport an object from one position to another

\copyright    All content (c) 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "teleporter.h"
#include "../gameobject/gameobject.h"

Teleporter::Teleporter() : Component<Teleporter>()
{
	trigger_ = Vector2D();
	reset_ = Vector2D();
}

Teleporter::~Teleporter()
{
}

void Teleporter::Update(float dt)
{
	if (!xMatters_ && !yMatters_)
	{
		return;
	}
	Vector2D current = dynamic_cast<Transform *>(owner->GetComponent<Transform>())->Translation();
	bool xTrigger = true;
	bool yTrigger = true;
	if (xMatters_)
	{
		if (lessThan_)
		{
			if (current[0] <= trigger_[0])
			{
				xTrigger = true;
			}
			else
			{
				xTrigger = false;
			}
		}
		else
		{
			if (current[0] >= trigger_[0])
			{
				xTrigger = true;
			}
			else
			{
				xTrigger = false;
			}
		}
	}
	if (yMatters_)
	{
		if (lessThan_)
		{
			if (current[1] <= trigger_[1])
			{
				yTrigger = true;
			}
			else
			{
				yTrigger = false;
			}
		}
		else
		{
			if (current[1] >= trigger_[1])
			{
				yTrigger = true;
			}
			else
			{
				yTrigger = false;
			}
		}
	}

	if (xTrigger && yTrigger)
	{
		dynamic_cast<Transform *>(owner->GetComponent<Transform>())->Translation(reset_);
	}
}

void Teleporter::SetTriggerPosition(Vector2D v, bool lessThan, bool xMatters, bool yMatters)
{
	trigger_ = v;
	lessThan_ = lessThan;
	xMatters_ = xMatters;
	yMatters_ = yMatters;
}

void Teleporter::SetResetPosition(Vector2D v)
{
	reset_ = v;
}