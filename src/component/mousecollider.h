/******************************************************************************/
/*!
\file   mousecollider.h
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  collider for object and the mouse pointer
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef MOUSE_COLLIDER_H
#define MOUSE_COLLIDER_H

#include <functional>

#include "component.h"
#include "../messaging/InputEvent.h"

class MouseCollider : public Component<MouseCollider>
{
public:
  COMPONENT_NAME(MouseCollider);

  MouseCollider();
  virtual ~MouseCollider();

  void Update(float dt);
  void Draw() const;

  void SetTrigger(std::function<void(void)> func);

  void Trigger();

private:
  void input_handler(InputEvent * data);

  std::function<void(void)> func_;
};

#endif // !MOUSE_COLLIDER_H
