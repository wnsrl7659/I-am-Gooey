/******************************************************************************/
/*!
\file   SquareMesh.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
SquareMesh class, which handle information of Square mesh

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "SquareMesh.h"

const Point SquareMesh::vertices[4]
= { Point(-0.5,-0.5),  // 0
	Point(0.5,-0.5),   // 1
	Point(0.5,0.5),    // 2
	Point(-0.5,0.5)    // 3
};


const Mesh::Face SquareMesh::faces[2]
= { Face(0,1,2),  Face(0,2,3) };


const Mesh::Edge SquareMesh::edges[4]
= { Edge(0,1),  Edge(1,2),  Edge(2,3),  Edge(3,0) };


const Mesh::Face SquareMesh::color[4]
= { Face(1,1,1),  Face(1,1,1), Face(1,1,1),  Face(1,1,1) };


const Mesh::Edge SquareMesh::text[4]
= { Edge(0,0),  Edge(1,0),  Edge(1,1),  Edge(0,1) };


/**************************************************************************/
/*!
  \brief  return vertex count of the mesh
  \return int
	  vertex count of the mesh
*/
/**************************************************************************/
int SquareMesh::vertexCount(void) {
	return 4;
}

/**************************************************************************/
  /*!
	\brief  return const array of vertices
	\return const Point*
		const array of vertices
  */
  /**************************************************************************/
const Point* SquareMesh::vertexArray(void) {
	return vertices;
}

/**************************************************************************/
  /*!
	\brief  return dimension (2x2)
	\return Vector
		dimension of the mesh (2x2)
  */
  /**************************************************************************/
Vector SquareMesh::dimensions(void) {
	return Vector(2, 2);
}

/**************************************************************************/
  /*!
	\brief  return center position of the mesh
	\return Point
		center position of the mesh
  */
  /**************************************************************************/
Point SquareMesh::center(void) {
	return Point(0, 0);
}

/**************************************************************************/
  /*!
	\brief  return face count of the mesh
	\return int
		face count of the mesh
  */
  /**************************************************************************/
int SquareMesh::faceCount(void) {
	return 2;
}

/**************************************************************************/
  /*!
	\brief  return faces of the mesh
	\return const Face*
		face array of the mesh
  */
  /**************************************************************************/
const Mesh::Face* SquareMesh::faceArray(void) {
	return faces;
}

/**************************************************************************/
  /*!
	\brief  return edge count of the mesh
	\return int
		edge count of the mesh
  */
  /**************************************************************************/
int SquareMesh::edgeCount(void) {
	return 4;
}

/**************************************************************************/
  /*!
	\brief  return edges array of the mesh
	\return const Edge*
		edges array of the mesh
  */
  /**************************************************************************/
const Mesh::Edge* SquareMesh::edgeArray(void) {
	return edges;
}

/**************************************************************************/
  /*!
	\brief  return color array of the mesh
	\return const Face*
		color array of the mesh
  */
  /**************************************************************************/
const Mesh::Face* SquareMesh::colorArray(void) {
	return color;
}

/**************************************************************************/
  /*!
	\brief  return texture uv coord array of the mesh
	\return const Edge*
		texture uv coord array of the mesh
  */
  /**************************************************************************/
const Mesh::Edge* SquareMesh::textArray(void) {
	return text;
}
