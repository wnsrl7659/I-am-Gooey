/******************************************************************************/
/*!
\file   layer.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  part of the work in progress data structure for map data. a layer is contained
  in a map, having data for one specific layer of the map

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef LAYER_H
#define LAYER_H

#include <unordered_map>
#include "tile.h"

class Layer
{
public:
  Layer();
  ~Layer();
  void AddTile(int tile);

  

private:
	std::vector<std::vector<int>> tiles;
};

#endif // !LAYER_H
