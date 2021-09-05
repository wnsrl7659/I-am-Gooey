/******************************************************************************/
/*!
\file   Render.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
Render class, which handle every drawing function with using openGL

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Camera.h"
#include "TextureManager.h"

#include "../animation/animation.h"

class Render {
  public:
	  /**************************************************************************/
		/*!
		  \brief  Initializer
		*/
	  /**************************************************************************/
	  Render(int);
	  /**************************************************************************/
		/*!
		  \brief  destructer
		*/
		/**************************************************************************/
	  ~Render(void);
	  /**************************************************************************/
		/*!
		  \brief  clear frame buffer with color
		  \param c
			color value
		*/
		/**************************************************************************/
	  void clearFrame(const glm::vec3& c);
	  /**************************************************************************/
		/*!
		  \brief  make sprite from model space to world space
		  \param camera
			main camera value
		*/
		/**************************************************************************/
	  void setModelToWorld(Camera camera);
	  /**************************************************************************/
		/*!
		  \brief  load mesh buffer and set it to member variable
		  \param id
			mesh id 
		*/
		/**************************************************************************/
	  void loadMesh(int id);
	  /**************************************************************************/
		/*!
		  \brief  delete every mesh buffer
		*/
		/**************************************************************************/
	  void unloadMesh(void);
	  /**************************************************************************/
		/*!
		  \brief  load texture buffer with id and set it to memeber variable
		  \param id
		    texture buffer id
		*/
		/**************************************************************************/
	  void loadTexture(const int id);
	  /**************************************************************************/
		/*!
		  \brief  get Texture buffer 
		  \return int
			texture buffer member variable
		*/
		/**************************************************************************/
	  int getTexture();
	  /**************************************************************************/
		/*!
		  \brief  rendering the edge with color
		  \param c
		    color value
		*/
		/**************************************************************************/
	  void displayEdges(const glm::vec3& c);
	  /**************************************************************************/
		/*!
		  \brief  rendering the face with color
		  \param c
		    color value
		*/
		/**************************************************************************/
	  void displayFaces(const glm::vec3& c);
	  /**************************************************************************/
		/*!
		  \brief  rendering the texuture
		*/
		/**************************************************************************/
	  void displayTexture(void);
	  /**************************************************************************/
		/*!
		  \brief  set sprite position
		  \param x
		    x coordinate
		  \param y
		    y coordinate
		*/
		/**************************************************************************/
	  void setTransform(float X, float Y);
	  /**************************************************************************/
		/*!
		  \brief  set sprite size
		  \param ratex
			width
		  \param ratey
			height
		*/
		/**************************************************************************/
	  void setScale(float ratex, float ratey);
	  /**************************************************************************/
		/*!
		  \brief  set sprite rotation
		  \param rate
			rotation rate
		*/
		/**************************************************************************/
	  void setRotate(float rate);
	  /**************************************************************************/
		/*!
		  \brief  set shader with id and put into member variablue
		  \param id
		    shader array order
		*/
		/**************************************************************************/
		void setShader(int id);
		/**************************************************************************/
		  /*!
			\brief  get shader program 
			\return GLuint
			  shader program value
		  */
		  /**************************************************************************/
	  GLuint getShader();

	  /**************************************************************************/
		/*!
		  \brief return sprite's alpha value
		  \return float
		    sprite's alpha value
		*/
		/**************************************************************************/
	  float getAlpha(); 
		/**************************************************************************/
		  /*!
			\brief  set alpha value in shader
		  */
		  /**************************************************************************/
		void setAlphaUniform();
		/**************************************************************************/
		  /*!
			\brief  set sprite's alpha value
			\param zeroToOne
			  alpha value
		  */
		  /**************************************************************************/
		void setAlpha(float zeroToOne);
		/**************************************************************************/
		/*!
				\brief  get shader program enum index
				\return GLuint
				shader program value
		*/
		/**************************************************************************/
		GLuint getShaderIndex()
		{
				return shaderIndex;
		}

  private:
    GLint utransform,
		  uview,
		  uprojection,
		  ualpha, 
		  ucolor;
	GLuint program,
		vertex_buffer,
		edge_buffer,
		face_buffer,
		texture_buffer,
		vertex_array,
		vertex_aarray;
	glm::mat3 transform, rscale, rotation;
    int mesh_edge_count,
        mesh_face_count;

		//Uniform values for shaders
		float image_alpha; //transparency, should range from 0 to 1
		int shaderIndex; //Index into the shader enum
};


#endif

