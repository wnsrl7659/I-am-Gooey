/*! ****************************************************************************
\file                Vector2D.h
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\brief
  A class for handling vector calculations in 2D space.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

#ifndef __VECTOR2D_H
#define __VECTOR2D_H

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

// Public Namespace Objects     ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  Class for vector math in 2D space.
*******************************************************************************/
class Vector2D
{
private:

  // Members              ///////////////////////

    /// The points of the vector
  float m_A, m_B;

public:

  // Con-/De- structors   ///////////////////////

  Vector2D(float a = 0.f, float b = 0.f); ///< Default constructor
  Vector2D(Vector2D const &) = default;   ///< Default copy constructor
  Vector2D(Vector2D &&) = default;        ///< Default move constructor

  ~Vector2D() = default;                  ///< Default destructor

  // Operators            ///////////////////////

  Vector2D & operator=(Vector2D const &) = default; ///< Default copy assignment
  Vector2D & operator=(Vector2D &&) = default;      ///< Default move assignment

    /// Element access. [0,1] -> [x,y]
  float & operator[](uint32_t);
  float operator[](uint32_t) const;

    /// Unary negative (x,y) -> (-x,-y)
  Vector2D operator-() const;
    /// Binary addition
  Vector2D operator+(Vector2D const &) const;
    /// Binary subtraction
  Vector2D operator-(Vector2D const &) const;
    /// Scale
  Vector2D operator*(float) const;
    /// Dot product
  float operator*(Vector2D const &) const;

    /// Binary addition
  Vector2D & operator+=(Vector2D const &);
    /// Binary subtraction
  Vector2D & operator-=(Vector2D const &);
    /// Scale
  Vector2D & operator*=(float);

  // Functions            ///////////////////////

    /// Sets the vector to 0s
  void Zero();
    /// Sets the vector to the given values
  void Set(float a, float b);
    /// Normalizes the vector to length 1
  void Normalize();
    /// Returns the length of the vector
  float Length() const;
    /// Returns the square of the length of the vector
  float SquareLength() const;
    /// Returns the distance between two vectors
  float Distance(Vector2D const &) const;
    /// Returns the square distance between two vectors
  float SquareDistance(Vector2D const &) const;
    /// Sets the vector to unit lengith at given angle (in radians)
  void Angle(float);

}; // class Vector2D

#endif // __VECTOR2D_H
