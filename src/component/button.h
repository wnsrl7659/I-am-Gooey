/******************************************************************************/
/*!
\file   button.h
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Button, Acorn, Number, and Selector Components header. All related, trust me.

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "component.h"
#include "../messaging/InputEvent.h"
#include <vector>
#include <iterator>

enum Digit
{
	tens,
	ones,
	unset,
};

class UI : public Component<UI>
{
public:
	COMPONENT_NAME(UI); //!< name of component
	UI();
	virtual ~UI();
	void Set(float x = 0, float y = 0);
	virtual void Update(float dt);
	virtual void Draw() const;
private:
	float posX; //!< relative x position to camera
	float posY; //!< relative y position to camera
};

class Acorn : public UI
{
public:
	COMPONENT_NAME(Acorn); //!< name of component
	Acorn();
	virtual ~Acorn();
	void Set(float x = 0, float y = 0);
	virtual void Update(float dt);
	virtual void Draw() const;
	void GetHint(InputEvent * data);
private:
	float posX; //!< relative x position to camera
	float posY; //!< relative y position to camera
};

class Number : public UI
{
public:
	COMPONENT_NAME(Number); //!< name of component
	Number();
	virtual ~Number();
	void Set(float x = 0, float y = 0, Digit place = unset);
	virtual void Update(float dt);
	virtual void Draw() const;
private:
	float posX; //!< relative x position to camera
	float posY; //!< relative y position to camera
	Digit digit; //!< what digit the instance represents 
	int numHints; //!< number of hints
};

enum bEvent
{
	noEvent,
	play,
	restart,
	resume,
	credits,
	tutorial,
	options,
	optionsback,
	creditsEnd,
	menu,
	quit,
	win,
	poptions,
	poptionsback,
	tutback,
};

class ButtonEvent
{
public:
	bEvent eventType;
	ButtonEvent(bEvent bvent);
	~ButtonEvent();

};

class Selector : public UI
{
public:
	COMPONENT_NAME(Selector); //!< name of component
	Selector();
	virtual ~Selector();
	void AddButton(GameObject* button);
	virtual void Update(float dt);
	virtual void Draw() const;
	void Move(InputEvent*);
	void Set(float x = 0, float y = 0);
	//void ButtonEventHandler(ButtonEvent * button);
private:
	float posX; //!< relative x position to camera
	float posY; //!< relative y position to camera
	int current; //!< current button hovering over
	std::vector<GameObject*> menuButtons; //!< vector of added buttons
};

extern int quitconfirmflag;
extern int menuconfirmflag;
extern int yesorno;