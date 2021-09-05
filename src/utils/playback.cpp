/******************************************************************************/
/*!
\file	playback.cpp
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Captures input events from player so we could play back the gameplay
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <sstream>
#include <filesystem>
#include <fstream>

#include "playback.h"
#include "../messaging/messaging.h"
#include "../messaging/InputEvent.h"
#include "../system/exceptions.h"
#include "utils.h"
#include "../timer/timer.h"

InputCatcher::InputCatcher()
{
	Messaging::messHandler->Attach<InputCatcher, InputEvent, &InputCatcher::Handler>(this, "InputEvent");
}

InputCatcher::~InputCatcher()
{
	if (running_)
	{
		Stop();
	}
	Messaging::messHandler->Detach<InputCatcher>(this, "InputEvent");
}

void InputCatcher::Start()
{
	std::stringstream ss;
	ss << Utils::GetGameDataFolder();
	ss << "\\PlaybackInfo";
	if (!std::filesystem::exists(ss.str()))
	{
		try
		{
			std::filesystem::create_directories(ss.str());
		}
		catch (std::filesystem::filesystem_error& e)
		{
			throw SOLException(System::EType::FILESYSTEM, e.what());
		}
	}
	ss << "\\" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	try
	{
		std::filesystem::create_directory(ss.str());
	}
	catch (std::filesystem::filesystem_error& e)
	{
		throw SOLException(System::EType::FILESYSTEM, e.what());
	}
	ss << "\\playback.dat";
	f_.open(ss.str());
	tm_.Start();
	running_ = true;
}

void InputCatcher::Handler(InputEvent * data)
{
	if (running_)
	{
		double ms = tm_.GetMilliseconds();
		events_.push_back(std::pair<double, InputEvent>(ms, *data));
	}
}

void InputCatcher::Stop()
{
	if (running_)
	{
		tm_.Stop();

		
			std::for_each(events_.cbegin(), events_.cend(), [this](auto p) -> void {
				f_ << p.first << ":" << p.second << "\n";
			});
			f_.close();

		running_ = false;
	}
}

PlaybackMachine::PlaybackMachine()
{
}

PlaybackMachine::~PlaybackMachine()
{
}

void PlaybackMachine::LoadPlaybackFile(std::string file)
{
	std::ifstream f;
	f.open(file);
	events_.clear();
	if (f.is_open())
	{
		std::string buffer;
		while (std::getline(f, buffer))
		{
			// get timestamp
			std::stringstream ss;
			ss << buffer.substr(0, buffer.find(":"));
			double time;
			ss >> time;
			// get event data;
			std::string key, action;
			key = buffer.substr(buffer.find(":") + 1, buffer.find_last_of(",") - buffer.find(":") - 1);
			action = buffer.substr(buffer.find_last_of(",") + 1, std::string::npos);
			InputEvent e(key, action);
			events_.push_back(std::pair<double, InputEvent>(time, e));
			buffer.clear();
		}
		cfile_ = file;
	}
}

std::string PlaybackMachine::CurrentFile() const
{
	return cfile_;
}

void PlaybackMachine::StartPlayback()
{
	running_ = true;
	it_ = events_.begin();
	tm_.Start();
}

void PlaybackMachine::StopPlayback()
{
	running_ = false;
	tm_.Stop();
}

void PlaybackMachine::Update()
{
	if (running_)
	{
		if (it_ == events_.end())
		{
			StopPlayback();
			return;
		}
		if ((*it_).first <= tm_.GetMilliseconds())
		{
			Messaging::messHandler->HandleMessage<InputEvent>("InputEvent", &(*it_).second);
			++it_;
		}
	}
}

namespace Playback
{
	PlaybackMachine * ply = nullptr;

	void Init()
	{
		ply = new PlaybackMachine;
	}

	void Update()
	{
		if (ply)
		{
			ply->Update();
		}
	}

	void Shutdown()
	{
		delete ply;
		ply = nullptr;
	}

	void Start()
	{
		ply->StartPlayback();
	}

	void Stop()
	{
		ply->StopPlayback();
	}
}

namespace InputCapture
{
	InputCatcher * icatcher = nullptr;

	void Init()
	{
		icatcher = new InputCatcher();
	}

	void Shutdown()
	{
		delete icatcher;
		icatcher = nullptr;
	}
}
