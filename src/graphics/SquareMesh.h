/******************************************************************************/
/*!
\file   SquareMesh.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
SquareMesh class, which handle information of Square mesh

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#ifndef SQUAREMESH_H
#define SQUAREMESH_H

#include "Mesh.h"


class SquareMesh : public Mesh {
  public:
	  /**************************************************************************/
		/*!
		  \brief  return vertex count of the mesh
		  \return int
			  vertex count of the mesh
		*/
		/**************************************************************************/
    int vertexCount(void);
	/**************************************************************************/
	  /*!
		\brief  return const array of vertices
		\return const Point*
			const array of vertices
	  */
	  /**************************************************************************/
    const Point* vertexArray(void);
	/**************************************************************************/
	  /*!
		\brief  return dimension (2x2)
		\return Vector
			dimension of the mesh (2x2)
	  */
	  /**************************************************************************/
    Vector dimensions(void);
	/**************************************************************************/
	  /*!
		\brief  return center position of the mesh
		\return Point
			center position of the mesh
	  */
	  /**************************************************************************/
    Point center(void);
	/**************************************************************************/
	  /*!
		\brief  return face count of the mesh
		\return int
			face count of the mesh
	  */
	  /**************************************************************************/
    int faceCount(void);
	/**************************************************************************/
	  /*!
		\brief  return faces of the mesh
		\return const Face*
			face array of the mesh
	  */
	  /**************************************************************************/
    const Face* faceArray(void);
	/**************************************************************************/
	  /*!
		\brief  return edge count of the mesh
		\return int
			edge count of the mesh
	  */
	  /**************************************************************************/
    int edgeCount(void);
	/**************************************************************************/
	  /*!
		\brief  return edges array of the mesh
		\return const Edge*
			edges array of the mesh
	  */
	  /**************************************************************************/
	const Edge* edgeArray(void);
	/**************************************************************************/
	  /*!
		\brief  return color array of the mesh
		\return const Face*
			color array of the mesh
	  */
	  /**************************************************************************/
	const Face* colorArray(void);
	/**************************************************************************/
	  /*!
		\brief  return texture uv coord array of the mesh
		\return const Edge*
			texture uv coord array of the mesh
	  */
	  /**************************************************************************/
	const Edge* textArray(void);
  private:
    static const Point vertices[4];
    static const Face faces[2];
    static const Edge edges[4];
	static const Face color[4];
	static const Edge text[4];
};

#endif

