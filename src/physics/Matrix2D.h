/*! ****************************************************************************
\file                Matrix2D.h
\author              Chyler Morrison

\par  DigiPen login: morrison.c

\par        Project: Radiant (Volunteer)

\brief
  A class for handling matrix calculations in 2D space.

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*******************************************************************************/

#ifndef __MATRIX2D_H
#define __MATRIX2D_H

// Include Files                          ////////////////////////////////////// 

#include <cstdint>

// Forward Namespace References ////////////////////////////////////////////////

class Vector2D;

// Public Namespace Objects     ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  A class for matrix math in 2D space
*******************************************************************************/
class Matrix2D
{
private:

  // Members              ///////////////////////

  float m_Matrix[3][3];

public:

  // Con-/De- structors   ///////////////////////

  Matrix2D(float * arr = nullptr);      ///< Default constructor
  Matrix2D(Matrix2D const &) = default; ///< Default copy constructor
  Matrix2D(Matrix2D &&) = default;      ///< Default move constructor

  ~Matrix2D() = default;                ///< Default destructor

  // Operators            ///////////////////////

  Matrix2D & operator=(Matrix2D const &) = default; ///< Default copy assignment
  Matrix2D & operator=(Matrix2D &&) = default;      ///< Default move assignment

    /// Element access. [0,1] -> [x,y]
  float * operator[](uint32_t);
  float const * operator[](uint32_t) const;

    /// Matrix-Vector mult.
  Vector2D operator*(Vector2D const &);
    /// Matrix-Matrix mult.
  Matrix2D operator*(Matrix2D const &);

  // Functions            ///////////////////////

    /// Sets the matrix to the identity matrix
  void Identity();
    /// Transposes the matrix
  void Transpose();
    /// Sets the matrix to a translation
  void Translate(float x, float y);
  void Translate(Vector2D const &);
    /// Sets the matrix to a scale
  void Scale(float x, float y);
    /// Sets the matrix to an angle
  void Rotate(float rad);

}; // class Matrix2D

#endif // __MATRIX2D_H
