/******************************************************************************/
/*!
\file   collider.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  collider component for colliding

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <sstream>

#include "collider.h"
#include "../gameobject/gameobject.h"
#include "../settings/settings.h"
#include "../physics/Physics.h"


#include "../trace/trace.h"

template<typename T>
static T squareFunc(T value)
{
	return value * value;
}

bool Collider::BoxColliding(Collider * other)
{
	float distanceSquare = 0.0f; // distance between two colliders

	Physics* phy = owner->GetComponent<Physics>();
	float dt = 0.0167f;

	Vector2D temp = phy->Velocity() * dt;
	Vector2D center__ = center_ + temp;

	distanceSquare = float(squareFunc(center__[0] - other->Center()[0]) + squareFunc(center__[1] - other->Center()[1]));

	float distanceSquareX = 0.0f; // distance between two colliders
	float distanceSquareY = 0.0f; // distance between two colliders

	distanceSquareX = float(abs(center__[0] - other->Center()[0]));
	distanceSquareY = float(abs(center__[1] - other->Center()[1]));

	// if (distanceSquare <= ((radius + other->radius) * (radius + other->radius)))
	if (distanceSquareX <= (radius + other->radius) && distanceSquareY <= (radius + other->radius))
	{
		colliding_ = true;
		other->colliding_ = true;
		std::stringstream ss;
		ss << owner->name << " is colliding with " << other->owner->name;
		Trace::Message(ss.str());
		return true;
	}
	return false;
}

bool Collider::OneWayColliding(Collider * other)
{
	// OTHER IS NOT PLAYER
	// THIS IS PLAYER AND OTHER IS THE LEDGE

	Physics* phy = owner->GetComponent<Physics>(); // player's velocity
	float dt = 0.0167f; // dt

	if (!phy) return false; // return if player has no velocity (for some reason)
	Vector2D temp = phy->Velocity() * dt;
	Vector2D center__ = Center() + temp; // future position of the player's collider
	
	int key = 0;
	if (phy->Velocity()[0] > 0 && phy->Velocity()[1] == 0)
	{
		key = 1; //right
	}
	else if (phy->Velocity()[0] < 0 && phy->Velocity()[1] == 0)
	{
		key = 2; //left
	}
	else if (phy->Velocity()[1] > 0 && phy->Velocity()[0] == 0)
	{
		key = 3; //up
	}
	else if (phy->Velocity()[1] < 0 && phy->Velocity()[0] == 0)
	{
		key = 4; //down
	}

	float yPos = 0.0f; // ledge y boundary
	float xPos = 0.0f; // ledge x boundary
	float xMin = 0.0f; // ledge x low
	float xMax = 0.0f; // ledge x high
	float yMin = 0.0f; // ledge y low
	float yMax = 0.0f; // ledge y high
	float dist = 0.0f; // distance
	float distanceSquareX = 0.0f; // distance between two colliders
	float distanceSquareY = 0.0f; // distance between two colliders

	distanceSquareX = float(abs(center__[0] - other->Center()[0]));
	distanceSquareY = float(abs(center__[1] - other->Center()[1]));

	// check dir
	if (distanceSquareX <= (radius + other->radius) && distanceSquareY <= (radius + other->radius))
	{
		switch (other->d_)
		{
		case Collider::ColDir::Up:
			if (key == 3)
			{
				return false;
			}
			break;
		case Collider::ColDir::Left:
			if (key == 2)
			{
				return false;
			}
			break;
		case Collider::ColDir::Down:
			if (key == 4)
			{
				return false;
			}
			break;
		case Collider::ColDir::Right:
			if (key == 1)
			{
				return false;
			}
			break;
		default:
			return false;
		}
		colliding_ = true;
		other->colliding_ = true;
		Trace::Message(std::string(owner->name + " is colliding with " + other->owner->name));
		return true;
	}
	return false;
	
}


Collider::Collider() : Component<Collider>()
{
}

Collider::~Collider()
{
}

void Collider::Update(float dt)
{
	if (trans_)
	{
		
		
			//radius = trans_->Scale()[0] / 2.25f;
			center_ = trans_->Translation() + offset_;
	}
}

void Collider::Draw() const
{
}

Transform * Collider::GetTransform() const
{
  return trans_;
}

void Collider::SetTransform(Transform * t)
{
  trans_ = t;
  center_ = trans_->Translation();
  radius = (trans_->Scale())[0] / 2.25f;
}

void Collider::SetType(Collider::ColType type)
{
	t = type;
}

void Collider::SetDir(Collider::ColDir dir)
{
	if (t == ColType::OneWay)
	{
		d_ = dir;
	}
}

Collider::ColType Collider::GetType() const
{
	return t;
}

Collider::ColDir Collider::GetDir() const
{
	return d_;
}

void Collider::SetHandler(void(*f)(GameObject*, GameObject*))
{
  handler_ = f;
}

void Collider::HandleCollision(GameObject * other)
{
  colliding_ = true;
  if (other && owner)
  {
	  if (owner->removing_ || other->removing_) return;
	  handler_(this->owner, other);
  }
}

bool Collider::IsColliding(Collider * other)
{
  colliding_ = false;
  if (!other) return false;
  other->colliding_ = false;
  if (!other->owner) return false;
  if (other->owner->removing_) return false;
  if (other->owner == this->owner) return false;

  bool c;

  switch (other->GetType())
  {
  case ColType::Box:
	  c = BoxColliding(other);
	  break;
  case ColType::OneWay:
	  c =  OneWayColliding(other);
	  break;
  }

  return c;
 
}

Vector2D Collider::GetOffset()
{
		return offset_;
}

void Collider::SetOffset(Vector2D o)
{
  offset_ = o;
}
