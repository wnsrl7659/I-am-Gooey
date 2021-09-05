/******************************************************************************/
/*!
\file		LightManager.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <vector>
#include "LightSource.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class LightManager
{
public:
		LightManager();
		~LightManager();
		void UpdateLights(float dt);
		void DeleteLight(LightSource* toRemove);
		void DeleteLight(std::string tag);

		LightSource* AddLight(std::string label = "", bool isHidden = false, float r = 1.f, float g = 1.f, float b = 1.f, float radius = 64);
		//void AddWall(glm::vec4 counterClockwiseCoordinates);
		void AddWall(glm::vec2 point1, glm::vec2 point2);

		void DeleteAllLights();
		void DeleteAllWalls();
		std::vector<LightSource *>& getLights();
		std::vector<glm::vec4>& getWalls();
		std::vector<glm::vec2>& getWallNormals();
		LightSource* GetLight(std::string label);
		std::vector<LightSource *>& getHiddenLights();

		bool getDoShadows();
		void setDoShadows(bool is);

		void MakeLightVisible(std::string label = "");
		void MakeLightHidden(std::string label = "");
private:
		bool doShadows;//Aka use the walls
		std::vector<LightSource *> lights;
		std::vector<glm::vec4> walls;
		std::vector<glm::vec2> wallNormals;
		std::vector<LightSource *> hiddenlights;//Lights that wont be looked for by tiles
};

extern LightManager *lightManager;

void LightManagerInit();
void LightManagerUpdate(float dt);
void LightManagerShutdown();