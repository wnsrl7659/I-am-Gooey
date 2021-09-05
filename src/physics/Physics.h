/*! ****************************************************************************
\file                Physics.h
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

#ifndef __PHYSICS_H
#define __PHYSICS_H

// Include Files                ////////////////////////////////////////////////

#include "../component/component.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "../component/component.h"

// Public  Objects              ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
*******************************************************************************/
class Physics : public Component<Physics>
{
private:

  // Members              ///////////////////////
  Vector2D m_Velocity;

public:

  // Members              ///////////////////////

  COMPONENT_NAME(Physics);

	float friction = 0.f;
  static float s_PlayerVelocity;

  // Con-/De- structors   ///////////////////////

  Physics() = default;

  Physics(Physics const &) = default;
  Physics(Physics &&) = default;

  virtual ~Physics() = default;

  // Operators            ///////////////////////

  Physics & operator=(Physics const &) = default;
  Physics & operator=(Physics &&) = default;

  // Accossors/Mutators   ///////////////////////

  // Functions            ///////////////////////

  Vector2D Velocity() const;
  void Velocity(Vector2D);

  virtual void Init();
  virtual void Update(float dt);
  virtual void Draw() const {};

private:
	float interval_ = DELTA_TIME - 0.015f;
	bool ismoved;
	Vector2D tempvel;

  // Functions                  ///////////////////////

}; // class Physics

#endif // __PHYSICS_H
