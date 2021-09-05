/******************************************************************************/
/*!
\file   Tilemap.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for tilemap loading from json

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Sprite.h"

#pragma once

class Tilemap
{
private:
		int layerCount;   //!< how many layers are in the tilemap
		int width, height;//!< width and height of the tilemap

		//Arrays with the size of the entire tilemap (w*h*l)
		int* x;    //!< x positions of each tile
		int* y;    //!< y positions of each tile
		int* data; //!< texture index of each tile

		//Arrays with size of the amount of layers
		bool* layerIsCollision; //!< layer collision flag
		bool* layerIsVisible;   //!< layer visibility flag
		std::string* names;     //!< layer name
public:
		Tilemap(const char *jsonData);
		~Tilemap();

		bool isCollisionLayer(int layerToCheck);
		bool isVisibleLayer(int layerToCheck);
		std::string getName(int layer);
		int getLayers();
		int getWidth();
		int getHeight();
		int getX(int index, int layer = 0);
		int getY(int index, int layer = 0);
		int getData(int index, int layer = 0);
		void printData();
};

typedef struct tMap {
	int status = 0;
	int count = 0;
	Sprite* sprite[5] = { 0 };
}tMap;

extern tMap trailmap[1000][1000];
extern int bowlmap[1000][1000];
extern int wallmap[1000][1000];
