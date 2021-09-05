/******************************************************************************/
/*!
\file	autoplay.h
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Plays the game all by itself, for testing purposes
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef AUTOPLAY_H
#define AUTOPLAY_H

class AutoTester
{
public:
	AutoTester();
	~AutoTester();

	void Start();
	void Stop();

	void Update(float dt);

private:
	bool running_ = false;
};

namespace Utils
{
	extern AutoTester * atst;

	void InitTester();
	void ShutdownTester();
}

#endif // !AUTOPLAY_H
