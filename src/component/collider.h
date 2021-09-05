
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

#ifndef TEMPCOL_H
#define TEMPCOL_H

#include "component.h"
#include "../physics/Transform.h"

class Collider : public Component<Collider>
{
public:
	enum class ColType
	{
		Box = 0,
		OneWay,
	};
	enum class ColDir
	{
		Up = 0,
		Right,
		Down,
		Left,

		NoDir,
	};

  COMPONENT_NAME(Collider);

  Collider();
  ~Collider();

  virtual void Update(float dt);
  virtual void Draw() const;

  virtual void SetTransform(Transform * t);
  Transform * GetTransform() const;

  void SetHandler(void(*f)(GameObject *, GameObject *));
  void HandleCollision(GameObject * other);
  void SetType(ColType t);
  void SetDir(ColDir d);
  ColType GetType() const;
  ColDir GetDir() const;

  virtual bool IsColliding(Collider * other);
  bool IsColliding() const {
    return colliding_;
  };

  
  void(*handler_)(GameObject * a, GameObject * b);
  Vector2D Center() const { return center_; };
  float Radius() const { return radius; };

  void SetColliding(bool c) { colliding_ = c; };

  void SetOffset(Vector2D o);
	Vector2D GetOffset();
	std::string coldata;

private:
  
  ColType t = ColType::Box;
  ColDir d_ = ColDir::NoDir;
  bool BoxColliding(Collider * other);
  bool OneWayColliding(Collider * other);

protected:
  Transform * trans_;
  float radius = 0.0;
  Vector2D center_ = Vector2D();
  Vector2D offset_ = Vector2D();

  bool colliding_ = false;
};

#endif // !TEMPCOL_H
