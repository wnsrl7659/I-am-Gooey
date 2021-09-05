/******************************************************************************/
/*!
\file   Affine.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Affine struct, which handle calculation of 3x3 matrix

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#ifndef AFFINE_H
#define AFFINE_H

#include <cmath>
#include <cassert>

struct Hcoord {
  float x, y, w;

  /**************************************************************************/
	/*!
	  \brief  defualt constructer
	  \param X
		  X value
	  \param Y
		  Y value
	  \param W
		  W value
	*/
  /**************************************************************************/
  explicit Hcoord(float X=0, float Y=0, float W=0);
  float& operator[](int i) { return *(&x+i); }
  float operator[](int i) const { return *(&x+i); }
  static bool nearr(float x, float y) { return std::abs(x-y) < 1e-5f; }
};


struct Point : Hcoord {
  /**************************************************************************/
	/*!
	  \brief  defualt constructer
	  \param X
		  X value
	  \param Y
		  Y value
	*/
  /**************************************************************************/
  explicit Point(float X=0, float Y=0);
  Point(const Hcoord& v) : Hcoord(v) { assert(nearr(w,1)); }
};


struct Vector : Hcoord {
  /**************************************************************************/
	/*!
	  \brief  defualt constructer
	  \param X
		  X value
	  \param Y
		  Y value
	*/
  /**************************************************************************/
  explicit Vector(float X=0, float Y=0);
  Vector(const Hcoord& v) : Hcoord(v) { assert(nearr(w,0)); }
};


struct Affine {
  Hcoord row[3];
  /**************************************************************************/
	/*!
	  \brief  defualt constructer
	*/
  /**************************************************************************/
  Affine(void);
  /**************************************************************************/
	/*!
	  \brief  defualt constructer
	  \param Lx
		  1st column vecter
	  \param Ly
		  2nd column vector
	  \param disp
	      3rd column position point
	*/
	/**************************************************************************/
  Affine(const Vector& Lx, const Vector& Ly, const Point& disp);
  Hcoord& operator[](int i) { return row[i]; }
  const Hcoord& operator[](int i) const { return row[i]; }
};


/**************************************************************************/
  /*!
	\brief  + operator for Hcoord calculation
	\param u
		Hcoordinate
	\param v
		other Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator+(const Hcoord& u, const Hcoord& v);
/**************************************************************************/
  /*!
	\brief  - operator for Hcoord calculation
	\param u
		Hcoordinate
	\param v
		other Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator-(const Hcoord& u, const Hcoord& v);
/**************************************************************************/
  /*!
	\brief  -(negative) operator for Hcoord calculation
	\param u
		Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator-(const Hcoord& v);
/**************************************************************************/
  /*!
	\brief  * operator for Hcoord calculation
	\param r
		float multiply value
	\param v
		Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator*(float r, const Hcoord& v);
/**************************************************************************/
  /*!
	\brief  * operator for Affine matrix multiply by Hcoord 
	\param A
		Affine matrix
	\param v
		Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator*(const Affine& A, const Hcoord& v);
/**************************************************************************/
  /*!
	\brief  * operator for Affine matrix 
	\param A
		Affine matrix
	\param B
		other Affine
	\return Affine
		result
  */
  /**************************************************************************/
Affine operator*(const Affine& A, const Affine& B);

/**************************************************************************/
  /*!
	\brief  dot product between 2 vectors
	\param u
		vector
	\param v
		other vector
  */
/**************************************************************************/
float dot(const Vector& u, const Vector& v);
/**************************************************************************/
  /*!
	\brief  absolute value of vector
	\param v
		vector
  */
/**************************************************************************/
float abs(const Vector& v);
/**************************************************************************/
  /*!
	\brief  normalize the vector
	\param v
		vector
	\return Vector
		normalized vector
  */
/**************************************************************************/
Vector normalize(const Vector& v);
/**************************************************************************/
  /*!
	\brief  return 3x3 affine matrix with respect to rotation rate
	\param t
		rotation rate
	\return Affine
		3x3 affine rotated matrix
  */
  /**************************************************************************/
Affine rotate(float t);
/**************************************************************************/
  /*!
	\brief  return 3x3 affine translate matrix w.r.t vector
	\param v
	    vector
	\return Affine
		3x3 affine translated matrix
  */
  /**************************************************************************/
Affine translate(const Vector& v);
/**************************************************************************/
  /*!
	\brief  return 3x3 affine matrix with respect to scale rate
	\param r
		scale rate
	\return Affine
		3x3 affine scaled matrix
  */
  /**************************************************************************/
Affine scale(float r);
/**************************************************************************/
  /*!
	\brief  return 3x3 affine matrix with respect to scale rate
	\param rx
		width scale rate
	\param ry
		height scale rate
	\return Affine
		3x3 affine scaled matrix
  */
  /**************************************************************************/
Affine scale(float rx, float ry);
/**************************************************************************/
  /*!
	\brief  return 3x3 affine inverse matrix 
	\param A
		Affine matrix
	\return Affine
		3x3 affine inverse matrix
  */
  /**************************************************************************/
Affine inverse(const Affine& A);



#endif

