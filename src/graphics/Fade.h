/******************************************************************************/
/*!
\file   Fade.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef FADE_H
#define FADE_H

#include "Sprite.h"
#include "Shader.h"
#include "Camera.h"
#include "../gameobject/gameobject.h"
#include "../component/button.h"

static ButtonEvent empty(noEvent);

void FadeInit();

void FadeUpdate(float dt);

void FadeShutdown();

void FadeIn(int ID, ButtonEvent what = empty, void* owner = NULL, int level = 0);
void FadeOut(void * ignore = nullptr);

bool IsFadeWorking();
bool IsFadeIned();

extern GameObject* fadeobj;

#endif

