/******************************************************************************/
/*!
\file TileParser.h
\author Justice Mealer
\par    email: j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <vector>
#include <string>


class Layer
{
public:
	Layer(const char*, float, bool);
	~Layer();
	std::vector<int>* GetTiles();
	std::vector<int>* GetXData();
	std::vector<int>* GetYData();
	void SetTileData(int);
	void SetLayerData(int, int);
	std::string GetName();
	float GetZPos();
	bool GetCollision();

private:
	//82 x 52 (wxh)
	std::vector<int> tiles;
	std::vector<int> xPos;
	std::vector<int> yPos;
	std::string name;
	float zPos = 0;
	bool isCollision;

};

class LayerMap
{
public:
	LayerMap(const char* filename);
	~LayerMap();
	void PrintMapData();
	std::vector<Layer>* GetLayers();
	int tileWidth;
	int tileHeight;

private:
	std::vector<Layer> layers;
	int numLayers;
	int xSize;
	int ySize;

};

