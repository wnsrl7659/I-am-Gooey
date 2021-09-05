/******************************************************************************/
/*!
\file   controller.h
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Controller, Player Controller, Camera Controller components

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "component.h"
#include "../messaging/InputEvent.h"
#include "../messaging/ControllerEvent.h"
#include "../puzzles/newpuzzle.h"

class Controller : public Component<Controller>
{
public: 
	COMPONENT_NAME(Controller); //!< name of component
	Controller() : Component() {};
	virtual ~Controller();
	virtual void Update(float dt);
	virtual void Draw() const;
	virtual void Move();
};

class PlayerController : public Controller
{
public:
	COMPONENT_NAME(PlayerController); //!< name of component
	PlayerController();
	~PlayerController();
	virtual void Update(float dt);
	virtual void Draw() const;
	virtual void Move(InputEvent *);
private:
	int idle; //!< idle animation number for when the player isnt moving
	float frameTimer; //!< frame timer for switching animation
};

extern MovementEvent move; //!< move enum for what the player's move state is
extern MovementEvent nextmove;
extern MovementEvent pastmove;
extern MovementEvent firstmove;
extern MovementEvent secondmove;

class CameraController : public Controller
{
public:
	COMPONENT_NAME(CameraController); //!< name of component
	CameraController();
	virtual void Update(float dt);
	virtual void Draw() const;
	virtual void Move();
};

void playerResetPos();
void trailReset();
void plhandleInput(int key, int action, GLFWwindow* window);

extern bool hitbywall;
extern bool trailswitch;
extern bool trailresetswitch;
extern bool trailbook;
extern int trailresetcount;

extern glm::vec2 playerpos;

extern bool pausecheck;