/******************************************************************************/
/*!
\file   settings.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  stores data for all of the games settings, including default values. Additionally
  serializes data from (and eventually to) a data file, as well as being affected
  by user changes during runtime.

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include "settings.h"
#include "../trace/trace.h"
#include "../system/common_defines.h"

namespace Settings
{
#ifdef _DEBUG
	extern bool graphicDebug = true;
#else
  extern bool graphicDebug = false;
#endif // _DEBUG

	extern bool debug = false;
	extern bool debugLines = false;
	extern bool liveUpdate = false;
	extern int masterVol = 0;
	extern int musicVol = 0;
	extern int sfxVol = 0;
	extern bool masterMuted = false;
	extern bool musicMuted = false;
	extern bool sfxMuted = false;
	extern bool windowed = false;
	extern bool colorStandard = false;
	extern bool colorBG = false;
	extern bool colorRY = false;
	extern bool colorRG = false;
	extern bool colorMono = false;
	extern bool culling = false;
	extern bool camfollowing = false;

	extern bool collisions = true;

	extern bool failedToOpen = false;

  std::map<std::string, bool *> boolsettings
  {
  {"graphicdebug", &graphicDebug},
  {"debug", &debug },
  {"debuglines", &debugLines},
  {"liveupdate", &liveUpdate},
  {"mastermuted", &masterMuted},
  {"musicmuted", &musicMuted},
  {"sfxmuted", &sfxMuted},
  {"windowed", &windowed},
  {"colorstandard", &colorStandard},
  {"colorbg", &colorBG},
  {"colorry", &colorRY},
  {"colorrg", &colorRG},
  {"colormono", &colorMono},
  {"culling", &culling},
  {"camfollowing", &camfollowing},
  {"collisions", &collisions},
  {"failedtoopen", &failedToOpen},
  };

  std::map<std::string, int *> intsettings
  {
  {"mastervol", &masterVol},
  {"musicvol", &musicVol},
  {"sfxvol", &sfxVol},
  };

  std::map<std::string, bool *> * GetBools()
  {
    return &boolsettings;
  }

  std::map<std::string, int *> * GetInts()
  {
    return &intsettings;
  }

  void Init()
  {
    std::ifstream f(".\\assets\\settings.dat");
    if (!f.is_open())
    {
      Trace::Message("Settings file failed to open!");
      failedToOpen = false;
      return;
    }

    // load whole file into vector
    std::vector<std::string> lines;
    std::string buffer;
    while (f)
    {
      std::getline(f, buffer);
      lines.push_back(buffer);
    }
    // parse out settings
    // bool settings
    for (auto p : boolsettings)
    {
      for (auto s : lines)
      {
        if (s.empty())
        {
          continue;
        }
        if (s.Has(p.first))
        {
          if (s.Has("true"))
          {
            *(p.second) = true;
          }
          else if (s.Has("false"))
          {
            *(p.second) = false;
          }
          break;
        }
      }
    }
    // int settings
    for (auto p : intsettings)
    {
      for (auto s : lines)
      {
        if (s.empty())
        {
          continue;
        }
        if (s.Has(p.first))
        {
          std::stringstream ss;
          ss << s.substr(s.find_last_of('=') + 1, std::string::npos);
          ss >> (*p.second);
          break;
        }
      }
    }
  }
}
