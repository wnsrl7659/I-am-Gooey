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

#include <filesystem>
#include <boost/tokenizer.hpp>
#include <fstream>

#include "utils.h"
#include "../trace/trace.h"

namespace Utils
{
	std::string GetGameDataFolder()
	{
		std::stringstream ss;
		ss << Trace::GetAppDataLocalLow();
		ss << "\\DigiPen\\IAmGooey";
		return ss.str();
	}

	std::set<std::string> GetFolderContents(std::string path)
	{
		std::set<std::string> contents;
		for (auto& p : std::filesystem::directory_iterator(path))
		{
			std::string s = p.path().string();
			s = s.substr(s.find_last_of("\\") + 1, std::string::npos);
			contents.insert(s);
		}
		return contents;
	}

	std::vector<std::vector<std::string>> tokenize_file(std::string path)
	{
		std::vector<std::vector<std::string>> tokens;

		std::ifstream file;
		file.open(path);
		if (file.is_open())
		{
			while (file)
			{
				std::string line;
				std::getline(file, line);
				// tokenize the line
				std::vector<std::string> l_tokens;
				boost::char_separator<char> sep(" ");
				boost::tokenizer<boost::char_separator<char>> tok(line, sep);
				for (auto token : tok)
				{
					l_tokens.push_back(token);
				}
				tokens.push_back(l_tokens);
			}
		}
		return tokens;
	}
}