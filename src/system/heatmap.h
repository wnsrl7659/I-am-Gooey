/******************************************************************************/
/*!
\file	heatmap.h
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Handles creating heatmaps of levels from player activity
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef HEATMAP_H
#define HEATMAP_H

#include <list>
#include "../timer/timer.h"

class HeatmapGenerator
{
public:
	/**************************************************************************/
	/*!
	\brief Constructor
	*/
	/**************************************************************************/
	HeatmapGenerator();

	/**************************************************************************/
	/*!
	\brief Destructor
	*/
	/**************************************************************************/
	~HeatmapGenerator();

	/**************************************************************************/
	/*!
	\brief Initializes the  generator and starts the timer
	*/
	/**************************************************************************/
	void Start();

	/**************************************************************************/
	/*!
	\brief Resets the generator and stops it running
	*/
	/**************************************************************************/
	void Stop();

	/**************************************************************************/
	/*!
	\brief Updates the heatmap data
	*/
	/**************************************************************************/
	void Update();


	/**************************************************************************/
	/*!
	\brief Sets the rate that the object captures the player's position data
	\param ms
		time in milliseconds
	*/
	/**************************************************************************/
	void SetCaptureRateMS(double ms);

	/**************************************************************************/
	/*!
	\brief Sets the rate that the object captures the player's position data
	\param seconds
		time in seconds
	*/
	/**************************************************************************/
	void SetCaptureRateS(double seconds);

	/**************************************************************************/
	/*!
	\brief Generates the heatmap file
	*/
	/**************************************************************************/
	void GenerateHeatmap();

private:
	void GetPosData();

	std::list<std::pair<float, float>> points_; // list of points captured every set amount of time
	double rate_ = 0.0; // rate of capture in ms, 0 means every frame
	Timer tm_; // timer
	bool init_ = false; // if the generator has been initialized. Won't run unless true
};

namespace System
{
	extern HeatmapGenerator * hgen;

	void Init();
	void Update();
	void Shutdown();
}

#endif // !HEATMAP_H
