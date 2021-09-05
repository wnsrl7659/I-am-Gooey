/******************************************************************************/
/*!
\file   exceptions.h
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Exceptions

\copyright    All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <exception>
#include <string>

namespace System
{
	enum class EType
	{
		OUT_OF_MEMORY = 0,
		FILESYSTEM,
	};
}

class SOLException : public std::exception
{
public:
	SOLException(System::EType type, std::string message) : std::exception(message.c_str()), t_(type) {};
	~SOLException() {};

	System::EType type() const { return t_; };

private:
	System::EType t_;
};