/******************************************************************************/
/*!
\file   DebugWindow.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Header for displaying an IMGUI window

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <map>
#include "../animation/animation.h"
#include "../gameobject/gameobject.h"
#include "../spaces/spacemanager.h"

namespace DebugWindow {
		void drawSpaceWindow(std::unordered_map<std::string, Space *>& spaces, SpaceManager& spaceManager);
		void drawFPSWindow();
		void drawWindows();
}