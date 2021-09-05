/******************************************************************************/
/*!
\file   Affine.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Affine struct, which handle calculation of 3x3 matrix

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Affine.h"

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
Hcoord::Hcoord(float X, float Y, float W)
{
	x = X;
	y = Y;
	w = W;
}

/**************************************************************************/
  /*!
	\brief  defualt constructer
	\param X
		X value
	\param Y
		Y value
  */
/**************************************************************************/
Point::Point(float X, float Y)
{
	x = X;
	y = Y;
	w = 1; // w value of Point is 1
}

/**************************************************************************/
  /*!
	\brief  defualt constructer
	\param X
		X value
	\param Y
		Y value
  */
/**************************************************************************/
Vector::Vector(float X, float Y)
{
	x = X;
	y = Y;
	w = 0; // w value of Vector is 0
}

/**************************************************************************/
  /*!
	\brief  defualt constructer
  */
/**************************************************************************/
Affine::Affine(void)
{
	for (int i = 0; i < 3; i++)
	{
		row[i] = Hcoord(0, 0, 0);
	}
	row[2].w = 1; // same as the matrix for uniform scaling by 0 
}

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
Affine::Affine(const Vector& Lx, const Vector& Ly, const Point& disp)
{
	for (int i = 0; i < 3; i++)
	{
		row[i] = Hcoord(Lx[i], Ly[i], disp[i]);
	}
}

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
Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	float w = 0;
	//Error check for situation a[2][2] == 2;
	if (u.w == 1 && v.w == 1)
	{
		w = 1;
	}
	else
	{
		w = u.w + v.w;
	}
	Hcoord a(u.x+v.x, u.y+v.y, w);
	return a;
}

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
Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	Hcoord a(u.x - v.x, u.y - v.y, std::abs(u.w - v.w));
	return a;
}

/**************************************************************************/
  /*!
	\brief  -(negative) operator for Hcoord calculation
	\param u
		Hcoordinate
	\return Hcoord
		result
  */
  /**************************************************************************/
Hcoord operator-(const Hcoord& v)
{
	Hcoord a(-v.x, -v.y, v.w);
	return a;
}

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
Hcoord operator*(float r, const Hcoord& v)
{
	Hcoord a(r*v.x, r*v.y, v.w);
	return a;
}

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
Hcoord operator*(const Affine& A, const Hcoord& v)
{
	Hcoord a;
	//Simplification
	for (int i = 0; i < 3; i++)
	{
		a[i] = A.row[i].x * v.x + A.row[i].y * v.y + A.row[i].w * v.w;
	}
	return a;
}

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
Affine operator*(const Affine& A, const Affine& B)
{
	Affine a;
	// first-version
	/*
	a.row[0].x = A.row[0].x * B.row[0].x + A.row[0].y * B.row[1].x + A.row[0].w * B.row[2].x;
	a.row[0].y = A.row[0].x * B.row[0].y + A.row[0].y * B.row[1].y + A.row[0].w * B.row[2].y;
	a.row[0].w = A.row[0].x * B.row[0].w + A.row[0].y * B.row[1].w + A.row[0].w * B.row[2].w;
	a.row[1].x = A.row[1].x * B.row[0].x + A.row[1].y * B.row[1].x + A.row[1].w * B.row[2].x;
	a.row[1].y = A.row[1].x * B.row[0].y + A.row[1].y * B.row[1].y + A.row[1].w * B.row[2].y;
	a.row[1].w = A.row[1].x * B.row[0].w + A.row[1].y * B.row[1].w + A.row[1].w * B.row[2].w;
	a.row[2].x = A.row[2].x * B.row[0].x + A.row[2].y * B.row[1].x + A.row[2].w * B.row[2].x;
	a.row[2].y = A.row[2].x * B.row[0].y + A.row[2].y * B.row[1].y + A.row[2].w * B.row[2].y;
	a.row[2].w = A.row[2].x * B.row[0].w + A.row[2].y * B.row[1].w + A.row[2].w * B.row[2].w;
	*/
	// second-version
	/*
	a.row[0][0] = A.row[0].x * B.row[0][0] + A.row[0].y * B.row[1][0] + A.row[0].w * B.row[2][0];
	a.row[0][1] = A.row[0].x * B.row[0][1] + A.row[0].y * B.row[1][1] + A.row[0].w * B.row[2][1];
	a.row[0][2] = A.row[0].x * B.row[0][2] + A.row[0].y * B.row[1][2] + A.row[0].w * B.row[2][2];
	a.row[1][0] = A.row[1].x * B.row[0][0] + A.row[1].y * B.row[1][0] + A.row[1].w * B.row[2][0];
	a.row[1][1] = A.row[1].x * B.row[0][1] + A.row[1].y * B.row[1][1] + A.row[1].w * B.row[2][1];
	a.row[1][2] = A.row[1].x * B.row[0][2] + A.row[1].y * B.row[1][2] + A.row[1].w * B.row[2][2];
	a.row[2][0] = A.row[2].x * B.row[0][0] + A.row[2].y * B.row[1][0] + A.row[2].w * B.row[2][0];
	a.row[2][1] = A.row[2].x * B.row[0][1] + A.row[2].y * B.row[1][1] + A.row[2].w * B.row[2][1];
	a.row[2][2] = A.row[2].x * B.row[0][2] + A.row[2].y * B.row[1][2] + A.row[2].w * B.row[2][2];
	*/
	//simplification
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			a.row[i][j] = A.row[i][0] * B.row[0][j] + A.row[i][1] * B.row[1][j] + A.row[i][2] * B.row[2][j];
		}
	}
	return a;
}

/**************************************************************************/
  /*!
	\brief  dot product between 2 vectors
	\param u
		vector
	\param v
		other vector
  */
  /**************************************************************************/
float dot(const Vector& u, const Vector& v) // dot fomula = x1*x2 + y1*y2
{
	return u.x*v.x + u.y*v.y;
}

/**************************************************************************/
  /*!
	\brief  absolute value of vector
	\param v
		vector
  */
  /**************************************************************************/
float abs(const Vector& v) // length of vector fomula = x^2 + y^2 
{
	return sqrt(v.x*v.x + v.y*v.y);
}

/**************************************************************************/
  /*!
	\brief  normalize the vector
	\param v
		vector
	\return Vector
		normalized vector
  */
  /**************************************************************************/
Vector normalize(const Vector& v) //normalization fomula = vector / length of vector
{
	Vector a;
	a.x = v.x / abs(v);
	a.y = v.y / abs(v);
	return a;
}

/**************************************************************************/
  /*!
	\brief  return 3x3 affine matrix with respect to rotation rate
	\param t
		rotation rate
	\return Affine
		3x3 affine rotated matrix
  */
  /**************************************************************************/
Affine rotate(float t) // counterclockwise rotation by angle t about origin
{
	Affine a; 
	a.row[0][0] = cos(t);
	a.row[0][1] = -sin(t);
	a.row[1][0] = sin(t);
	a.row[1][1] = cos(t);
	return a;
}

/**************************************************************************/
  /*!
	\brief  return 3x3 affine translate matrix w.r.t vector
	\param v
		vector
	\return Affine
		3x3 affine translated matrix
  */
  /**************************************************************************/
Affine translate(const Vector& v) //Basic transformation - Translation
{
	Affine a; 
	a.row[0][0] = 1;
	a.row[1][1] = 1;
	a.row[0][2] = v.x;
	a.row[1][2] = v.y;
	return a;
}

/**************************************************************************/
  /*!
	\brief  return 3x3 affine matrix with respect to scale rate
	\param r
		scale rate
	\return Affine
		3x3 affine scaled matrix
  */
  /**************************************************************************/
Affine scale(float r) //Basic transformation - Uniform scaling
{
	Affine a;
	a.row[0][0] = r;
	a.row[1][1] = r;
	return a;
}

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
Affine scale(float rx, float ry) //Basic transformation - non-Uniform scaling
{
	Affine a;
	a.row[0][0] = rx;
	a.row[1][1] = ry;
	return a;
}

/**************************************************************************/
  /*!
	\brief  return 3x3 affine inverse matrix
	\param A
		Affine matrix
	\return Affine
		3x3 affine inverse matrix
  */
  /**************************************************************************/
Affine inverse(const Affine& A)
{
	Affine Result;
	Affine T;
	Affine L;
	float det = 0.0f;

	//setting Affine T(-v)
	T.row[0].x = 1;
	T.row[1].y = 1;
	T.row[0].w = -A.row[0].w;
	T.row[1].w = -A.row[1].w;

	//setting det A
	det = 1 / (A.row[0].x * A.row[1].y - A.row[0].y * A.row[1].x);

	///setting Affine L(-1)
	L.row[0].x = det * A.row[1].y;
	L.row[1].y = det * A.row[0].x;
	L.row[0].y = det * (-A.row[0].y);
	L.row[1].x = det * (-A.row[1].x);

	Result = L * T;

	return Result;
}