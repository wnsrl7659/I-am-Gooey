/******************************************************************************/
/*!
\file TileParser.cpp
\author Justice Mealer
\par    email: j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "../system/common_defines.h"
#include <rapidjson\document.h>
#include "TileParser.h"
#include "../system/FileReading.h" /* read file to char* */
#include <iostream>
#include "../settings/settings.h"
#include <string>


using namespace rapidjson;

Layer::Layer(const char* lName, float zHeight, bool col)
{
	name = lName;
	zPos = zHeight;
	isCollision = col;
	
}

Layer::~Layer()
{
	tiles.clear();
	xPos.clear();
	yPos.clear();
	name.clear();
}


void Layer::SetTileData(int tileData)
{
	tiles.push_back(tileData);
}

void Layer::SetLayerData(int x, int y)
{
	xPos.push_back(x);
	yPos.push_back(y);
}

std::vector<int>* Layer::GetTiles()
{
	return &tiles;
}

std::string Layer::GetName()
{
	return name;
}

float Layer::GetZPos()
{
	return zPos;
}

bool Layer::GetCollision()
{
	return isCollision;
}

std::vector<int>* Layer::GetXData()
{
	return &xPos;
}

std::vector<int>* Layer::GetYData()
{
	return &yPos;
}

LayerMap::LayerMap(const char* filename)
{
	char* jsonData = StringReadFromFile(filename);
	Document document;
	if (document.Parse(jsonData).HasParseError())
		return;

	if (jsonData)
		delete[] jsonData;

	assert(document.IsObject());
	//Get the width and height
	assert(document.HasMember("width"));
	assert(document.HasMember("height"));
	assert(document["width"].IsInt());
	assert(document["height"].IsInt());
	
	xSize = document["width"].GetInt();
	ySize = document["height"].GetInt();

	assert(document.HasMember("tilewidth"));
	assert(document.HasMember("tileheight"));
	assert(document["tilewidth"].IsInt());
	assert(document["tileheight"].IsInt());
	tileWidth = document["tilewidth"].GetInt() * 2;
	tileHeight = document["tileheight"].GetInt() * 2;
	//Get the number of layers
	assert(document.HasMember("nextlayerid"));
	assert(document["nextlayerid"].IsInt());
	numLayers = document["nextlayerid"].GetInt() - 1;
	document["layers"].GetArray();

	assert(document.HasMember("layers"));
	assert(document["layers"].IsArray());
	
	const Value& layerArray = document["layers"];
	for (unsigned int i = 0; i < layerArray.Size(); i++)
	{
		assert(layerArray[i].HasMember("type"));
		assert(layerArray[i]["type"].IsString());
		
		if (strncmp(layerArray[i]["type"].GetString(), "tilelayer", 12) == 0)
		{
			assert(layerArray[i].HasMember("properties"));
			const char* name = layerArray[i]["name"].GetString();
			Layer newLayer(name, (float)layerArray[i]["properties"][1]["value"].GetInt(), layerArray[i]["properties"][0]["value"].GetBool());
			const Value& data = layerArray[i]["data"];
			for (unsigned int j = 0; j < data.Size(); j++)
			{
				newLayer.SetTileData(data[j].GetInt());	
				int rowNumber = (int)(j / xSize);
				int colNumber = (int)(j % xSize);
				newLayer.SetLayerData((tileWidth * colNumber), (-tileHeight * rowNumber));
			}
			layers.push_back(newLayer);
		}
		else if (strncmp(layerArray[i]["type"].GetString(), "group", 64) == 0)
		{
			const Value& tempLayerArray = layerArray[i]["layers"];
			for (unsigned int h = 0; h < tempLayerArray.Size(); h++)
			{
				if (strncmp(tempLayerArray[h]["name"].GetString(), "levelchange", 64) == 0)
				{
					const Value& ttla = tempLayerArray[h]["layers"];
					for (unsigned int b = 0; b < ttla.Size(); b++)
					{
						assert(ttla[b].HasMember("properties"));
						const char* name = ttla[b]["name"].GetString();
						Layer newLayer(name, (float)ttla[b]["properties"][1]["value"].GetInt(), ttla[b]["properties"][0]["value"].GetBool());
						const Value& data = ttla[b]["data"];
						for (unsigned int k = 0; k < data.Size(); k++)
						{
							newLayer.SetTileData(data[k].GetInt());
							int rowNumber = (int)(k / xSize);
							int colNumber = (int)(k % xSize);
							newLayer.SetLayerData((tileWidth * colNumber), (-tileHeight * rowNumber));
						}
						layers.push_back(newLayer);
					}
				}
				else
				{
					assert(tempLayerArray[h].HasMember("properties"));
					const char* name = tempLayerArray[h]["name"].GetString();
					Layer newLayer(name, (float)tempLayerArray[h]["properties"][1]["value"].GetInt(), tempLayerArray[h]["properties"][0]["value"].GetBool());
					const Value& data = tempLayerArray[h]["data"];
					for (unsigned int j = 0; j < data.Size(); j++)
					{
						newLayer.SetTileData(data[j].GetInt());
						int rowNumber = (int)(j / xSize);
						int colNumber = (int)(j % xSize);
						newLayer.SetLayerData((tileWidth * colNumber), (-tileHeight * rowNumber));
					}
					
					layers.push_back(newLayer);
				}
			}
		}
		else
		{
			continue;
		}
	}
}

LayerMap::~LayerMap()
{
	layers.clear();
}

void LayerMap::PrintMapData()
{
	std::cout << "Num Layers: " << numLayers << std::endl;
	std::cout << "Width: " << xSize << std::endl;
	std::cout << "Height: " << ySize << std::endl;
	for (unsigned int i = 0; i < layers.size(); i++)
	{
		std::cout << layers.at(i).GetName() << " " << layers.at(i).GetZPos() << std::endl;
	}
}

std::vector<Layer>* LayerMap::GetLayers()
{
	return &layers;
}


