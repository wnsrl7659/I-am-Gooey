/******************************************************************************/
/*!
\file   onewaycollider.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	collider that can only be collided with from one direction

\copyright    All content (c) 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef ONEWAYCOLLIDER_H
#define ONEWAYCOLLIDER_H

#include "component.h"
#include "collider.h"

class OneWayCollider : public Collider
{
public:
	COMPONENT_NAME(OneWayCollider);

	enum class ColOrientation
	{
		up = 0,
		left,
		down,
		right,
	};

	OneWayCollider();
	~OneWayCollider();

	void SetOrientation(ColOrientation o);

	virtual void Draw() const;

	virtual bool isColliding(Collider * other);

private:
	ColOrientation orientation_;
};

#endif // !ONEWAYCOLLIDER_H
