/******************************************************************************/
/*!
\file   audiomanager.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
Audio manager class, which handles all audio events, and any tweaking of them,
through integration with FMOD Studio

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

/***********************************************************************

TODO:

Stretch: check if a file date has been changed, and load in
the file during runtime (in engine hotswap)

************************************************************************/


#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#undef CreateEvent

#include <unordered_map>
#include <set>

#include "fmod_studio.hpp"
#include "../messaging/message.h"
#include "../messaging/InputEvent.h"
#include "audio.h"
#include "../puzzles/newpuzzle.h"


class AudioManager
{
public:
	/**************************************************************************/
	  /*!
		\brief Default constructor
	  */
	  /**************************************************************************/
	AudioManager();


	/**************************************************************************/
	/*!
		\brief Destructor
	*/
	/**************************************************************************/
	~AudioManager();

	/**************************************************************************/
	  /*!
		\brief Initializes the FMOD system
	  */
	  /**************************************************************************/
	void Init(void);

	/**************************************************************************/
	  /*!
		\brief Updates the FMOD system
		\param dt
			time since last update
	  */
	  /**************************************************************************/
	void Update(float dt);

	/**************************************************************************/
	  /*!
		\brief Shuts down the FMOD system
	  */
	  /**************************************************************************/
	void Shutdown(void);

	/**************************************************************************/
	  /*!
		\brief Loads in a .bank file into memory
		\param path
			the filepath to the bank to be opened (FOR FUTURE REVISION)
	  */
	  /**************************************************************************/
	void LoadBank(const char * path);

	bool EventIsRunning(const char * name);


	/**************************************************************************/
		  /*!
			\brief unloads bank files
		  */
		  /**************************************************************************/
	void UnloadBank(const std::string& name);

	/**************************************************************************/
		  /*!
			\brief Creates an Event struct and stores it in a vector
			\param path
				the description (future)
			\return eDsc
				the event description wanted (for now just ambience)
		  */
		  /**************************************************************************/
	FMOD::Studio::EventDescription * GetEventDescription(std::string path);

	/**************************************************************************/
	/*!
		\brief Sets the currently active snapshot
		\param data
			Name of the snapshot to be set.
	*/
	/**************************************************************************/
	void SetSnapshot(std::string data);

	/**************************************************************************/
	/*!
		\brief Creates an FMOD event from data given in a string
		\param data
			Pointer to a string containing data needed to create event
	*/
	/**************************************************************************/
	void CreateEventFromString(std::string * data);

	/**************************************************************************/
	/*!
		\brief Handler for input events
		\param data
			Pointer to the InputEvent
	*/
	/**************************************************************************/
	void HandleInput(InputEvent * data);

	/**************************************************************************/
	/*!
		\brief Handler for PuzzleEvents
		\param data
			Pointer to the PuzzleEvent class with event data
	*/
	/**************************************************************************/
	void PuzzleHandler(PuzzleEvent * data);

	/**************************************************************************/
	/*!
		\brief Edits a currently running event
		\param data
			Pointer to the data needed to edit the event
	*/
	/**************************************************************************/
	void EditEvent(Audio::EventData * data);

#undef CreateEvent
	/**************************************************************************/
			/*!
			  \brief Creates an EventInstance to be played
		\param name
		  name of the event
			  \param eDsc
				the EventDescription * to be used in creating the instance
				\return inst
					the active event instance
			*/
			/**************************************************************************/
	FMOD::Studio::EventInstance * CreateEvent(std::string name, FMOD::Studio::EventDescription * eDsc);

	/**************************************************************************/
	/*!
		\brief Stops the event, and removes it from the active events
		\param name
			Name of the event to stop
	*/
	/**************************************************************************/
	void StopEvent(std::string name);

	std::unordered_map<std::string, FMOD::Studio::EventInstance *> * GetActiveEvents()
	{
		return &activeEvents_;
	}

	void StopAllEvents();

	enum VolType
	{
		Master = 0,
		Sfx,
		Music,
	};
private:

	/**************************************************************************/
			  /*!
				\brief Parses GUIDs.txt to get data on events, busses etc from FMOD
				\param filepath
					path to the GUIDs file (FOR FUTURE REVISION)
			  */
			  /**************************************************************************/
	void ParseGUID(const char * filepath);

	/**************************************************************************/
	/*!
		\brief (DEPRECATED) Reloads the GUIDs.txt file from disk, and rebuilds
			all the associated events
	*/
	/**************************************************************************/
	void UpdateGUID();

	/**************************************************************************/
		  /*!
			\brief Creates an Event struct and stores it in a vector
			\param path
				fmod studio path for the event (NOT A FILE PATH)
		  */
		  /**************************************************************************/
	void BuildEvent(std::string path);

	/**************************************************************************/
	/*!
		\brief Creates a new snapshot
		\param path
			fmod studio path for the snapshot
	*/
	/**************************************************************************/
	void BuildSnapshot(std::string path);

	/**************************************************************************/
	/*!
		\brief Creates a new VCA
		\param path
			fmod studio path for the vca
	*/
	/**************************************************************************/
	void BuildVCA(std::string path);

	/**************************************************************************/
	/*!
		\brief Builds a new bus
		\param path
			fmod studio path for the bus
	*/
	/**************************************************************************/
	void BuildBus(std::string path);

	/**************************************************************************/
	/*!
		\brief Sets up events and parameters based on a space being 
			built or destroyed
		\param data
			What space was being built/destroyed
	*/
	/**************************************************************************/
	void SetUpForSpace(std::string * data);

	/**************************************************************************/
	/*!
		\brief Sets the volume of a certain type (sfx, music, master)
		\param vol
			level to set the volume to
		\param vt
			The type of volume to adjust
	*/
	/**************************************************************************/
	void EditVolume(int vol, VolType vt);

	/**************************************************************************/
	/*!
		\brief Sets the volumes to those referenced in the settings
	*/
	/**************************************************************************/
	void UpdateVolumes();

	FMOD::Studio::System * _system; // studio system
	FMOD::Studio::EventInstance * curSnapshot_; // current snapshot
	std::unordered_map<std::string, FMOD::Studio::Bank *> banks_; // map of banks and the name of the bank
	std::unordered_map<std::string, FMOD::Studio::EventDescription *> events_; // map of event descriptions and their names
	std::unordered_map<std::string, FMOD::Studio::EventInstance *> activeEvents_; // event of actively running events and their names
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> snapshots_; // list of all snapshots
	std::unordered_map<std::string, FMOD::Studio::VCA *> vcas_; // map of VCAs
	std::unordered_map<std::string, FMOD::Studio::Bus *> busses_; // map of busses

	std::set<std::string> keys_; // currently activated keys
	FMOD::Studio::EventDescription * prevSnapshot_; // previous snapshot, needed for things like the lutespace, where it's only active for a 
													// short time so we need to remember what to reset the snapshot to after it ends
	std::string guidfile;

	std::set<std::string> const restricted_events = // events that aren't allowed to stack plays (play more than one at the same time)
	{
		"Brazier_Conditional",
		"Main_Menu",
		"Level_1",
		"Level_2",
		"Level_3",
		"Brazier",
		"Final_Flame",
	};
};

#endif
