/******************************************************************************/
/*!
\file   housespace.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
 space for the interior of Oliver's house

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "spacemanager.h"
#include "../messaging/messaging.h"

namespace Spaces
{
	void BuildHouse()
	{
		// pass house.json to our parser and let it be serialized
		
		BuildPlayerspace(); // since this is the first place they player will spawn, we'll build the playerspace now. Built last for draw order
	}
}
