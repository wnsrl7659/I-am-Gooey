/******************************************************************************/
/*!
\file   map.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
  data structure for all information about a map. a work in progress

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef MAP_H
#define MAP_H

#include "../component/component.h"
#include "../maps/layer.h"

class Map : public Component<Map>
{
public:
  COMPONENT_NAME(Map);
  Map();
  ~Map();

  virtual void Update(float dt);
  virtual void Draw() const;
  Layer * AddLayer(const std::string& name);
  Layer * GetLayer(const std::string& name) const;
  void RemoveLayer(const std::string& name);

private:
  std::unordered_map<const std::string&, Layer *> layers_;
  int layerCount_;
};

#endif // !MAP_H

