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

#include "onewaycollider.h"
#include "../gameobject/gameobject.h"
#include "../physics/Physics.h"
#include "../settings/settings.h"

template<typename T>
static T squareFunc(T value)
{
	return value * value;
}

OneWayCollider::OneWayCollider() : Collider()
{
}

OneWayCollider::~OneWayCollider()
{
}

void OneWayCollider::SetOrientation(OneWayCollider::ColOrientation o)
{
	orientation_ = o;
}

void OneWayCollider::Draw() const
{
}

bool OneWayCollider::isColliding(Collider * other)
{
	colliding_ = false;
	if (!other) return false;
	other->SetColliding(false);

	if (!other->owner) return false;
	if (other->owner->removing_) return false;
	if (other->owner == this->owner) return false;

	Physics* phy = other->owner->GetComponent<Physics>();
	float dt = 0.0167f;

	if (!phy) return false;
	Vector2D temp = phy->Velocity() * dt;
	Vector2D center__ = other->Center() + temp; // future position of the other collider

	float yPos = 0.0f;
	float xPos = 0.0f;
	float xMin = 0.0f;
	float xMax = 0.0f;
	float yMin = 0.0f;
	float yMax = 0.0f;
	float dist = 0.0f;

	switch (orientation_)
	{
	case OneWayCollider::ColOrientation::up:
		// check which side other is on
		// for this class, the orientation is the direction movement is blocked from
		// So in this case, up, the player cannot move through the collider while 
		// colliding with the top side from above
		yPos = center_[1] + radius; // pos at top of sprite
		xMin = center_[0] - radius; // left boundary
		xMax = center_[0] + radius; // right boundary
		// if player's current (not future) top edge is below, then he can pass through and we don't care
		if ((other->Center()[1] + other->Radius()) < yPos)
		{
			return false;
		}
		// check the x or y bounds (dependent on which direction) using future position
		// check if other right < this left
		if ((center__[0] + other->Radius()) < xMin)
		{
			return false;
		}
		// check if other left > this right
		if ((center__[0] - other->Radius()) > xMax)
		{
			return false;
		}
		// by this point we now know that there is some overlap with our line collider and other's collider
		// check dist from other bottom edge to the line (center + radius?) with future position
		dist = (center__[1] - other->Radius()) - yPos; // our coordinate system has x positive right and y positive up
		// check collision
		if (dist <= 0)
		{
			colliding_ = true;
			other->SetColliding(true);
			Trace::Message(std::string(owner->name + " is collising with " + other->owner->name));
			return true;
		}
		break;
	case OneWayCollider::ColOrientation::left:
		// blocked left to right, open right to left
		xPos = center_[0] - radius;
		yMin = center_[1] - radius;
		yMax = center_[1] + radius;

		// check side
		if ((other->Center()[0] + other->Radius()) > xPos)
		{
			return false;
		}
		//check bounds
		// if other bot is > this top
		if ((center__[1] - other->Radius()) > yMax)
		{
			return false;
		}
		// if other top < this bot
		if ((center__[1] + other->Radius()) < yMin)
		{
			return false;
		}
		// check dist
		dist = yPos - (center__[0] + other->Radius());
		if (dist <= 0)
		{
			colliding_ = true;
			other->SetColliding(true);
			Trace::Message(std::string(owner->name + " is colliding with " + other->owner->name));
			return true;
		}
		break;
	case OneWayCollider::ColOrientation::down:
		yPos = center_[1] - radius;
		xMin = center_[0] - radius;
		xMax = center_[0] + radius;

		// check which side other is on
		if ((other->Center()[1] - other->Radius()) > yPos)
		{
			return false;
		}
		// check bounds
		// other right < this left
		if ((center__[0] + other->Radius()) < xMin)
		{
			return false;
		}
		// other left > this right
		if ((center__[0] - other->Radius()) > xMax)
		{
			return false;
		}
		dist = yPos - (center__[1] + other->Radius());
		if (dist <= 0)
		{
			colliding_ = true;
			other->SetColliding(true);
			Trace::Message(std::string(owner->name + " is colliding with " + other->owner->name));
			return true;
		}

		break;
	case OneWayCollider::ColOrientation::right:
		xPos = center_[0] + radius;
		yMin = center_[1] - radius;
		yMax = center_[1] + radius;

		// check which side other is one
		if ((other->Center()[0] + other->Radius()) < xPos)
		{
			return false;
		}
		//check bounds
		// other bot > this top
		if ((other->Center()[1] - other->Radius()) > yMax)
		{
			return false;
		}
		// other top < this bot
		if ((other->Center()[1] + other->Radius()) < yMin)
		{
			return false;
		}
		dist = (center__[0] - other->Radius()) - xPos;
		if (dist <= 0)
		{
			colliding_ = true;
			other->SetColliding(true);
			Trace::Message(std::string(owner->name + " is colliding with " + other->owner->name));
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}
