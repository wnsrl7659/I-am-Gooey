/******************************************************************************/
/*!
\file	heatmap.cpp
\author	Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Handles creating heatmaps of levels from player activity
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <fstream>

#include "heatmap.h"
#include "../physics/Transform.h"
#include "../spaces/spacemanager.h"
#include "../timer/timer.h"
#include "../graphics/Tilemap.h"

namespace System
{
	HeatmapGenerator * hgen = nullptr;

	void Init()
	{
		hgen = new HeatmapGenerator();
	}

	void Update()
	{
		hgen->Update();
	}

	void Shutdown()
	{
		delete hgen;
		hgen = nullptr;
	}
}

HeatmapGenerator::HeatmapGenerator()
{
}

HeatmapGenerator::~HeatmapGenerator()
{
}

void HeatmapGenerator::GetPosData()
{
	Space * sp = Spaces::spaceManager->GetSimilarSpace("playerspace");
	if (!sp)
	{
		return;
	}
	GameObject * obj = sp->GetObject("player");
	if (!obj)
	{
		return;
	}
	Transform * t = obj->GetComponent<Transform>();
	if (!t)
	{
		return;
	}
	points_.push_back(std::pair<float, float>(t->Translation()[0], t->Translation()[1]));
}

void HeatmapGenerator::Start()
{
	init_ = true;
	tm_.Start();
}

void HeatmapGenerator::Stop()
{
	tm_.Stop();
	points_.clear();
	init_ = false;
	rate_ = 0.0;
}

void HeatmapGenerator::Update()
{
	if (init_)
	{
		if (rate_ == 0.0)
		{
			GetPosData();
		}
		else if (tm_.GetMilliseconds() > rate_)
		{
			GetPosData();
			tm_.Stop();
			tm_.Start();
		}
	}
}

void HeatmapGenerator::SetCaptureRateMS(double ms)
{
	rate_ = ms;
}

void HeatmapGenerator::SetCaptureRateS(double seconds)
{
	rate_ = seconds * 1000.0;
}

void HeatmapGenerator::GenerateHeatmap()
{
	// passes points to a function that will add the data to data for drawing the level
	// and will return the pure data
	// this function will write the file
	char * data = nullptr; // data that we get from the function
	std::ofstream file;
	file.open("aa");
	file << data;
	file.close();
}
