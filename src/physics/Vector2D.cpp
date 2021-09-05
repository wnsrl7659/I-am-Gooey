/*! ****************************************************************************
\file                Vector2D.cpp
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\brief
  Implementation of the Vector2D class defined in Vector2D.h

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "Vector2D.h"

// Public Functions                       //////////////////////////////////////

Vector2D::Vector2D(float a, float b) : m_A(a), m_B(b)
{
}

float & Vector2D::operator[](uint32_t i)
{
  return i==0 ? m_A : m_B;
}

float Vector2D::operator[](uint32_t i) const
{
  return i==0 ? m_A : m_B;
}

Vector2D Vector2D::operator-() const
{
  return Vector2D(-m_A, -m_B);
}

Vector2D Vector2D::operator+(Vector2D const & rhs) const
{
  return Vector2D(m_A + rhs.m_A, m_B + rhs.m_B);
}

Vector2D Vector2D::operator-(Vector2D const & rhs) const
{
  return Vector2D(m_A - rhs.m_A, m_B - rhs.m_B);
}

Vector2D Vector2D::operator*(float s) const
{
  return Vector2D(m_A*s, m_B*s);
}

float Vector2D::operator*(Vector2D const & rhs) const
{
  return m_A*rhs.m_A + m_B*rhs.m_B;
}

Vector2D & Vector2D::operator+=(Vector2D const & rhs)
{
  m_A += rhs.m_A;
  m_B += rhs.m_B;

  return *this;
}

Vector2D & Vector2D::operator-=(Vector2D const & rhs)
{
  m_A -= rhs.m_A;
  m_B -= rhs.m_B;

  return *this;
}

Vector2D & Vector2D::operator*=(float s)
{
  m_A *= s;
  m_B *= s;

  return *this;
}

void Vector2D::Zero()
{
  m_A = m_B = 0;
}

void Vector2D::Set(float a, float b)
{
  m_A = a;
  m_B = b;
}

void Vector2D::Normalize()
{
  float length = Length();

  m_A /= length;
  m_B /= length;
}

float Vector2D::Length() const
{
  return std::sqrt(SquareLength());
}

float Vector2D::SquareLength() const
{
  return m_A*m_A + m_B*m_B;
}

float Vector2D::Distance(Vector2D const & rhs) const
{
  return std::sqrt(SquareDistance(rhs));
}

float Vector2D::SquareDistance(Vector2D const & rhs) const
{
  return (rhs.m_A-m_A)*(rhs.m_A-m_A) + (rhs.m_B-m_B)*(rhs.m_B-m_B);
}

void Vector2D::Angle(float rad)
{
  m_A = std::cos(rad);
  m_B = std::sin(rad);
}
