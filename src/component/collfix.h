
/******************************************************************************/
/*!
\file   collfix.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  this file was originally for a temporary fix to the collision system, and then
  became the official way to check collisions due to high efficiency and not crashing
  the game

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef COLLFIX_H
#define COLLFIX_H

#include "../physics/Vector2D.h"
#include "../physics/Matrix2D.h"
#include "../gameobject/gameobject.h"
#include "../physics/Transform.h"

#include <vector>

class Collider;

namespace fix
{
  void UpdateCollisions();
}

extern int tempflag;
extern int pmflag;
#endif
