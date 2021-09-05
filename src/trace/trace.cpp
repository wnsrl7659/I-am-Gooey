/******************************************************************************/
/*!
\file   trace.cpp
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  Takes messages and either prints to a console or a file. Used for debugging
  and aggregating error messages.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <ShlObj.h>


#include "trace.h"
#include "../settings/settings.h"
#include "../system/sysinfo.h"
#include "../system/exceptions.h"

#define _FILEIO
#undef _FILEIO

namespace Trace {

	std::ofstream tFile; // error file
	std::ofstream log; // log file for all debug messages
	static unsigned messageCount;

	static std::array<std::string, 32> messages;
	static unsigned lSize = 0;

	/**************************************************************************/
	/*!
	\brief private function that prints out the error messages
	\param m
	  the message to be printed
	*/
	/**************************************************************************/
	static void PrintMessage(const char * m)
	{
		++lSize;
		if (lSize >= 32)
		{
			lSize = 0;
		}
		messages[lSize] = m;
		if (Settings::debug)
		{
			std::cout << m << std::endl;
		}
	}

	static void PrintError(const char * m)
	{
		++lSize;
		if (lSize >= 32)
		{
			lSize = 0;
		}
		messages[lSize] = m;
		CreateCrashDump();
#ifdef _WIN32
		MessageBox(NULL, "Error!", (std::string("A fatal error occured.\n") + std::string(m)).c_str(), MB_OK);
#endif //  _WIN32
		exit(-1);
	}

	/**************************************************************************/
	/*!
	\brief opens the trace file and any other forms of output
	*/
	/**************************************************************************/
	void Init(void)
	{
		messageCount = 0;
	}

	/**************************************************************************/
	/*!
	\brief converts std::string to const char * and passes it to PrintMessage()
	\param m
	  error message
	*/
	/**************************************************************************/
	void MFunc(std::string m, int l, const char * f)
	{
		// write to files / outstream
		PrintMessage(m.c_str());
	}

	/**************************************************************************/
	/*!
	\brief converts an out string stream * to a const char * and passes it to PrintMessage()
	\param m
	  error message
	*/
	/**************************************************************************/
	void MFunc(std::stringstream  m, int l, const char * f)
	{
		PrintMessage(m.str().c_str());
	}

	/**************************************************************************/
	/*!
	\brief passes the const char * along to PrintMessage
	\param m
	  error message
	*/
	/**************************************************************************/
	void MFunc(const char * m, int l, const char * f)
	{
		PrintMessage(m);
	}

	void ReportError(std::stringstream m, int l, const char * f)
	{
		std::stringstream ss;
		ss << "Error (" << f << ":" << l << "): " << m.str();
		PrintError(ss.str().c_str());
	}

	void ReportError(std::string m, int l, const char * f)
	{
		std::stringstream ss;
		ss << "Error (" << f << ':' << l << "): " << m;
		PrintError(ss.str().c_str());
	}

	void ReportError(const char * m, int l, const char *f)
	{
		std::stringstream ss;
		ss << "Error (" << f << ':' << l << "): " << m;
		PrintError(ss.str().c_str());
	}

	std::string CreateCrashDump(void)
	{
		// get appdata path
		PWSTR path;
		HRESULT result;
		result = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &path); // this is windows only I'm pretty sure
		if (result != S_OK)
		{
			switch (result)
			{
			case E_FAIL:
				std::cout << "SHGetKnownFolderPath failed!" << std::endl;
				exit(-1);
				break;
			case E_INVALIDARG:
				std::cout << "SHGetKnownFolderPath failed, invalid argument!" << std::endl;
				exit(-1);
				break;
			case E_ABORT:
				std::cout << "SHGetKnownFolderPath failed, aborted!" << std::endl;
				exit(-1);
				break;
			case ERROR_FILE_NOT_FOUND:
				std::cout << "SHGetKnownFolderPath failed, file not found!" << std::endl;
				exit(-1);
				break;
			default:
				std::cout << result << std::endl;
				exit(-1);
				break;
			}
		}

		char p[128];

		wcstombs(p, path, 128);

		std::stringstream ss; // path to crashdump folder in game files in %LOCALAPPDATA%\ProgramData
		ss << p << "\\DigiPen";
		// create DigiPen folder if not existing
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		// create Song of Life folder if not existing
		ss << "\\SongOfLife";
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		ss << "\\crashdumps";
		// create crashdump folder if not existing
		if (!std::filesystem::exists(ss.str()))
		{
			std::filesystem::create_directory(ss.str());
		}
		time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ss << '\\' << t;
		// create folder for specific dump
		std::filesystem::create_directory(ss.str());
		ss << "\\crashdump.txt";
		std::ofstream d;
		d.open(ss.str());
		if (d.is_open())
		{
			// hardware info
			d << "Hardware Information\n----------\n";
			d << System::DumpSystemInformation();
			// callstack
			d << "\nCallstack\n----------\n";
			d << System::DumpCallstack();
			// internal messages
			d << "\nLast 32 Messages:\n----------\n";
			std::for_each(messages.cbegin(), messages.cend(), [&d](std::string s) -> void {d << s << '\n'; });
			d.close();
		}
		CoTaskMemFree(path);
		return ss.str();
	}

	std::string GetAppDataLocalLow()
	{
		PWSTR path;
		SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &path);
		char p[128];
		wcstombs(p, path, 128);
		return std::string(p);


	}

	/**************************************************************************/
	/*!
	\brief closes all means of output for error messages
	*/
	/**************************************************************************/
	void Shutdown(void)
	{
	}
}
