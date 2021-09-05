/******************************************************************************/
/*!
\file
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief

\copyright    All content (c)2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#include <intrin.h>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#include <ShlObj.h>
#endif // _WIN32

#include <GLFW/glfw3.h>

#include "sysinfo.h"
#include "../settings/settings.h"

namespace System
{
	gpuinfo * gInfo = nullptr;

	cpuinfo::cpuinfo()
	{
		// taken from msdn, prints vendor string of cpu
		std::vector<std::array<int, 4>> data_;
		std::array<int, 4> cpui;
		int nIds = 0;
		int id = 0;

		__cpuid(cpui.data(), 0);
		nIds = cpui[0];
		for (int i = 0; i <= nIds; ++i)
		{
			__cpuidex(cpui.data(), i, 0);
			data_.push_back(cpui);
		}

		char vendor[0x20];
		memset(vendor, 0, sizeof(vendor));
		*reinterpret_cast<int*>(vendor) = data_[0][1];
		*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
		*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
		vendorid_ = vendor;

		// get brand string
		__cpuid(cpui.data(), 0x80000000);
		id = cpui[0];

		std::vector<std::array<int, 4>> exinfo;

		for (int i = 0x80000000; i <= id; ++i)
		{
			__cpuidex(cpui.data(), i, 0);
			exinfo.push_back(cpui);
		}

		if (id >= 0x80000004)
		{
			char brand[0x40];
			memset(brand, 0, sizeof(brand));
			memcpy(brand, exinfo[2].data(), sizeof(cpui));
			memcpy(brand + 16, exinfo[3].data(), sizeof(cpui));
			memcpy(brand + 32, exinfo[4].data(), sizeof(cpui));
			
			model_ = brand;
		}
	}

	cpuinfo::~cpuinfo()
	{
	}

	std::ostream& operator<<(std::ostream& os, cpuinfo const& c)
	{
		os << c.vendorid_ << ' ' << c.model_;
		return os;
	}

	gpuinfo::gpuinfo()
	{
		if (gInfo == nullptr)
		{
			vendor_ = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
			renderer_ = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
		}
		else
		{
			vendor_ = gInfo->vendor_;
			renderer_ = gInfo->renderer_;
		}
	}

	gpuinfo::~gpuinfo()
	{
	}

	std::ostream& operator<<(std::ostream& os, gpuinfo const& g)
	{
		os << g.vendor_ << ' ' << g.renderer_;
		return os;
	}

	std::string DumpSystemInformation()
	{
		std::stringstream info;
		try
		{
			cpuinfo c;
			info << c << '\n';
		}
		catch (...)
		{
			info << "Unable to get CPU information!\n";
		}
		try
		{
			
			gpuinfo g;
			info << g << '\n';
		}
		catch (...)
		{
			info << "Unable to get GPU information!\n";
		}
		return info.str();
	}

	std::string DumpCallstack()
	{
#ifdef _WIN32
		unsigned const frames = 32;
		PVOID stack[frames];
		int count = CaptureStackBackTrace(0, frames, stack, NULL);
		std::stringstream ss;
		for (int i = 0; i < count; ++i)
		{
			ss << stack[i] << '\n';
		}
		
		return ss.str();
#endif // _WIN32
	}

}
