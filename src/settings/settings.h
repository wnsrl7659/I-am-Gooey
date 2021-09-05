/******************************************************************************/
/*!
\file   settings.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
stores data for all of the games settings, including default values. Additionally
serializes data from (and eventually to) a data file, as well as being affected
by user changes during runtime.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <map>
#include <string>

namespace Settings
{
	// settings
	extern bool graphicDebug;
	extern bool debug;
	extern bool debugLines;
	extern bool liveUpdate;
	extern int masterVol;
	extern int musicVol;
	extern int sfxVol;
	extern bool masterMuted;
	extern bool musicMuted;
	extern bool sfxMuted;
	extern bool windowed;
	extern bool colorStandard;
	extern bool colorBG;
	extern bool colorRY;
	extern bool colorRG;
	extern bool colorMono;
	extern bool culling;
	extern bool camfollowing;
	extern bool collisions;

	// just for giggles
	extern bool failedToOpen;


	void Init();

  std::map<std::string, bool *> * GetBools();
  std::map<std::string, int *> * GetInts();
}
#endif // !SETTING