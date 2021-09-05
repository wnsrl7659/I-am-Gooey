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

#ifndef CPUID_H
#define CPUID_H

#include <string>

namespace System
{
	class cpuinfo
	{
	public:
		cpuinfo();
		~cpuinfo();

		std::string Vendor() const { return vendorid_; };
		std::string Model() const { return model_; };


		friend std::ostream& operator<<(std::ostream& os, cpuinfo const& c);

	private:
		std::string vendorid_; // cpu vendor
		std::string model_; // model of cpu

	};

	class gpuinfo
	{
	public:
		gpuinfo();
		~gpuinfo();

		std::string Vendor() const { return vendor_; };
		std::string Renderer() const { return renderer_; };

		friend std::ostream& operator<<(std::ostream& os, gpuinfo const& g);


	private:
		std::string vendor_;
		std::string renderer_;
	};

	std::string DumpSystemInformation();

	std::string DumpCallstack();

	extern gpuinfo * gInfo;
}

#endif // !CPUID_H
