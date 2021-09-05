/******************************************************************************/
/*!
\file   button.cpp
\author Justice Mealer
\par    Email: j.mealer@digipen.edu
\par    Course: Game 200
\brief  Button, Acorn, Number, and Selector Components. All related, trust me.

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

constexpr int VOL_CHANGE = 5;

#define VOLUP(x) x = (x + VOL_CHANGE > 100) ? 100 : x + VOL_CHANGE;
#define VOLDOWN(x) x = (x - VOL_CHANGE < 0) ? 0 : x - VOL_CHANGE;

#include "button.h"
#include "../graphics/Sprite.h"
#include "../physics/Transform.h"
#include "../gameobject/gameobject.h"
#include "../messaging/messaging.h"
#include "../messaging/messagehandler.h"
#include "../messaging/InputEvent.h"
#include "../physics/Vector2D.h"
#include "../spaces/spaces.h"
#include "../spaces/spacemanager.h"
#include "../system/TrueType.h"
#include "../puzzles/newpuzzle.h"
#include "../graphics/Fade.h"
#include "../audio/audiomanager.h"
#include "../audio/audio.h"
#include "../system/TrueType.h"
#include "../system/localize.h"
#include "../settings/settings.h"
#include "../graphics/FontRender.h"

int quitconfirmflag = 0;
int menuconfirmflag = 0;
int yesorno = 0;

/**************************************************************************/
/*!
\brief Button Event Constructor
\param eventName
   name of event from an enum called bEvent, can be of types like play, quit, resume, etc
*/
/**************************************************************************/
ButtonEvent::ButtonEvent(bEvent eventName) : eventType(eventName)
{
}

/**************************************************************************/
/*!
\brief Button Event Destructor
*/
/**************************************************************************/
ButtonEvent::~ButtonEvent()
{
}

/**************************************************************************/
/*!
\brief Button Constructor
*/
/**************************************************************************/
UI::UI() : Component()
{
	posX = 0.0f;
	posY = 0.0f;
}

/**************************************************************************/
/*!
\brief Button Destructor
*/
/**************************************************************************/
UI::~UI()
{

}

/**************************************************************************/
/*!
\brief Sets the button's posX and posY variables, which are relative to the camera
\param x
	float for x pos
\param y
	float for y pos
*/
/**************************************************************************/
void UI::Set(float x, float y)
{
	posX = x;
	posY = y;
}

/**************************************************************************/
/*!
\brief Button Update function, draws the object relative to camera space
\param dt
	delta time in case you need it
*/
/**************************************************************************/
void UI::Update(float dt)
{
	Transform * transform = dynamic_cast<Transform *>(owner->GetComponent<Transform>());
	Sprite * sprite = dynamic_cast<Sprite *>(owner->GetComponent<Sprite>());
	if(transform != nullptr)
		transform->Translation(Vector2D(mainCam.center().x + posX, mainCam.center().y + posY));
	if(sprite != nullptr)
		sprite->SetPosition(mainCam.center().x + posX, mainCam.center().y + posY);
}

/**************************************************************************/
/*!
\brief Button Draw function
*/
/**************************************************************************/
void UI::Draw() const
{

}

/**************************************************************************/
/*!
\brief Acorn constructor. Used only for Acorn GameObj. Adds a msg handler
*/
/**************************************************************************/
Acorn::Acorn() : UI()
{
	posX = 0.0f;
	posY = 0.0f;
	//Messaging::messHandler->Attach<Acorn, InputEvent, &Acorn::GetHint>(this, "InputEvent");
}

/**************************************************************************/
/*!
\brief Acorn Destructor. Detaches message handler
*/
/**************************************************************************/
Acorn::~Acorn()
{
	//Messaging::messHandler->Detach<Acorn>(this, "InputEvent");
}

/**************************************************************************/
/*!
\brief Acorn Set function for setting posx and posy
\param x
	relative x pos
\param y
	relative y pos
*/
/**************************************************************************/
void Acorn::Set(float x, float y)
{
	posX = x;
	posY = y;
}

/**************************************************************************/
/*!
\brief Acorn Update function, draws relative to camera space and updates numHints with the data manager
\param dt
	delta time incase i need it
*/
/**************************************************************************/
void Acorn::Update(float dt)
{
	Transform * transform = dynamic_cast<Transform *>(owner->GetComponent<Transform>());
	Sprite * sprite = dynamic_cast<Sprite *>(owner->GetComponent<Sprite>());
	if(transform != nullptr)
		transform->Translation(Vector2D(mainCam.center().x + posX, mainCam.center().y + posY));
	if(sprite != nullptr)
		sprite->SetPosition(mainCam.center().x + posX, mainCam.center().y + posY);
	//numHints = Data::pManager->HintCount();
	/*if (numHints >= 100)
	{
		numHints = 99;
	}*/
}

/**************************************************************************/
/*!
\brief Acorn Draw function
*/
/**************************************************************************/
void Acorn::Draw() const
{

}

/**************************************************************************/
/*!
\brief Acorn Hint Cheat
\param data
	input event data to check if its the cheat button
*/
/**************************************************************************/
void Acorn::GetHint(InputEvent * data)
{
	if (data->action == "keydown" && data->key == "m")
	{
		//Data::pManager->AddHint();
	}
}

/**************************************************************************/
/*!
\brief Number Component Constructor, used for the Hint Counter object
*/
/**************************************************************************/
Number::Number() : UI()
{
	posX = 0.0f;
	posY = 0.0f;
	numHints = PzManage::pzm->hints;
}

/**************************************************************************/
/*!
\brief Number Destructor
*/
/**************************************************************************/
Number::~Number()
{

}

/**************************************************************************/
/*!
\brief Number Set function for setting posx and posy
\param x
relative x pos
\param y
relative y pos
*/
/**************************************************************************/
void Number::Set(float x, float y, Digit place)
{
	posX = x;
	posY = y;
	digit = place;
}

/**************************************************************************/
/*!
\brief Number Update function, draws relative to camera space and updates the hint counter with hint number
\param dt
delta time incase i need it
*/
/**************************************************************************/
void Number::Update(float dt)
{
	numHints = PzManage::pzm->hints;
	Transform * transform = dynamic_cast<Transform *>(owner->GetComponent<Transform>());
	Sprite * sprite = dynamic_cast<Sprite *>(owner->GetComponent<Sprite>());
	Animation* anim = dynamic_cast<Animation *>(owner->GetComponent<Animation>());
	FontRender* font = dynamic_cast<FontRender *>(owner->GetComponent<FontRender>());
	if(transform != nullptr)
		transform->Translation(Vector2D(mainCam.center().x + posX, mainCam.center().y + posY));
	if(sprite != nullptr)
		sprite->SetPosition(mainCam.center().x + posX, mainCam.center().y + posY);
	unsigned oneD;
	unsigned tenD;
	if (numHints >= 10)
	{
		oneD = numHints % 10;
		tenD = (numHints - oneD) / 10;
	}
	if (numHints < 10)
	{
		oneD = numHints;
		tenD = 0;
	}
	if (anim != nullptr)
	{
		if (digit == tens)
		{
			(*anim)[0][0] = tenD;
		}
		else if (digit == ones)
		{
			(*anim)[0][0] = oneD;
		}
	}
	int sumnum = tenD * 10 + oneD;
	
	if (font != nullptr)
	{
		std::wstring tempw = std::to_wstring(sumnum);
		font->SetText(tempw);
	}
}

/**************************************************************************/
/*!
\brief Number Draw function
*/
/**************************************************************************/
void Number::Draw() const
{

}

/**************************************************************************/
/*!
\brief Selector Component, keeps track of buttons, attaches message handler
*/
/**************************************************************************/
Selector::Selector()
{
	Messaging::messHandler->Attach<Selector, InputEvent, &Selector::Move>(this, "InputEvent");
	//Messaging::messHandler->Attach<Selector, ButtonEvent, &Selector::ButtonEventHandler>(this, "ButtonEvent");
	current = 0;
	menuButtons.clear();
}

/**************************************************************************/
/*!
\brief Selector destructor, detaches message handler
*/
/**************************************************************************/
Selector::~Selector()
{
	Messaging::messHandler->Detach<Selector>(this, "InputEvent");
	//Messaging::messHandler->Detach<Selector>(this, "ButtonEvent");
	menuButtons.clear();
}

/**************************************************************************/
/*!
\brief Adds a button to the selector's vector
\param button
	game object pointer to the button, doesnt need to technically be a button component
*/
/**************************************************************************/
void Selector::AddButton(GameObject* button)
{
	menuButtons.push_back(button);
}

/**************************************************************************/
/*!
\brief Selector Set function for setting posx and posy
\param x
relative x pos
\param y
relative y pos
*/
/**************************************************************************/
void Selector::Set(float x, float y)
{
	posX = x;
	posY = y;
}

/**************************************************************************/
/*!
\brief Selector Update function, draws relative to the button it is hovering next to
\param dt
delta time incase i need it
*/
/**************************************************************************/
void Selector::Update(float dt)
{
	if ((owner->space) && (owner->space->Active()))
	{
		if (current < 0)
		{
			current = menuButtons.size() - 1;
		}
		if ((size_t)current >= menuButtons.size())
		{
			current = 0;
		}

		Transform * transform = dynamic_cast<Transform *>(owner->GetComponent<Transform>());
		Sprite * sprite = dynamic_cast<Sprite *>(owner->GetComponent<Sprite>());

		Transform* buttonTransform = dynamic_cast<Transform *>(menuButtons.at(current)->GetComponent<Transform>());

		Vector2D buttonTranslate = buttonTransform->Translation();

		if(transform != nullptr && buttonTransform != nullptr)
			transform->Translation(Vector2D(buttonTranslate[0] + posX, buttonTranslate[1] + posY));
		if(sprite != nullptr)
			sprite->SetPosition(buttonTranslate[0] + posX, buttonTranslate[1] + posY);
	}
}

/**************************************************************************/
/*!
\brief Number Draw function
*/
/**************************************************************************/
void Selector::Draw() const
{

}

/**************************************************************************/
/*!
\brief Moves the selector to the next button based on input and handles if the button is selected. Sends out a button event.
\param input
input event to see how the button will move
*/
/**************************************************************************/
void Selector::Move(InputEvent* input)
{
		if (current < 0)
				current = 0;
		if (current > menuButtons.size() - 1)
				current = menuButtons.size() - 1;

	if ((owner->space->Active()) == true)
	{
		ButtonEvent what(noEvent);
		if (input->action == "keydown" && quitconfirmflag == 0 && menuconfirmflag == 0)
		{
			if (input->key == "w" || input->key == "uup")
			{
				Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
				Audio::EventData data;
				data.eventName = "Menu_Clicks";
				data.paramName = "UI";
				data.val = 0.0f;
				Audio::EditParam(&data);
				current--;
			}
			else if (input->key == "s" || input->key == "ddown")
			{
				Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
				Audio::EventData data;
				data.eventName = "Menu_Clicks";
				data.paramName = "UI";
				data.val = 0.0f;
				Audio::EditParam(&data);
				current++;
			}
			/////
			else if (input->key == "a" || input->key == "lleft")
			{
				if (menuButtons.at(current)->name == "masterV")
				{
					VOLDOWN(Settings::masterVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

				}
				else if (menuButtons.at(current)->name == "BGMV")
				{
					VOLDOWN(Settings::musicVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

				}
				else if (menuButtons.at(current)->name == "EffectV")
				{
					VOLDOWN(Settings::sfxVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

				}
				else if (menuButtons.at(current)->name == "Language")
				{
					if (fontcount <= 1)
					{
						fontcount = 7;
					}
					if (fontcount > 1)
					{
						Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

						fontcount--;
						switch (fontcount)
						{
						case 1:
							System::SetLocale("./localization", "en", "en_US.UTF-8");
							fonttype = 0;
							break;
						case 2:
							System::SetLocale("./localization", "ru", "ru_RU.UTF-8");
							// set font in use to russian font
							fonttype = font_RUS;
							break;
						case 3:
							System::SetLocale("./localization", "ja", "ja_JP.UTF-8");
							// set font in use
							fonttype = font_JAP;
							break;
						case 4:
							System::SetLocale("./localization", "ko", "ko_KR.UTF-8");
							fonttype = font_KOR;
							break;
						case 5:
							System::SetLocale("./localization", "de", "de_DE.UTF-8");
							fonttype = 0;
							break;
						case 6:
							System::SetLocale("./localization", "es", "es_ES.UTF-8");
							fonttype = 0;
							break;
						}
					}
				}
			}
			else if (input->key == "d" || input->key == "rright")
			{
				if (menuButtons.at(current)->name == "masterV")
				{
					// d (+ or raise) on master volume
					VOLUP(Settings::masterVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
				}
				else if (menuButtons.at(current)->name == "BGMV")
				{
					VOLUP(Settings::musicVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

				}
				else if (menuButtons.at(current)->name == "EffectV")
				{
					VOLUP(Settings::sfxVol);
					Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
				}
				else if (menuButtons.at(current)->name == "Language")
				{
					if (fontcount >= 6)
					{
						fontcount = 0;
					}
					if (fontcount < 6)
					{
						Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));

						fontcount++;
						switch (fontcount)
						{
						case 1:
							System::SetLocale("./localization", "en", "en_US.UTF-8");
							fonttype = 0;
							break;
						case 2:
							System::SetLocale("./localization", "ru", "ru_RU.UTF-8");
							fonttype = font_RUS;
							break;
						case 3:
							System::SetLocale("./localization", "ja", "ja_JP.UTF-8");
							fonttype = font_JAP;
							break;
						case 4:
							System::SetLocale("./localization", "ko", "ko_KR.UTF-8");
							fonttype = font_KOR;
							break;
						case 5:
							System::SetLocale("./localization", "de", "de_DE.UTF-8");
							fonttype = 0;
							break;
						case 6:
							System::SetLocale("./localization", "es", "es_ES.UTF-8");
							fonttype = 0;
							break;
						}
					}
				}
			}
			/////
			////
			else if (input->key == "esc")
			{
				if (menuButtons.at(current)->name == "End")
				{
					what.eventType = creditsEnd;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
			}
		}
		else if (input->action == "keyup" && quitconfirmflag == 0 && menuconfirmflag == 0)
		{
			if (input->key == "enter")
			{
				Messaging::messHandler->HandleMessage<std::string>("audio event", &std::string("play Menu_Clicks"));
				Audio::EventData data;
				data.eventName = "Menu_Clicks";
				data.paramName = "UI";
				data.val = 0.52f;
				Audio::EditParam(&data);

				if (menuButtons.at(current)->name == "Play Game")
				{
					what.eventType = play;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
					//FadeIn(1, what);
				}
				else if (menuButtons.at(current)->name == "Resume Game")
				{
					if (Spaces::spaceManager->currentScreen_ == Spaces::Screens::PauseMenu)
					{
						what.eventType = resume;
						Messaging::messHandler->HandleMessage("ButtonEvent", &what);
					}
				}
				else if (menuButtons.at(current)->name == "Restart Game")
				{
					//what.eventType = restart;
					//Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuButtons.at(current)->name == "Back Game")
				{
					Spaces::spaceManager->SetSpace(Spaces::Screens::PauseMenu);
					what.eventType = bEvent::poptionsback;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
					ffullscreentextswtich = false;
					input->key = "";
				}
				else if (menuButtons.at(current)->name == "BBack Game")
				{
					what.eventType = bEvent::optionsback;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
					fullscreentextswtich = false;
				}
				else if (menuButtons.at(current)->name == "Tutorial Back")
				{
					Spaces::spaceManager->SetSpace(Spaces::Screens::PauseMenu);
					what.eventType = bEvent::tutback;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
					input->key = "";
				}
				else if (menuButtons.at(current)->name == "Quit Game")
				{
					if (quitconfirmflag == 0)
					{
						quitconfirmflag = 1;
						input->key = "";
					}
				}
				else if (menuButtons.at(current)->name == "Options")
				{
					what.eventType = options;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuButtons.at(current)->name == "pOptions")
				{
					Spaces::spaceManager->SetSpace(Spaces::Screens::PausePause);
					what.eventType = poptions;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuButtons.at(current)->name == "Tutorial")
				{
					Spaces::spaceManager->SetSpace(Spaces::Screens::PausePause);
					what.eventType = tutorial;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuButtons.at(current)->name == "Credits")
				{
					what.eventType = credits;
					Messaging::messHandler->HandleMessage("ButtonEvent", &what);
				}
				else if (menuButtons.at(current)->name == "menu")
				{
					if (menuconfirmflag == 0)
					{
						menuconfirmflag = 1;
						input->key = "";
					}
				}
				else if (menuButtons.at(current)->name == "Screen Toggle")
				{
					ToggleFullScreen();
				}
				else if (menuButtons.at(current)->name == "MuteV")
				{
					if (mutetext == true)
					{
						mutetext = false;
					}
					else
					{
						mutetext = true;
					}
					Settings::masterMuted = !Settings::masterMuted;
				}
			}

		}
	}
	else
	{
		current = 0;
	}
	
}

/*void Selector::ButtonEventHandler(ButtonEvent * button)
{
	if ((owner->space->Active()) == true)
	{
		InputEvent action("none", "none");
		if (button->eventType == "play")
		{
			action.key = "p";
			action.action = "keydown";
			Messaging::messHandler->HandleMessage("InputEvent", &action);
		}
		else if (button->eventType == "resume")
		{
			action.key = "esc";
			action.action = "keydown";
			Messaging::messHandler->HandleMessage("InputEvent", &action);
		}
		else if (button->eventType == "restart")
		{
			action.key = "r";
			action.action = "keydown";
			Messaging::messHandler->HandleMessage("InputEvent", &action);
		}
	}
}*/


