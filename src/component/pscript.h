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

#ifndef P_SCRIPT_H
#define P_SCRIPT_H


#include <functional>
#include <deque>

#include "component.h"

class p_script : public Component<p_script>
{
private:
  typedef std::function<void(float, GameObject *)> p_script_func;

public:
  COMPONENT_NAME(p_script);

  p_script();
  virtual ~p_script();

  virtual void Update(float dt);
  virtual void Draw() const;

  void pre_update(float dt);

  void push_back(p_script_func const f);
  void push_front(p_script_func const f);

  void pop_front();
  void pop_back();

  p_script_func const& front() const;
  p_script_func const& back() const;

private:
  std::deque<p_script_func> script_funcs;

};

#endif // !P_SCRIPT_H
