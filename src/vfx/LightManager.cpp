/******************************************************************************/
/*!
\file		LightManager.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "LightManager.h"
#include "../graphics/Camera.h"


LightManager* lightManager;

LightManager::LightManager()
{
		doShadows = true;
}

void LightManager::DeleteLight(LightSource* toRemove)
{
		int size = lights.size();
		for (int i = 0; i < size; i++)
		{
				if (toRemove == lights[i])
				{
						//Put the light at end of list in the found spot, so pop back doesnt get rid of the wrong thing
						lights[i] = lights[size - 1];
						delete toRemove;
						lights.pop_back();
						return;
				}
		}
}

void LightManager::DeleteLight(std::string tag)
{
		for (LightSource* it: lights)
		{
				if (it->label == tag)
				{
						DeleteLight(it);
						return;
				}
		}
}

LightSource* LightManager::AddLight(std::string label, bool isHidden, float r, float g, float b, float radius)
{
		LightSource* newLight = new(LightSource);
		newLight->Set(r, g, b, radius);
		newLight->label = label;

		if(isHidden)
				hiddenlights.push_back(newLight);
		else
				lights.push_back(newLight);
		return newLight;
}

void LightManager::AddWall(glm::vec2 point1, glm::vec2 point2)
{
		walls.push_back(glm::vec4(point1, point2));
		glm::vec2 norm = glm::vec2(point2.y - point1.y, point1.x - point2.x);
		wallNormals.push_back(glm::normalize(norm));
}

void LightManager::DeleteAllLights()
{
		while (!lights.empty())
		{
				LightSource* light = lights.back();
				delete light;
				lights.pop_back();
		}
		while (!hiddenlights.empty())
		{
				LightSource* light = hiddenlights.back();
				delete light;
				hiddenlights.pop_back();
		}
}
void LightManager::DeleteAllWalls()
{
		while (!walls.empty())
		{
				walls.pop_back();
		}
		while (!wallNormals.empty())
		{
				wallNormals.pop_back();
		}
}
void LightManager::UpdateLights(float dt)
{
		for (auto it : lights)
		{
				if (it->label == "camera")
				{
						glm::vec2 campos = mainCam.center();
						it->X(campos.x);
						it->Y(campos.y);
				}

				it->Update(dt);
		}
}
LightManager::~LightManager()
{
		DeleteAllLights();
}

std::vector<LightSource *>& LightManager::getLights()
{
		return lights;
}
std::vector<LightSource *>& LightManager::getHiddenLights()
{
		return hiddenlights;
}
std::vector<glm::vec4>& LightManager::getWalls()
{
		return walls;
}
std::vector<glm::vec2>& LightManager::getWallNormals()
{
		return wallNormals;
}
LightSource* LightManager::GetLight(std::string label)
{
		//loop through visible lights
		for (unsigned int i = 0; i < lights.size(); ++i)
		{
				if (label == lights[i]->label || label == "")
				{
						return lights[i];
				}
		}
		return nullptr;
}
void LightManager::MakeLightHidden(std::string label)
{
		//loop through visible lights
		for (unsigned int i = 0; i < lights.size(); ++i)
		{
				if (label == lights[i]->label || label == "")
				{
						hiddenlights.push_back(lights[i]);

						//swap the end with the current, and pop end
						lights[i] = lights.back();
						lights.pop_back();

						//since a swap happened, we gotta check the one we
						//just swapped with, so decrement i
						--i;
				}
		}
}
void LightManager::MakeLightVisible(std::string label)
{
		//loop through hidden lights
		for (unsigned int i = 0; i < hiddenlights.size(); ++i)
		{
				if (label == hiddenlights[i]->label || label == "")
				{
						lights.push_back(hiddenlights[i]);

						//swap the end with the current, and pop end
						hiddenlights[i] = hiddenlights.back();
						hiddenlights.pop_back();

						//since a swap happened, we gotta check the one we
						//just swapped with, so decrement i
						--i;
				}
		}
}

bool LightManager::getDoShadows()
{
		return doShadows;
}
void LightManager::setDoShadows(bool is)
{
		doShadows = is;
}

//FUNCTIONS NOT APART OF CLASS
void LightManagerUpdate(float dt)
{
		lightManager->UpdateLights(dt);
}
void LightManagerShutdown()
{
		delete(lightManager);
}
void LightManagerInit()
{
		lightManager = new LightManager();

		//LightSource* lp = lightManager->AddLight();
		//lp->X(1755);
		//lp->Y(-1920);
}