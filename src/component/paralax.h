/******************************************************************************/
/*!
\file   paralax.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  derived component for the paralax effect on the main menu, gives the object 
  attached a constant movement

\copyright    All content (c) 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef PARALAX_H
#define PARALAX_H

#include "component.h"

class Paralax : public Component<Paralax>
{
public:
  enum class pDir
  {
    X = 0,
    Y,
    Diag,
  };

	COMPONENT_NAME(Paralax);

	Paralax();
	virtual ~Paralax();

	virtual void Update(float dt);
	virtual void Draw() const;

	void Speed(float s);
	float Speed() const;

  void Direction(pDir d);
  pDir Direction() const;

  float Interval() const;

private:
	float speed_ = 0.0f;
	float interval_ = DELTA_TIME - 0.012f;
  pDir dir_ = pDir::X;
};

#endif // !PARALAX_H
