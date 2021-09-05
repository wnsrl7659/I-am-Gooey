/******************************************************************************/
/*!
\file   TileDictionary.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "TileDictionary.h"

namespace TileDictionary
{
		tileName getName(int index)
		{
				switch (index)
				{
				case 10:
						return Brick;
				case 3:
						return Slab;
				case 58:
						return PillarBase;
				case 59:
						return PillarBroken;
				case 42:
						return PillarMid;
				case 26:
						return PillarTop;
				case 11:
						return WindowTop;
				case 27:
						return WindowBot;
				case 12:
						return DoorwayLeft;
				case 13:
						return DoorwayRight;
				case 18: case 19: case 20: case 34: case 35: case 36: case 50: case 51: case 52:
						return Carpet;
				case 71: case 72: case 71 + 16: case 72 + 16: case 98: case 99: case 100: case 101: case 102: case 103: case 98 + 16: case 99 + 16: case 100 + 16: case 101 + 16: case 102 + 16: case 103 + 16:
						return CarpetArrow;
				case 53:
						return Torch;
				case 21: case 22: case 21 + 16: case 22 + 16: case 4:
						return MagentaBowl;
				case 23: case 24: case 23 + 16: case 24 + 16: case 5:
						return CyanBowl;
				case 226: case 227: case 226 + 16: case 227 + 16: case 226 + 32: case 227 + 32:
						return StairsDown;
				case 228: case 229: case 228 + 16: case 229 + 16: case 228 + 32: case 229 + 32:
						return StairsDown;
				case 66: case 67: case 68: case 69: case 70: case 82: case 83: case 84: case 85: case 86: 
						return Trail;
				case 43:
						return Coin;
				case 30: case 31:
						return StatueTop;
				case 46: case 47:
						return StatueBot;
				case 58+16:
						return PitEdge;
				case 16:
						return Rock;
				  
				 case 273 + 16 + 2: case 273 + 16 + 3: 
				 case 273 + 32 + 2: case 273 + 32 + 3: 
						return GiantBowl;

				case 273 + 1: case 273 + 4:
				case 273 + 48 + 1: case 273 + 48 + 4:
						return GiantBowlCorner;

				case 273 + 2: case 273 + 3:
				case 273 + 48 + 2: case 273 + 48 + 3:
				case 273 + 16 + 1: case 273 + 16 + 4:
				case 273 + 32 + 1: case 273 + 32 + 4:
						return GiantBowlEdge;
				}

				return invalid;
		}
}