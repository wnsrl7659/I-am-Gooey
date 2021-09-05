/******************************************************************************/
/*!
\file   FileReading.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for getting a char* from a file

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "FileReading.h"
#include <iostream>
#include <string>
#include "../settings/settings.h"
#include "../trace/trace.h"

/*****************************************************************/
/*!
		\brief
				Given a filename, returns the contents as a string
				(in char* form).
		\return
				"error" on error. otherwise, the contents in the file are returned
*/
/*****************************************************************/
char* StringReadFromFile(const char * filename)
{
		FILE *fp;
		long int FileSize;
		char *string;

		fopen_s(&fp, filename, "rb");
		if (!fp)
		{
			Trace::Message(std::string("Couldn't open file: " + std::string(filename) + "\nCheck directory and filename"));
				return (char *)"error";
		}

		//Go to the end of the file and get the filesize
		fseek(fp, 0, SEEK_END);
		FileSize = ftell(fp);

		//Return to the beginning
		fseek(fp, 0, SEEK_SET);

		//allocate the string
		string = new char[FileSize + 1];

		//copy the file into the buffer
		if (fread(string, FileSize, 1, fp) != 1)
		{
				fclose(fp);
				delete[] string;
				Trace::Message(std::string("Couldn't copy the contents of the file" + std::string(filename)));
				return (char *)"error";
		}
		Trace::Message(std::string("Successfully read string ") + std::string(filename));

		fclose(fp);
		string[FileSize] = 0;
		return string;
}