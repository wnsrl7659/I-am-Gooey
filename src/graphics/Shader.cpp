/******************************************************************************/
/*!
\file   Shader.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for handling construction of shaders handles

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <iostream>
#include "../system/FileReading.h" /* load char* (shader source) from a file */
#include "Shader.h"
#include "../settings/settings.h"
#include "../trace/trace.h"

GLuint ShaderArray[Shader_Last]; //!< Array of shader IDs

Location location[Shader_Last]; //!< Array of uniform locations for each shader

/*****************************************************************/
/*!
		\brief
				Reads a shader from a file
		\param filename
				the filename, of course
		\return 
				the string (char*)
*/
/*****************************************************************/
static char* ShaderReadFromFile(const char * filename)
{
		return StringReadFromFile(filename);
}

/*****************************************************************/
/*!
		\brief
				Compiles a single shader, fragment or vertex, given a
				filename.
		\param shaderType
				Fragment or Vertex.
		\param filename
				the filename to get the shader code from
		\return 
				the ID for the program
*/
/*****************************************************************/
static unsigned int ShaderCompile(GLenum shaderType, const char* filename)
{
		//Get the shader code from a file
		const char* shaderCode = ShaderReadFromFile(filename);
		if (strcmp(shaderCode, "error") == 0)
		{
				Trace::Message("Shader has error reading file: "), Trace::Message(filename);
				std::cout << "Shader has error reading file: " << filename << std::endl;
		}
		else if(Settings::debug)//if read successfully in debug, print out the string
				std::cout << shaderCode << std::endl;

		//create the shader from the code
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		//delete the now pointless code
		delete[] shaderCode;

		//ERROR HANDLING
		GLint errorResult;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &errorResult);
		if (errorResult == GL_FALSE)
		{
				/**/
				GLint size;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
				GLchar *message = new GLchar[size + 1];
				glGetShaderInfoLog(shader, size, &size, message);

				std::cout << filename << ": " << std::endl << message << std::endl;
				Trace::Message(message);

				delete[] message;
				glDeleteShader(shader);
				return 0;
				/**/
		}

		return shader;
}

/*****************************************************************/
/*!
		\brief
				Creates a program from two shader files, a vertex, and a 
				fragment.
		\param vertexFile
				filename of the vertex shader
		\param fragmentFile
				filename of the fragment shader
		\return
				ID of the shader program.
*/
/*****************************************************************/
GLuint ShaderCreate(const char* vertexFile, const char* fragmentFile)
{
		GLuint program = glCreateProgram();
		GLuint vertShader = ShaderCompile(GL_VERTEX_SHADER, vertexFile);
		GLuint fragShader = ShaderCompile(GL_FRAGMENT_SHADER, fragmentFile);

		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);
		glValidateProgram(program);

		//in the program now, ok to delete
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		//std::cout << "Created shader with[" << vertexFile << "]and[" << fragmentFile << "]" << std::endl;

		return program;
}

/*****************************************************************/
/*!
		\brief
				Initializes the global shader array
*/
/*****************************************************************/
void ShaderInit()
{
		ShaderArray[Shader_BasicColor] = ShaderCreate("./assets/shaders/Vert_Default.txt", "./assets/shaders/Frag_Col.txt");
		ShaderArray[Shader_BasicTexture] = ShaderCreate("./assets/shaders/Vert_Default.txt", "./assets/shaders/Frag_Tex.txt");
		ShaderArray[Shader_BasicTextureColorBlend] = ShaderCreate("./assets/shaders/Vert_Animated.txt", "./assets/shaders/Frag_TexColBlend.txt");
		ShaderArray[Shader_AnimatedTexture] = ShaderCreate("./assets/shaders/Vert_Animated.txt", "./assets/shaders/Frag_Tex.txt");
		ShaderArray[Shader_SolidColor] = ShaderCreate("./assets/shaders/Vert_Solid.txt", "./assets/shaders/Frag_Col.txt");
		ShaderArray[Shader_TileLighting] = ShaderCreate("./assets/shaders/Vert_TileLight.txt", "./assets/shaders/Frag_TileLight.txt");
		ShaderArray[Shader_Multiply] = ShaderCreate("./assets/shaders/Vert_Default.txt", "./assets/shaders/Frag_TexColBlend.txt");
		SetAddress();
}

/*****************************************************************/
/*!
		\brief
				Frees all the shader programs
*/
/*****************************************************************/
void ShaderShutdown()
{
		for (int i = Shader_StartOfList + 1; i < Shader_Last; i++)
				glDeleteProgram(ShaderArray[i]);
}

/*****************************************************************/
/*!
		\brief
				Sets the uniform addresses for each program
*/
/*****************************************************************/
void SetAddress()
{
	for(int i = 0; i < Shader_Last; i ++)
	{
		location[i].uColor = glGetUniformLocation(ShaderArray[i], "uColor");
		location[i].utransform = glGetUniformLocation(ShaderArray[i], "transform");
		location[i].uview = glGetUniformLocation(ShaderArray[i], "view");
		location[i].uprojection = glGetUniformLocation(ShaderArray[i], "projection");
		location[i].aIndex = glGetUniformLocation(ShaderArray[i], "aIndex");
		location[i].aRow = glGetUniformLocation(ShaderArray[i], "aRows");
		location[i].aColums = glGetUniformLocation(ShaderArray[i], "aColums");
		location[i].image_alpha = glGetUniformLocation(ShaderArray[i], "image_alpha");
		location[i].r = glGetUniformLocation(ShaderArray[i], "ured");
		location[i].g = glGetUniformLocation(ShaderArray[i], "ugreen");
		location[i].b = glGetUniformLocation(ShaderArray[i], "ublue");

		location[i].LightData = glGetUniformLocation(ShaderArray[i], "LightData");
		location[i].LightColor = glGetUniformLocation(ShaderArray[i], "LightColor");
		location[i].isWall = glGetUniformLocation(ShaderArray[i], "isWall");
		location[i].TileZ = glGetUniformLocation(ShaderArray[i], "TileZ");
		location[i].validLights = glGetUniformLocation(ShaderArray[i], "validLights");
		location[i].texture_normal = glGetUniformLocation(ShaderArray[i], "texture_normal");
		location[i].texture_color = glGetUniformLocation(ShaderArray[i], "texture_color");
		//location[i].real_color = glGetUniformLocation(ShaderArray[i], "real_color");
		
	}
}

/*****************************************************************/
/*!
		\brief
				Gets the index from the Shader list enum, given a 
				program handler int.
		\return
				Shader list index. -1 on error.
*/
/*****************************************************************/
int ShaderGetIndexFromHandler(int programHandler)
{
		int result = 0;
		for (int i = 0; i < Shader_Last; i++)
		{
				if (programHandler == ShaderArray[i])
				{
						return i;
				}
		}
		return -1; //no matches
}

/*****************************************************************/
/*!
		\brief
				Given a program id, finds which index it belongs to
				and returns a string name for the program. Useful for
				debugging.
		\return
				The string
*/
/*****************************************************************/
std::string ShaderGetNameFromInt(int program)
{
		std::string result = "Unspecified";
		int i = ShaderGetIndexFromHandler(program);

		switch (i)
		{
		case Shader_AnimatedTexture:
				result = "Animated Texture";
				break;
		case Shader_BasicColor:
				result = "Basic Color";
				break;
		case Shader_BasicTexture:
				result = "Basic Texture";
				break;
		case Shader_SolidColor:
				result = "Solid Color";
				break;
		case Shader_TileLighting:
				result = "Tile Lighting";
				break;
		}

		std::stringstream addOn;
		addOn << " uint:[" << program << "]";
						
		result += addOn.str();
		
		return result;
}
