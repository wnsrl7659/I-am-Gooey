/*! ****************************************************************************
\file                Transforms.cpp
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Transform.h"

// Public Objects                         //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

Transform::Transform() : m_Translation(0,0), m_Scale(1,1), m_Angle(0),
                         m_Matrix(), m_MatrixDirty(true)
{
}

Transform::operator Matrix2D()
{
    // If the transformation matrix needs to be recalculated, calculate it
  if(m_MatrixDirty)
  {
    Matrix2D s, r, t;

    s.Scale(m_Scale[0], m_Scale[1]);
    r.Rotate(m_Angle);
    t.Translate(m_Translation);

    m_Matrix = t * r * s;

    m_MatrixDirty = false;
  }

  return m_Matrix;
}

Vector2D const & Transform::Translation() const
{
  return m_Translation;
}

float Transform::Rotation() const
{
  return m_Angle;
}

Vector2D const & Transform::Scale() const
{
  return m_Scale;
}

void Transform::Translation(Vector2D const & t)
{
  m_Translation = t;
  m_MatrixDirty = true;
}

void Transform::Rotation(float angle)
{
  m_Angle = angle;
  m_MatrixDirty = true;
}

void Transform::Scale(Vector2D const & s)
{
  m_Scale = s;
  m_MatrixDirty = true;
}
