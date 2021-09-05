/******************************************************************************/
/*!
\file   teleporter.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	used to teleport an object from one position to another

\copyright    All content (c) 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef TELEPORTER_H
#define TELEPORTER_H

#include "component.h"
#include "../physics/Transform.h"

class Teleporter : public Component<Teleporter>
{
public:
	COMPONENT_NAME(Teleporter);

	Teleporter();
	virtual ~Teleporter();

	virtual void Update(float dt);
	virtual void Draw() const {};

	void SetTriggerPosition(Vector2D v, bool lessThan = false, bool xMatters = true, bool yMatters = true);
	void SetResetPosition(Vector2D v);

private:
	Vector2D trigger_;
	Vector2D reset_;

	bool lessThan_;
	bool xMatters_;
	bool yMatters_;
};

#endif // !TELEPORTER_H
