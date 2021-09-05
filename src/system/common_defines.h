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

#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#define Has(x) find(x) != std::string::npos
#define TileName(i) tile.getName(i)
#define _FILEIO
#define SpacesContains(x) spaces.find(x) != spaces.end()
#undef GetObject
#undef main

constexpr float DELTA_TIME = 1.0f / 60.0f;

//#define TESTING


#endif // !COMMON_DEFINES_H
