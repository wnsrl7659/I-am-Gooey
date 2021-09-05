/*! ****************************************************************************
\file                Physics.cpp
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "../gameobject/gameobject.h"

#include "Transform.h"
#include "Physics.h"
#include "../component/collfix.h"

float Physics::s_PlayerVelocity = 4;

// Public Functions                       //////////////////////////////////////

Vector2D Physics::Velocity() const
{
	return m_Velocity;
}

void Physics::Velocity(Vector2D v)
{
	m_Velocity = v;
}

void Physics::Init()
{
}

void Physics::Update(float dt)
{
	Transform * transform = owner->GetComponent<Transform>();

	if (!transform)
	{
		return;
	}

	interval_ -= dt;
	if ((int)abs(transform->Translation()[0] - 32 + m_Velocity[0]) % 64 == 0 &&
		(int)abs(transform->Translation()[1] + 48 + m_Velocity[1]) % 64 == 0 &&
		owner->name == "player" && ismoved == false)
	{
		if (m_Velocity[0] == 0 && m_Velocity[1] == 0)
		{

		}
		else
		{
			ismoved = true;
			tempvel[0] = transform->Translation()[0] + m_Velocity[0];
			tempvel[1] = transform->Translation()[1] + m_Velocity[1];
		}
	}
	if (ismoved == true)
	{
		m_Velocity[0] = 0;
		m_Velocity[1] = 0;
	}

	if (interval_ <= 0.0f)
	{
		interval_ = DELTA_TIME / 2.0f;

		if (ismoved == true)
		{
			transform->Translation(tempvel);
			m_Velocity *= (1.0 - friction * dt);
			fix::UpdateCollisions();
			ismoved = false;
		}
		else
		{
			transform->Translation((m_Velocity)+transform->Translation());
			m_Velocity *= (1.0 - friction * dt);
			fix::UpdateCollisions();
		}
	}
}

