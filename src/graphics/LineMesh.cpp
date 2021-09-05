/******************************************************************************/
/*!
\file   LineMesh.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
LineMesh class, which handle information of line mesh

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "LineMesh.h"

Point LineMesh::vertices[2]
= { Point(0.0,0.0),
	Point(0.5,0)
};


const Mesh::Face LineMesh::faces[1]
= { Face(0,1,0) };


const Mesh::Edge LineMesh::edges[1]
= { Edge(0,1) };


const Mesh::Face LineMesh::color[2]
= { Face(1,1,1), Face(1,1,1) };


const Mesh::Edge LineMesh::text[2]
= { Edge(0,0),  Edge(1,0) };

/**************************************************************************/
	/*!
	  \brief  return vertex count of the mesh
	  \return int
		  vertex count of the mesh
	*/
/**************************************************************************/
int LineMesh::vertexCount(void) {
	return 2;
}

/**************************************************************************/
  /*!
	\brief  return const array of vertices
	\return const Point*
		const array of vertices
  */
  /**************************************************************************/
const Point* LineMesh::vertexArray(void) {
	return vertices;
}

/**************************************************************************/
  /*!
	\brief  return dimension (2x2)
	\return Vector
		dimension of the mesh (2x2)
  */
  /**************************************************************************/
Vector LineMesh::dimensions(void) {
	return Vector(2, 2);
}

/**************************************************************************/
  /*!
	\brief  return center position of the mesh
	\return Point
		center position of the mesh
  */
  /**************************************************************************/
Point LineMesh::center(void) {
	return Point(0, 0);
}

/**************************************************************************/
  /*!
	\brief  return face count of the mesh
	\return int
		face count of the mesh
  */
  /**************************************************************************/
int LineMesh::faceCount(void) {
	return 1;
}

/**************************************************************************/
  /*!
	\brief  return faces of the mesh
	\return const Face*
		face array of the mesh
  */
  /**************************************************************************/
const Mesh::Face* LineMesh::faceArray(void) {
	return faces;
}

/**************************************************************************/
  /*!
	\brief  return edge count of the mesh
	\return int
		edge count of the mesh
  */
  /**************************************************************************/
int LineMesh::edgeCount(void) {
	return 1;
}

/**************************************************************************/
  /*!
	\brief  return edges array of the mesh
	\return const Edge*
		edges array of the mesh
  */
  /**************************************************************************/
const Mesh::Edge* LineMesh::edgeArray(void) {
	return edges;
}

/**************************************************************************/
  /*!
	\brief  return color array of the mesh
	\return const Face*
		color array of the mesh
  */
  /**************************************************************************/
const Mesh::Face* LineMesh::colorArray(void) {
	return color;
}

/**************************************************************************/
  /*!
	\brief  return texture uv coord array of the mesh
	\return const Edge*
		texture uv coord array of the mesh
  */
  /**************************************************************************/
const Mesh::Edge* LineMesh::textArray(void) {
	return text;
}

/**************************************************************************/
  /*!
	\brief  change the member variable first point function
	\param x
		x coordinate
	\param y
		y coordinate
  */
  /**************************************************************************/
void LineMesh::SetVertexA(float x, float y)
{
	vertices[0].x = x;
	vertices[0].y = y;
}

/**************************************************************************/
  /*!
	\brief  change the member variable second point function
	\param x
		x coordinate
	\param y
		y coordinate
  */
  /**************************************************************************/
void LineMesh::SetVertexB(float x, float y)
{
	vertices[1].x = x;
	vertices[1].y = y;
}