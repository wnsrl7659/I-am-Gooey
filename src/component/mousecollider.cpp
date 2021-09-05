/******************************************************************************/
/*!
\file   mousecollider.cpp
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  collider for object and the mouse pointer
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "mousecollider.h"
#include "../messaging/messaging.h"
#include "../system/Window.h"
#include "../physics/Transform.h"
#include "../gameobject/gameobject.h"


MouseCollider::MouseCollider() : Component<MouseCollider>()
{
  Messaging::messHandler->Attach<MouseCollider, InputEvent, &MouseCollider::input_handler>(this, "InputEvent");
}

MouseCollider::~MouseCollider()
{
  Messaging::messHandler->Detach<MouseCollider>(this, "InputEvent");
}

void MouseCollider::Update(float)
{

}

void MouseCollider::Draw() const
{

}

void MouseCollider::SetTrigger(std::function<void(void)> func)
{
  func_ = func;
}

void MouseCollider::Trigger()
{
  func_();
}

void MouseCollider::input_handler(InputEvent * data)
{
  if (!(data->key == "mouse left" && data->action == "keyup"))
  {
    return;
  }

  if (!owner) return;
  Transform * t = nullptr;
  t = owner->GetComponent<Transform>();
  if (!t) return;
  double x, y;
  glfwGetCursorPos(GetWindow(), &x, &y);
  Vector2D pos;
  pos = t->Translation();
  Vector2D sc = t->Scale();

  // check x pos left
  if (x < pos[0] - sc[0]) return;
  // check x pos right
  if (x > pos[0] + sc[0]) return;
  // check y pos up
  if (y < pos[1] - sc[1]) return;
  // check y pos down
  if (y > pos[1] + sc[1]) return;

  // if we made it here, the mouse is inside the collider
  Trigger();
}

