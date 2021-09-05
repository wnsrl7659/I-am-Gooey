/******************************************************************************/
/*!
\file   p_script.h
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  A P(suedo) script. Not a real script like binding c# or python, but
  it works in a similar way

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "pscript.h"

p_script::p_script() : Component<p_script>()
{

}

p_script::~p_script()
{
  script_funcs.clear();
}

void p_script::Update(float)
{
}

void p_script::Draw() const
{
}

void p_script::pre_update(float dt)
{
  for (auto& f : script_funcs)
  {
    f(dt, this->owner);
  }
}

void p_script::push_back(p_script::p_script_func const f)
{
  script_funcs.push_back(f);
}

void p_script::push_front(p_script::p_script_func const f)
{
  script_funcs.push_front(f);
}

void p_script::pop_front()
{
  script_funcs.pop_front();
}

void p_script::pop_back()
{
  script_funcs.pop_back();
}

p_script::p_script_func const& p_script::front() const
{
  return script_funcs.front();
}

p_script::p_script_func const& p_script::back() const
{
  return script_funcs.back();
}
