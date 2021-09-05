/******************************************************************************/
/*!
\file   TileDictionary.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once



namespace TileDictionary
{
		enum tileName
		{
				Brick,
				Slab, //floor tile
				PillarBase,
				PillarMid,
				PillarTop,
				PillarBroken,
				WindowTop,
				WindowBot,
				DoorwayLeft,
				DoorwayRight,
				Carpet,
				CarpetArrow,
				Torch, //room entrance torch
				MagentaBowl,
				CyanBowl,
				StairsUp,
				StairsDown,
				Trail,
				Coin,
				Rock,
				StatueTop,
				StatueBot,
				PitEdge,
				GiantBowl,
				GiantBowlCorner,
				GiantBowlEdge,

				invalid,
		};

		tileName getName(int SpriteSheetIndex);
}