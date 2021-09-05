/******************************************************************************/
/*!
\file   audio.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Audio namespace, for setting up, shutting down, and updating the audio manager

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

class AudioManager;

#include <string>

#ifndef AUDIO_H
#define AUDIO_H

namespace Audio
{
	extern AudioManager * aManager; // audio manager object

	void Init(void);

	void Update(float dt);

	void Shutdown(void);

	void PauseAll(void *);

	void ReturnToLevelSnap(void *);

  struct EventData
  {
    std::string eventName; // name of the event
    std::string paramName; // name of parameter 
    float val;             // value to set parameter with
  };

  void EditParam(EventData * d);
}

#endif