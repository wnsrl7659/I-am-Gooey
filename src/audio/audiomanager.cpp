/******************************************************************************/
/*!
\file   audiomanager.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Audio manager class, which handles all audio events, and any tweaking of them,
  through integration with FMOD Studio

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <sstream> // stringstream
#include <vector> // vector
#include <fstream> // ifstream
#include <iostream> // cout, endl
#include <string> // string
#include <filesystem>

#include "audiomanager.h"
#include "trace/trace.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "../messaging/messaging.h"
#include "audio.h"
#include "../settings/settings.h"
#include "../system/countdowntimer.h"
#include "../system/timermanager.h"
#include "../system/common_defines.h"
#include "../spaces/spacemanager.h"

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

static FMOD_RESULT result; // result for error checking fmod functions

static std::string bankPath = "assets/Radiant_FMOD/Desktop/";

static std::filesystem::file_time_type guid_write;

/**************************************************************************/
	  /*!
		\brief Error checks the result of FMOD functions
		\param r
		  the FMOD_RESULT being checked
		\param filename
		  the file the the check is occuring from
		\return line
		  the line number the check is occuring on
	  */
/**************************************************************************/
static void ERRCHECK_fn(FMOD_RESULT r, const char * filename, int line)
{
	if (r != FMOD_OK)
	{
		std::stringstream os;
		os << "Error! (" << r << ") " << "\"" << FMOD_ErrorString(r) << "\"" << " in " << filename << " on line " << line;
		Trace::Message(os.str());
	}
}

/**************************************************************************/
	  /*!
		\brief Constructor
	  */
/**************************************************************************/
AudioManager::AudioManager()
{
	Messaging::messHandler->Attach<AudioManager, std::string, &AudioManager::CreateEventFromString>(this, "audio event");
	Messaging::messHandler->Attach<AudioManager, std::string, &AudioManager::SetUpForSpace>(this, "space event");
  Messaging::messHandler->Attach < AudioManager, PuzzleEvent, &AudioManager::PuzzleHandler>(this, "PuzzleEvent");
  Messaging::messHandler->Attach<AudioManager, InputEvent, &AudioManager::HandleInput>(this, "InputEvent");
}

/**************************************************************************/
	  /*!
		\brief Destructor
	  */
/**************************************************************************/
AudioManager::~AudioManager()
{
	Messaging::messHandler->Detach<AudioManager>(this, "audio event");
	Messaging::messHandler->Detach<AudioManager>(this, "space event");
  Messaging::messHandler->Detach<AudioManager>(this, "level event");
  Messaging::messHandler->Detach<AudioManager>(this, "PuzzleEvent");
  Messaging::messHandler->Detach<AudioManager>(this, "InputEvent");
}

/**************************************************************************/
	  /*!
		\brief Changes snapshots and active keys based on currently created spaces
		\param data
			string from a message determining what space was created/destroyed
	  */
/**************************************************************************/
void AudioManager::SetUpForSpace(std::string * data)
{
	if (data->find("created") != std::string::npos)
	{
		if (data->find("level1") != std::string::npos)
		{
			StopEvent("Music_Title");
			Audio::EventData d;
			d.eventName = "Ambience";
			d.paramName = "Area";
			d.val = 1.0f;
			EditEvent(&d);
			SetSnapshot("Level");

		}
	}
	else if (data->find("destroyed") != std::string::npos)
	{
		if (data->find("level1") != std::string::npos)
		{
			StopEvent("Level_1");
			StopEvent("Ambience");
		}
	}
}

/**************************************************************************/
	  /*!
		\brief Sets the active snapshot
		\param snap
			string with the name of the snapshot
	  */
/**************************************************************************/
void AudioManager::SetSnapshot(std::string snap)
{
  if (curSnapshot_)
  {
    ERRCHECK(curSnapshot_->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
    ERRCHECK(curSnapshot_->release()); 
    curSnapshot_ = nullptr;
  }
  FMOD::Studio::EventDescription * snpDes = snapshots_[snap];
  ERRCHECK(snpDes->createInstance(&curSnapshot_));
  ERRCHECK(curSnapshot_->start());
  std::stringstream ss;
  ss << "Activating snapshot " << snap;
  Trace::Message(ss.str());
}

/**************************************************************************/
	  /*!
		\brief Initializes the FMOD system
	  */
/**************************************************************************/
void AudioManager::Init(void)
{
	// create fmod studio system
	ERRCHECK(FMOD::Studio::System::create(&_system));
	// create low level system
	//ERRCHECK(FMOD::System_Create(&_lowSystem));

	// init fmod systems
	// if the setting for live update is true we'll init with live update
	// we can also turn this off, since doing so requires admin permissions
	// on digipen computers
	if (Settings::liveUpdate)
	{
		ERRCHECK(_system->initialize(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));
	}
	else
	{
		ERRCHECK(_system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
	}

	// parse event data
	ParseGUID("assets/Radiant_FMOD/GUIDS.txt");
}

/**************************************************************************/
	  /*!
		\brief Updates the FMOD system
		\param dt
			time since last update
	  */
/**************************************************************************/
void AudioManager::Update(float)
{
	// set volumes
	UpdateVolumes();
	std::vector<std::string> names; // vector of names of finished events
	_system->update();
	UpdateVolumes();
	for (auto& ev : activeEvents_)
	{
		if (!ev.second)
		{
			continue;
		}
		FMOD_STUDIO_PLAYBACK_STATE state;
		ERRCHECK(ev.second->getPlaybackState(&state));
		if (state == FMOD_STUDIO_PLAYBACK_PLAYING || state == FMOD_STUDIO_PLAYBACK_STARTING)
		{
			continue;
		}
		else
		{
			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				bool p;
				ERRCHECK(ev.second->getPaused(&p));
				if (!p)
				{
					// only mark to be released if stopped and not paused
					names.push_back(ev.first);
				}
			}
		}
	}
	for (std::string& s : names)
	{
		StopEvent(s);
	}
}

void AudioManager::StopAllEvents()
{
	std::vector<std::string> names;
	for (auto& e : activeEvents_)
	{
		if (e.first.Has("Transition")) continue; // don't stop the transition noise, since it's like, a transition
		names.push_back(e.first);
	}
	for (std::string s : names)
	{
		StopEvent(s);
	}
}

/**************************************************************************/
	  /*!
		\brief Shuts down the FMOD system
	  */
/**************************************************************************/
void AudioManager::Shutdown(void)
{
	std::vector<std::string*> names; // stores names to avoid invalidating pointers
	// clear the key set
	keys_.clear();

	// clear snapshots events
	// get names
	for (auto& snp : snapshots_)
	{
		names.push_back(&const_cast<std::string&>(snp.first));
	}
	// release the memory
	for (std::string * nm : names)
	{
		if (snapshots_.count(*nm))
		{
			ERRCHECK(snapshots_[*nm]->releaseAllInstances());
		}
		snapshots_.erase(*nm);
	}
	snapshots_.clear();
	names.clear();

	// free active events
	// first stop all the events and gather names
	for (auto& e : activeEvents_)
	{
		ERRCHECK(e.second->stop(FMOD_STUDIO_STOP_IMMEDIATE));
		names.push_back(&const_cast<std::string&>(e.first));
	}
	// remove the events
	for (const std::string * nm : names)
	{
		if (activeEvents_.count(*nm))
		{
			ERRCHECK(activeEvents_[*nm]->release());
			activeEvents_.erase(*nm);
		}
	}
	activeEvents_.clear();
	names.clear(); // clear the vector to be reused

	// free event descriptions
	// gather names
	for (auto& e : events_)
	{
		names.push_back(&const_cast<std::string&>(e.first));
	}
	// release the memory
	for (const std::string * nm : names)
	{
		if (events_.count(*nm))
		{
			events_.erase(*nm);
		}
	}
	events_.clear();
	names.clear();

	// free bank data
	// get all the names
	for (auto& b : banks_)
	{
		names.push_back(&const_cast<std::string&>(b.first));
	}
	//remove the banks
	for (const std::string * nm : names)
	{
		UnloadBank(*nm);
	}
	banks_.clear();
	names.clear();

	ERRCHECK(_system->unloadAll());
	ERRCHECK(_system->release());
	//ERRCHECK(_lowSystem->release());
}

/**************************************************************************/
	  /*!
		\brief Loads in a .bank file into memory
		\param path
			the filepath to the bank to be opened (FOR FUTURE REVISION)
	  */
/**************************************************************************/
void AudioManager::LoadBank(const char * path)
{
	std::string filepath = (bankPath + path + ".bank");
	FMOD::Studio::Bank * bank;
	ERRCHECK(_system->loadBankFile(filepath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	banks_[path] = bank;
}

bool AudioManager::EventIsRunning(const char * name)
{
	return (activeEvents_.count(name) != 0) ? true : false;
}

/**************************************************************************/
	  /*!
		\brief unloads bank files
	  */
/**************************************************************************/
void AudioManager::UnloadBank(const std::string& name)
{
	if (banks_.count(name))
	{
		banks_.erase(name);
	}
}

void AudioManager::PuzzleHandler(PuzzleEvent * data)
{
	if (data->currState == PuzzleEvent::State::completed)
	{
		// get count of completed puzzles
		FMOD::Studio::EventInstance * mus = nullptr;
		std::array<std::string, 4> levels = { "Level_1", "Level_2", "Level_3", "Ambient_Wind" };
		for (std::string& s : levels)
		{
			if (activeEvents_.count(s) != 0)
			{
				mus = activeEvents_[s];
				break;
			}
		}
		if (mus)
		{
			float par = 0.0f;
			mus->getParameterValue("Puzzle Complete", &par);
			mus->setParameterValue("Puzzle Complete", par + 1.0f);
		}
	}
}

/**************************************************************************/
	  /*!
		\brief Parses GUIDs.txt to get data on events, busses etc from FMOD
		\param filepath
			path to the GUIDs file 
	  */
/**************************************************************************/
void AudioManager::ParseGUID(const char * filepath)
{
	guidfile = filepath;
	guid_write = std::filesystem::last_write_time(filepath);
	std::ifstream gFile(filepath, std::ios::in);
	if (!gFile)
	{
		Trace::Message("Unable to open GUIDs");
	}
  // load entire file into vector
	std::vector<std::string> gLines; // a vector of each line in the GUIDs file
	const int MAXLINE = 256;
	std::string str;
	// grab each line from GUIDs and put it in vector
	while (gFile)
	{
		std::getline(gFile, str);
		gLines.push_back(str);
	}

	////////// bank files ///////////////
	std::vector<std::string> banks; // vector for each line with bank in it
	// grab each line with "bank" in it
	for (auto& l : gLines)
	{
		if (l.find("bank") != std::string::npos)
		{
			banks.push_back(l);
		}
	}
	for (auto& l : banks)
	{
		LoadBank(l.substr(l.find("bank") + 6).c_str());
	}
  // load the master strings bank, which isn't listed
  // in GUIDs.txt but MUST be loaded. lmao I've spent
  // like 5 hours trying to find this problem
  LoadBank("Master Bank.strings");

  //////// end banks //////////////////

  //////////// events //////////////
	// put each line that has the word "event" in it 
	// into a vector
	std::string g("event");
	std::vector<std::string> eve;
	for (auto& l : gLines)
	{
		if (l.find(g) != std::string::npos)
		{
			eve.push_back(l);
		}
	}
	// add the event descriptions to _events
	// parse out the extra data to just have the FMOD studio path
	// for the event
	for (std::string s : eve)
	{
		int ind = s.find(g);
		if (ind == std::string::npos)
		{
			continue;
		}
		BuildEvent((s.substr(s.find(g)).c_str()));
	}
  ///////// end events //////////////////
	///////////// snapshots ////////////////
	std::string snp("snapshot");
	std::vector<std::string> snaps;
	for (auto& l : gLines)
	{
		if (l.find(snp) != std::string::npos)
		{
			snaps.push_back(l);
		}
	}
	for (std::string& s : snaps)
	{
		int ind = s.find(snp);
		if (ind == std::string::npos)
		{
			continue;
		}
		BuildSnapshot((s.substr(s.find(snp)).c_str()));
	}
	////////// end snapshots //////////////

	///////// vca ////////////
	std::string vc("vca");
	std::vector<std::string> vcs;
	for (auto& l : gLines)
	{
		if (l.find(vc) != std::string::npos)
		{
			vcs.push_back(l);
		}
	}
	for (std::string& v : vcs)
	{
		int ind = v.find(vc);
		if (ind == std::string::npos)
		{
			continue;
		}
		BuildVCA((v.substr(v.find(vc)).c_str()));
	}
	////////// end vcas ///////////
	///////// busses /////////////
	std::string b("bus");
	std::vector<std::string> bs;
	for (auto& l : gLines)
	{
		if (l.find(b) != std::string::npos)
		{
			bs.push_back(l);
		}
	}
	for (std::string& bu : bs)
	{
		int ind = bu.find(b);
		if (ind == std::string::npos)
		{
			continue;
		}
		BuildBus((bu.substr(bu.find(b)).c_str()));
	}
}

void AudioManager::BuildBus(std::string path)
{
	FMOD::Studio::Bus * b = nullptr;
	ERRCHECK(_system->getBus(path.c_str(), &b));
	busses_[path.substr(path.find_last_of('/') + 1, std::string::npos).c_str()] = b;
}

void AudioManager::BuildVCA(std::string path)
{
	FMOD::Studio::VCA * vc = nullptr;
	ERRCHECK(_system->getVCA(path.c_str(), &vc));
	vcas_[path.substr(path.find_last_of('/') + 1, std::string::npos).c_str()] = vc;
}

void AudioManager::BuildSnapshot(std::string path)
{
	FMOD::Studio::EventDescription * desc = nullptr;
	ERRCHECK(_system->getEvent(path.c_str(), &desc));
	snapshots_[path.substr(path.find_last_of('/') + 1, std::string::npos).c_str()] = desc;
}

/**************************************************************************/
	  /*!
		\brief Builds Event Descriptions and puts them in a map
		\param path
			fmod studio path for the event (NOT A FILE PATH)
	  */
/**************************************************************************/
void AudioManager::BuildEvent(std::string path)
{
  FMOD::Studio::EventDescription * desc;

	ERRCHECK(_system->getEvent(path.c_str(), &desc));
  events_[path.substr(path.find_last_of('/') + 1, std::string::npos).c_str()] = desc;
}

void AudioManager::StopEvent(std::string path)
{
	if (activeEvents_.count(path))
	{
		ERRCHECK(activeEvents_[path]->stop(FMOD_STUDIO_STOP_IMMEDIATE));
		ERRCHECK(activeEvents_[path]->release());
		activeEvents_.erase(path);
	}
}

/**************************************************************************/
	  /*!
		\brief Creates an EventInstance to be played
		\param eDsc
		  the EventDescription * to be used in creating the instance
	  */
/**************************************************************************/
#undef CreateEvent
FMOD::Studio::EventInstance * AudioManager::CreateEvent(std::string name, FMOD::Studio::EventDescription * eDsc)
{
	// check if the requested event is allowed to have more than one playing at the same time
	if ((restricted_events.find(name) != restricted_events.end()) && (activeEvents_.count(name) != 0))
	{
		return nullptr;
	}

	FMOD::Studio::EventInstance * inst;
	ERRCHECK(eDsc->createInstance(&inst));

	std::stringstream ss;
	ss << name;
	int i = 2;
	while (activeEvents_.count(ss.str()))
	{
		ss.str("");
		ss << name << i;
		++i;
	}

	activeEvents_[ss.str()] = inst;
	std::stringstream oss;
	oss << "Creating sound event: " << ss.str();
	Trace::Message(oss.str());
	return inst;
}

/**************************************************************************/
	  /*!
		\brief Creates an event and plays it from the data in a string 
			   passed by a message
		\param data
			string * with the data about event
	  */
/**************************************************************************/
void AudioManager::CreateEventFromString(std::string * data)
{
  FMOD::Studio::EventInstance * evnt;
  if (data->Has("play"))
  {
    // event is to play audio

    evnt = CreateEvent(data->substr(data->find_first_of(' ') + 1, std::string::npos).c_str(), GetEventDescription(data->substr(data->find_first_of(' ') + 1, std::string::npos).c_str()));
	if (evnt)
	{
		ERRCHECK(evnt->start());
	}
  }
  else if (data->Has("stop"))
  {
    // find event and stop it
	  StopEvent(data->substr(data->find_first_of(' ') + 1, std::string::npos));
    //ERRCHECK(activeEvents_[data->substr(data->find_first_of(' ') + 1, std::string::npos)]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
  }
  else if (data->Has("pause-p"))
  {
	  if (data->Has("all"))
	  {
		  // pause all running events
		  for (auto& e : activeEvents_)
		  {
			  ERRCHECK(e.second->setPaused(true));
		  }
	  }
	  else
	  {
		  std::string event = data->substr(data->find_first_of(' ') + 1, std::string::npos);
		  if (activeEvents_.count(event) != 0)
		  {
			  ERRCHECK(activeEvents_[event]->setPaused(true));
		  }
	  }
  }
  else if (data->Has("unpause"))
  {
	  if (data->Has("all"))
	  {
		  // unpause all running events
		  for (auto& e : activeEvents_)
		  {
			  ERRCHECK(e.second->setPaused(false));
		  }
	  }
	  else
	  {
		  std::string event = data->substr(data->find_first_of(' ') + 1, std::string::npos);
		  if (activeEvents_.count(event) != 0)
		  {
			  ERRCHECK(activeEvents_[event]->setPaused(false));
		  }
	  }
  }
}

/**************************************************************************/
	  /*!
		\brief Handles an input event
		\param data
			pointer to InputEvent class that has the data about what the event was
	  */
/**************************************************************************/
void AudioManager::HandleInput(InputEvent * data)
{
	if (data->key == "m" && data->action == "keyup")
	{
		Settings::musicMuted = !Settings::musicMuted;
	}
}

/**************************************************************************/
	  /*!
		\brief Edits the parameters of an event
		\param data
			pointer to the EventData struct that has data on events
	  */
/**************************************************************************/
void AudioManager::EditEvent(Audio::EventData * data)
{
	FMOD::Studio::EventInstance * evnt = activeEvents_[data->eventName];
	std::stringstream ss;
	ss << "Editing " << data->eventName << "'s parameter " << data->paramName << " to " << data->val;
	Trace::Message(ss.str());
	ERRCHECK(evnt->setParameterValue(data->paramName.c_str(), data->val));
}

/**************************************************************************/
	  /*!
		\brief Gets an event description object
		\param path
			the fmod path of the event
		\return The appropriate event description, or nullptr is none found
	  */
/**************************************************************************/
FMOD::Studio::EventDescription * AudioManager::GetEventDescription(std::string path)
{
	if (events_.count(path))
	{
		return events_[path];
	}
	else return nullptr;
}

void AudioManager::UpdateVolumes()
{
	EditVolume(Settings::musicVol, VolType::Music);
	EditVolume(Settings::sfxVol, VolType::Sfx);
	EditVolume(Settings::masterVol, VolType::Master);
	
}

void AudioManager::EditVolume(int vol, AudioManager::VolType vt)
{
	// clamp vol to 100 max 0 min
	if (vol > 100)
	{
		vol = 100;
	}
	if (vol < 0)
	{
		vol = 0;
	}
	float voll = float(vol) / 100.0f;
	FMOD::Studio::VCA * v = nullptr;
	FMOD::Studio::Bus * mb = nullptr;
	switch(vt)
	{
	case VolType::Master:
		mb = busses_[""];
		if (Settings::masterMuted) voll = 0;
		ERRCHECK(mb->setVolume(voll));
		break;
	case VolType::Music:
		v = vcas_["Music"];
		if (Settings::musicMuted) voll = 0;
		ERRCHECK(v->setVolume(voll));
		break;
	case VolType::Sfx:
		v = vcas_["SFX"];
		if (Settings::sfxMuted) voll = 0;
		ERRCHECK(v->setVolume(voll));
		v = vcas_["Ambience"];
		ERRCHECK(v->setVolume(voll));
		break;
	default:
		break;
	}
}

void AudioManager::UpdateGUID()
{
	if (std::filesystem::last_write_time(guidfile) > guid_write)
	{
		// hotswap in the guids
	}
}

