/******************************************************************************/
/*!
\file   Tilemap.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for tilemap loading from json

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <rapidjson\document.h>
#include "Tilemap.h"
#include "../system/FileReading.h" /* read file to char* */
#include <iostream>
#include "../settings/settings.h"
#include <string>

using namespace rapidjson;

tMap trailmap[1000][1000] = {0};
int bowlmap[1000][1000] = { 0 };
int wallmap[1000][1000] = { 0 };

/*****************************************************************/
/*!
		\brief
				prints the data array for each layer in a tilemap. For
				debugging
*/
/*****************************************************************/
void Tilemap::printData()
{
		if (Settings::debug)
		{
				std::cout << std::endl << "TILEMAP DATA" << std::endl;
				std::cout << "Layers: " << layerCount << std::endl;
				std::cout << "Rows:" << height << " Cols:" << width << std::endl;

				for (int layer = 0; layer < layerCount; layer++)
				{
						std::cout << std::endl << "LAYER #" << layer << std::endl;

						std::cout << "  DATA:" << std::endl;
						for (int i = 0; i < height; i++)
						{
								std::cout << "  ";
								for (int j = 0; j < width; j++)
										std::cout << data[(layer * width * height) + (i * width) + j] << " ";
								std::cout << std::endl;
						}
				}
				std::cout << std::endl;
		}
}

/*****************************************************************/
/*!
		\brief
				Constructor. given a filename (for a json file), parses
				the contents and sets the fields of the tilemap.
		\param filename
				definitley not the filename...
*/
/*****************************************************************/
Tilemap::Tilemap(const char *filename)
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

		width = document["width"].GetInt();
		height = document["height"].GetInt();
		//Get the number of layers
		assert(document.HasMember("nextlayerid"));
		assert(document["nextlayerid"].IsInt());
		layerCount = document["nextlayerid"].GetInt() - 1;

		//Allocate space for the data
		data = new int[width*height*layerCount];
		x = new int[width*height*layerCount];
		y = new int[width*height*layerCount];

		layerIsCollision = new bool[layerCount];
		layerIsVisible = new bool[layerCount];
		names = new std::string[layerCount];

		//Get a reference to the layer array, which contains all the arrays of tiles
		assert(document.HasMember("layers"));
		assert(document["layers"].IsArray());
		const Value& layer = document["layers"];

		//Get the width and height of the tiles
		assert(document.HasMember("tilewidth"));
		assert(document["tilewidth"].IsInt());
		int tileWidth = document["tilewidth"].GetInt();

		assert(document.HasMember("tileheight"));
		assert(document["tileheight"].IsInt());
		int tileHeight = document["tileheight"].GetInt();

		//std::cout << "TESTTESTTEST: " << layer[0]["id"].GetInt() << std::endl;

		//Loop through the array of layers (just 1 for now)
		int i;
		for (i = 0; i < layerCount; i++)
		{
				//Get the data from the current layer
				assert(layer[i].HasMember("data"));
				assert(layer[i]["data"].IsArray());
				const Value& jdata = layer[i]["data"];

				//Get the visibilty from the current layer
				assert(layer[i].HasMember("visible"));
				assert(layer[i]["visible"].IsBool());
				layerIsVisible[i] = layer[i]["visible"].GetBool();
				

				//Get the name from the current layer
				assert(layer[i].HasMember("name"));
				assert(layer[i]["name"].IsString());

				//TempName for modifying later
				std::string TempName = layer[i]["name"].GetString();
				names[i] = TempName;

				//Check if the name includes "collision"
				for (unsigned i = 0; i < TempName.size(); i++)
				{
						if(TempName[i] >= 'a' && TempName[i] <= 'z')
								TempName[i] += 'A' - 'a';
				}

				if (TempName.find("COLLISION", 0) != std::string::npos)
						layerIsCollision[i] = true;
				else
						layerIsCollision[i] = false;


				//Loop through the data (length is width*height)
				int j;
				for (j = 0; j < width * height; j++)
				{
						int subscript = j + (width * height * i);

						data[subscript] = jdata[j].GetInt();

						int rowNumber = (int)(j / width);
						int colNumber = (int)(j % width);

						x[subscript] = tileWidth * colNumber;
						y[subscript] = -tileHeight * rowNumber;
				}
		}
}

/*****************************************************************/
/*!
		\brief
				given a layer index, returns if the layer is visible
		\return
				if the layer is visible
*/
/*****************************************************************/
bool Tilemap::isVisibleLayer(int layerToCheck)
{
		if (layerToCheck > layerCount)
				return false;
		return layerIsVisible[layerToCheck];
}

/*****************************************************************/
/*!
		\brief
				gets the name of the specified layer
		\param layer
				The layer you want to get the name of, hopefully.
		\return
				the name
*/
/*****************************************************************/
std::string Tilemap::getName(int layer)
{
		if (layer > layerCount)
				return NULL;
		return names[layer];
}

/*****************************************************************/
/*!
		\brief
				given a layer index, checks if its a collision layer
		\param layerToCheck
				the layer to check. variable names op
		\returns
				if its a collision layer
*/
/*****************************************************************/
bool Tilemap::isCollisionLayer(int layerToCheck)
{
		if (layerToCheck > layerCount)
				return false;
		return layerIsCollision[layerToCheck];
}

/*****************************************************************/
/*!
		\brief
				Detructor for a tilemap, frees the plentiful arrays that
				come with one. so long sweet tilemap.
*/
/*****************************************************************/
Tilemap::~Tilemap()
{
		delete[] data;
		delete[] x;
		delete[] y;

		delete[] layerIsCollision;
		delete[] layerIsVisible;
		delete[] names;
}

/*****************************************************************/
/*!
		\brief
				helper function which checks if the 1D index fits inside
				the tilemap
		\param index
				the 1D index
		\param layerCount
				the amount of layers
		\param w
				the width of all layers
		\param h
				the height of all layers
		\return
				true if the index is valid, false otherwise.
*/
/*****************************************************************/
static bool CheckTileIndex(int index, int layerCount, int w, int h)
{
		//Check index
		if (index > w*h*layerCount)
				return false;
		return true;
}

/*****************************************************************/
/*!
		\brief
				gets the width of the tilemap
		\return
				the width
*/
/*****************************************************************/
int Tilemap::getWidth()
{
	return width;
}

/*****************************************************************/
/*!
		\brief
				gets the height of the tilemap
		\return
				exactly what you'd expect
*/
/*****************************************************************/
int Tilemap::getHeight()
{
	return height;
}

/*****************************************************************/
/*!
		\brief
				gets the amount of layers in the ogre
		\return
				the layer count
*/
/*****************************************************************/
int Tilemap::getLayers()
{
		return layerCount;
}

/*****************************************************************/
/*!
		\brief
				Given an layer, and an index into that layer, returns 
				the x value
		\param index
				index into the layer
		\param layer
				the layer in the tilemap
		\return
				the x value (world coordinates)
*/
/*****************************************************************/
int Tilemap::getX(int index, int layer)
{
		int layerIndexOffset = layer * width * height;
		if (CheckTileIndex(index + layerIndexOffset, layerCount, width, height))
				return x[index + layerIndexOffset];
		return NULL;
}

/*****************************************************************/
/*!
		\brief
				Given an layer, and an index into that layer, returns
				the y value
		\param index
				index into the layer
		\param layer
				the layer in the tilemap
		\return
				the y value (world coordinates)
*/
/*****************************************************************/
int Tilemap::getY(int index, int layer)
{
		int layerIndexOffset = layer * width * height;
		if (CheckTileIndex(index + layerIndexOffset, layerCount, width, height))
				return y[index + layerIndexOffset];
		return NULL;
}

/*****************************************************************/
/*!
		\brief
				Given an layer, and an index into that layer, returns
				the texture data
		\param index
				index into the layer
		\param layer
				the layer in the tilemap
		\return
				the texture data (index in sprite sheet)
*/
/*****************************************************************/
int Tilemap::getData(int index, int layer)
{
		int layerIndexOffset = layer * width * height;
		if (CheckTileIndex(index + layerIndexOffset, layerCount, width, height))
				return data[index + layerIndexOffset];
		return NULL;
}
