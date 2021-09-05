/******************************************************************************/
/*!
\file   Render.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Render class, which handle every drawing function with using openGL

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Render.h"
#include "Shader.h"
#include "MeshManager.h"
#include <iostream>
#include <glm/gtx/matrix_transform_2d.hpp>

static glm::mat3 origin = {};


/**************************************************************************/
  /*!
	\brief  Initializer
  */
  /**************************************************************************/
Render::Render(int shader)
{
	origin[0][0] = 1;
	origin[1][1] = 1;
	origin[2][2] = 1;

	program = ShaderArray[shader];
	shaderIndex = shader;

	//Set the value of private data members 
	utransform = location[shader].utransform;
	uview = location[shader].uview;
	uprojection = location[shader].uprojection;
	ualpha = location[shader].image_alpha;
	ucolor = location[shader].uColor;

	image_alpha = 1.0f;
	//Set various guaranteed Uniforms here
	glUniform1f(location[shader].image_alpha, image_alpha);

	transform = origin;
	rscale = origin;
	rotation = origin;
}
/**************************************************************************/
  /*!
	\brief  destructer
  */
  /**************************************************************************/
Render::~Render(void)
{
	// dont delete shaders, ShaderShutdown() does this
}

/**************************************************************************/
  /*!
	\brief  clear frame buffer with color
	\param c
	  color value
  */
  /**************************************************************************/
void Render::clearFrame(const glm::vec3& c)
{
	//clear background
	//glClearColor(c.x, c.y, c.w, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

/**************************************************************************/
  /*!
	\brief  make sprite from model space to world space
	\param camera
	  main camera value
  */
  /**************************************************************************/
void Render::setModelToWorld(Camera camera)
{
		/*
		int ViewportData[4];
		glGetIntegerv(GL_VIEWPORT, ViewportData);
		float Vwidth = (float)ViewportData[2];
		float Vheight = (float)ViewportData[3];
		float ratio = Vwidth / Vheight;
		/**/

	glm::mat3 M = transform * rotation * rscale;
	glm::mat3 V = worldToCamera(camera);
	glm::mat3 P = cameraToNDC(camera);

	//Viewport Version
	//Affine P = scale(2.0f / Vwidth, 2.0f / Vheight);

	// select vertex buffer
	glUseProgram(program);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	// set vertex transformation
	float R[9] = { M[0][0], M[1][0], M[2][0],
				   M[0][1], M[1][1], M[2][1],
				   M[0][2], M[1][2], M[2][2] };
	float C[9] = { V[0][0], V[1][0], V[2][0],
				   V[0][1], V[1][1], V[2][1],
				   V[0][2], V[1][2], V[2][2] };
	float O[9] = { P[0][0], P[1][0], P[2][0],
				   P[0][1], P[1][1], P[2][1],
				   P[0][2], P[1][2], P[2][2] };

	GLint uniformLocation = location[shaderIndex].utransform;
	if (uniformLocation != -1)
			glUniformMatrix3fv(uniformLocation, 1, true, R);
	uniformLocation = location[shaderIndex].uview;
	if (uniformLocation != -1)
			glUniformMatrix3fv(uniformLocation, 1, true, C);
	uniformLocation = location[shaderIndex].uprojection;
	if (uniformLocation != -1)
			glUniformMatrix3fv(uniformLocation, 1, true, O);
}

/**************************************************************************/
  /*!
	\brief  load mesh buffer and set it to member variable
	\param id
	  mesh id
  */
  /**************************************************************************/
void Render::loadMesh(int id)
{
	Buffer* temp = GetBuffer(id);
	vertex_buffer = temp->vertex_buffer;
	face_buffer = temp->face_buffer;
	vertex_array = temp->vertex_array;
	vertex_aarray = temp->vertex_aarray;
	edge_buffer = temp->edge_buffer;
	mesh_edge_count = temp->mesh_edge_count;
	mesh_face_count = temp->mesh_face_count;
}
/**************************************************************************/
  /*!
	\brief  delete every mesh buffer
  */
  /**************************************************************************/
void Render::unloadMesh(void)
{
//	// delete GPU buffers used
//	glDeleteBuffers(1, &face_buffer);
//	//glDeleteBuffers(1, &edge_buffer);
//	glDeleteBuffers(1, &vertex_buffer);
//	glDeleteVertexArrays(1, &vertex_array);
}

/**************************************************************************/
  /*!
	\brief  load texture buffer with id and set it to memeber variable
	\param id
	  texture buffer id
  */
  /**************************************************************************/
void Render::loadTexture(const int id)
{
	texture_buffer = GetTexture(id);
}
/**************************************************************************/
  /*!
	\brief  get Texture buffer
	\return int
	  texture buffer member variable
  */
  /**************************************************************************/
int Render::getTexture()
{
	return texture_buffer;
}
/**************************************************************************/
  /*!
	\brief  rendering the edge with color
	\param c
	  color value
  */
  /**************************************************************************/
void Render::displayEdges(const glm::vec3& c)
{
	// draw edges
	float color[3] = { c.x, c.y, c.z };
	glUniform3f(ucolor, color[0], color[1], color[2]);
	glBindVertexArray(vertex_aarray);
	glDrawElements(GL_LINES, mesh_edge_count * 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/**************************************************************************/
  /*!
	\brief  rendering the face with color
	\param c
	  color value
  */
  /**************************************************************************/
void Render::displayFaces(const glm::vec3& c)
{
	// draw faces
	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, mesh_face_count * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/**************************************************************************/
  /*!
	\brief  rendering the texuture
  */
  /**************************************************************************/
void Render::displayTexture(void)
{
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture_buffer);

	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/**************************************************************************/
  /*!
	\brief  set sprite position
	\param x
	  x coordinate
	\param y
	  y coordinate
  */
  /**************************************************************************/
void Render::setTransform(float X, float Y)
{
	transform = origin;
	transform[2][0] = X;
	transform[2][1] = Y;
}

/**************************************************************************/
  /*!
	\brief  set shader with id and put into member variablue
	\param id
	  shader array order
  */
  /**************************************************************************/
void Render::setShader(int id)
{
		program = ShaderArray[id];
		shaderIndex = id;
}

/**************************************************************************/
  /*!
	\brief  set sprite size
	\param ratex
	  width
	\param ratey
	  height
  */
  /**************************************************************************/
void Render::setScale(float ratex, float ratey)
{
	rscale = origin;
	rscale[0][0] = ratex;
	rscale[1][1] = ratey;
}

/**************************************************************************/
  /*!
	\brief  set sprite rotation
	\param rate
	  rotation rate
  */
  /**************************************************************************/
void Render::setRotate(float rate)
{
	glm::mat3 tempM = origin;
	rotation = glm::rotate(tempM, rate);
}

/**************************************************************************/
  /*!
	\brief  get shader program
	\return GLuint
	  shader program value
  */
  /**************************************************************************/
GLuint Render::getShader()
{
	return program;
}
/**************************************************************************/
  /*!
	\brief return sprite's alpha value
	\return float
	  sprite's alpha value
  */
  /**************************************************************************/
float Render::getAlpha()
{
	return image_alpha;
}
/**************************************************************************/
  /*!
	\brief  set alpha value in shader
  */
  /**************************************************************************/
void Render::setAlphaUniform()
{
		glUniform1f(location[shaderIndex].image_alpha, image_alpha);
}
/**************************************************************************/
  /*!
	\brief  set sprite's alpha value
	\param zeroToOne
	  alpha value
  */
  /**************************************************************************/
void Render::setAlpha(float zeroToOne) 
{
	image_alpha = zeroToOne; 
}