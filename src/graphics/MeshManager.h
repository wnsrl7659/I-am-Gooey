/******************************************************************************/
/*!
\file   MeshManager.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Mesh Manager, which handle buffers of Meshs

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <GLFW/glfw3.h>

typedef struct Buffer {
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint vertex_aarray;
	GLuint face_buffer;
	GLuint edge_buffer;
	int mesh_edge_count;
	int	mesh_face_count;
}Buffer;

/**************************************************************************/
  /*!
	\brief  Initializer
  */
/**************************************************************************/
void MeshManagerInit();

/**************************************************************************/
  /*!
	\brief  Update
	\param dt
	    time value
  */
  /**************************************************************************/
void MeshManagerUpdate(float dt);

/**************************************************************************/
  /*!
	\brief  common Shutdown function(desturcter)
  */
  /**************************************************************************/
void MeshManagerShutdown();

/**************************************************************************/
  /*!
	\brief  Create the buffer from mesh
	\param id
		mesh array's order
  */
  /**************************************************************************/
void CreateBuffer(int id);


/**************************************************************************/
  /*!
	\brief  return specific mesh buffer depend on id
	\param id
		mesh array's order
	\return Buffer*
		one of mesh array depend on id
  */
  /**************************************************************************/
Buffer* GetBuffer(const int id);



#endif

