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


#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <set>

namespace Utils
{
	std::string GetGameDataFolder();

	std::set<std::string> GetFolderContents(std::string path);

	std::vector<std::vector<std::string>> tokenize_file(std::string path);
}

#endif // !UTILS_H