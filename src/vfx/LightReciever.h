/******************************************************************************/
/*!
\file		LightReciever.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../component/component.h"
#include "TileDictionary.h"
#include <vector>

class LightReciever : public Component<LightReciever>
{
public:
		COMPONENT_NAME(LightReciever);
		LightReciever();
		void setUniforms();
		void Update(float dt) {}
		void Draw() const {}

		float getZ() { return Z; }
		int getIsWall() { return isWall; }

		void setZ(float f) { Z = f; }
		void setIsWall(int i) { isWall = i; }
		void SetColor1(float r, float g, float b);
		void SetColor2(float r, float g, float b);
		void SetColor3(float r, float g, float b);
		void SetColorsFromTileName(TileDictionary::tileName name, int floor);
		void CreateLightFromTileName(TileDictionary::tileName name, int floor);
		void CreateWallFromTileName(TileDictionary::tileName name);
		void setDoLava(int i) { doLava = i; }

private:
		
		int isWall;
		int doLava;
		float Z;//Z position of this object
		float RealColors[9]; //Tricolor values for color map
};