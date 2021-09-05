/******************************************************************************/
/*!
\file   Shader.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for handling construction of shaders handles

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <GL/glew.h>

enum ShaderList  //!< List of all shader programs to make
{
		Shader_BasicColor = 0,
		Shader_BasicTexture,
		Shader_BasicTextureColorBlend,
		Shader_AnimatedTexture,
		Shader_SolidColor,
		Shader_TileLighting,
		Shader_Multiply,

		Shader_Last,
		Shader_StartOfList = -1,
};

typedef struct Location {

	GLint uColor;      //!< transform matrix uniform
	GLint utransform;  //!< transform matrix uniform
	GLint uview;       //!< view matrix
	GLint uprojection; //!< projection matrix
	GLint aIndex;      //!< animation index
	GLint aRow;        //!< row  count for dividing the texture
	GLint aColums;     //!< colum count for dividing the texture
	GLint image_alpha; //!< transparency of the image
	GLint r;					 //!< Solid color passed into fragment shader
	GLint g;					 //!< Solid color passed into fragment shader
	GLint b;					 //!< Solid color passed into fragment shader
	GLint LightData;   //!< Light position arrays
	GLint LightColor;  //!< Light Color data
	GLint isWall;      //!< If a light reciever is a wall
	GLint TileZ;       //!< Light recievers Z value
	GLint validLights; //!< How many Valid lights there are
	GLint texture_normal; //!< Normal texture map
	GLint texture_color;  //!< Color texture map
	//GLint real_color;  //!< The real color for the color map
}Location;

extern GLuint ShaderArray[Shader_Last];  //!< Global storage for all program handles

extern Location location[Shader_Last];  //!< Global storage for all uniform locations (for each program)

//Gets index representation from the shader handler
int ShaderGetIndexFromHandler(int programHandler);

//Gets string representation from the handle address
std::string ShaderGetNameFromInt(int shaderAddress);

//Reads a shader from a filename
static char* ShaderReadFromFile(const char * filename);

//Compiles a shader
static unsigned int ShaderCompile(GLenum shaderType, const char* shaderCode);

//Creates a single shader program with a vertex and fragment file
GLuint ShaderCreate(const char* vertex, const char* fragment);

//Compiles all shaders
void ShaderInit();

//Deletes all shader programs
void ShaderShutdown();

//Sets the uniform location addresses
void SetAddress();

#endif 
