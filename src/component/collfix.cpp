
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


#include "collfix.h"
#include "../spaces/spacemanager.h"
#include "collider.h"
#include "../spaces/spaces.h"
#include "../gameobject/gameobject.h"
#include "../component/onewaycollider.h"
#include "../settings/settings.h"

int tempflag = 1;
int pmflag = 1;


namespace fix
{

  // temp function that checks collision much faster than the 
  // collider handler does
  void UpdateCollisions()
  {
    if (!Settings::collisions) return;
    Space * plySpace = Spaces::spaceManager->GetSpace("9999 playerspace");
    if (!plySpace) return;
    GameObject * player = plySpace->GetObject("player");
    if (!player) return;
    Collider * col = dynamic_cast<Collider *>(player->GetComponent<Collider>());
    if (!col) return;

	/*auto cols = Data::dManager->GetColliderVector();

	for (auto c : *cols)
	{
		if (c)
		{
			if (col->IsColliding(c))
			{
				col->HandleCollision(c->owner);
				c->HandleCollision(player);
			}
		}
	}*/
	auto sp = Spaces::spaceManager->GetSpaces();
	for (auto s : *sp)
	{
		auto gs = s.second->GetObjectMap();
		for (auto& g : *gs)
		{
			Collider * c = dynamic_cast<Collider *>(g.second->GetComponent<Collider>());
			if (c)
			{
				if (col->IsColliding(c))
				{
					col->HandleCollision(c->owner);
					c->HandleCollision(col->owner);
				}
			}
		}
	}
  }
}
