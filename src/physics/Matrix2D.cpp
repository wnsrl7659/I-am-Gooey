/*! ****************************************************************************
\file                Matrix2D.cpp
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\brief
  Implementation of the Matrix2D class defined in Matrix2D.h

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cstdint>
#include <cmath>
#include <cstring>

#include "Matrix2D.h"
#include "Vector2D.h"

// Public Functions                       //////////////////////////////////////

Matrix2D::Matrix2D(float * arr)
{
    // If the passed array exists
  if(arr)
  {
    for(uint32_t i = 0; i < 9; ++i)
    {
      m_Matrix[i/3][i%3] = arr[i];
    }
  }
}

float * Matrix2D::operator[](uint32_t i)
{
  return m_Matrix[i];
}

float const * Matrix2D::operator[](uint32_t i) const
{
  return m_Matrix[i];
}

Vector2D Matrix2D::operator*(Vector2D const & rhs)
{
  return Vector2D(m_Matrix[0][0] * rhs[0] + m_Matrix[0][1] * rhs[1] + m_Matrix[0][2],
                  m_Matrix[1][0] * rhs[0] + m_Matrix[1][1] * rhs[1] + m_Matrix[1][2]);
}

Matrix2D Matrix2D::operator*(Matrix2D const & rhs)
{
  Matrix2D temp;

  for(uint32_t i = 0; i < 27; ++i)
  {
    temp[(i/3)%3][i%3] += m_Matrix[(i/3)%3][i/9] * rhs.m_Matrix[i/9][i%3];
  }

  return temp;
}

void Matrix2D::Identity()
{
  std::memset(m_Matrix, 0, sizeof(float)*9);

  m_Matrix[0][0] = m_Matrix[1][1] = m_Matrix[2][2] = 1;
}

void Matrix2D::Transpose()
{
  float temp[3][3];

  for(uint32_t i = 0; i < 9; ++i)
  {
    temp[i/3][i%3] = m_Matrix[i%3][i/3];
  }

  std::memcpy(m_Matrix, temp, sizeof(float) * 9);
}

void Matrix2D::Translate(float x, float y)
{
  Identity();

  m_Matrix[0][2] = x;
  m_Matrix[1][2] = y;
}

void Matrix2D::Translate(Vector2D const & v)
{
  Identity();

  m_Matrix[0][2] = v[0];
  m_Matrix[1][2] = v[1];
}

void Matrix2D::Scale(float x, float y)
{
  Identity();

  m_Matrix[0][0] = x;
  m_Matrix[1][1] = y;
}

void Matrix2D::Rotate(float rad)
{
  Identity();

  m_Matrix[0][0] =  std::cos(rad);
  m_Matrix[0][1] = -std::sin(rad);
  m_Matrix[1][0] =  std::sin(rad);
  m_Matrix[1][1] =  std::cos(rad);
}

// Private Functions                      //////////////////////////////////////
