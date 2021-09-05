/******************************************************************************/
/*!
\file   TextureManager.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
TextureManager class, which handle buffer of every Texture

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "TextureManager.h"
#include "../trace/trace.h"
#include "../settings/settings.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static Texture* texture = nullptr;

static int count = 0;

static char** path;

static GLuint* texture_buffer;

void TextureManagerInit()
{
	/*strcpy_s(path[0], sizeof(char) * 100, "./assets/resources/blarg.png");
	strcpy_s(path[1], sizeof(char) * 100, "./assets/resources/tile_set.png");
	strcpy_s(path[2], sizeof(char) * 100, "./assets/resources/oak_pure_1_v2.png");
	strcpy_s(path[3], sizeof(char) * 100, "./assets/resources/oak_pure_2.png");
	strcpy_s(path[4], sizeof(char) * 100, "./assets/resources/oak_pure_3.png");
	strcpy_s(path[5], sizeof(char) * 100, "./assets/resources/oak_pure_4.png");*/
	/******************************************************************/

	std::ifstream f("./assets/resources/path.txt", std::ios::in);
	if (!f)
	{
		Trace::Message("Settings file failed to open!");
		return;
	}
	std::vector<std::string> lines; // stores each line of the file
	std::string buffer; //buffer string

	std::getline(f, buffer);
	while (f)
	{
		count++;
		lines.push_back(buffer);
		std::getline(f, buffer);
	}

	assert(count == Tex_Count);

	texture = (Texture*)calloc(count, sizeof(Texture));
	path = (char **)calloc(count, sizeof(char*));
	texture_buffer = (GLuint*)calloc(count, sizeof(GLuint));

	for (int i = 0; i < count; i++)
	{
		path[i] = (char*)calloc(100, sizeof(char));
	}

	for (int i = 0; i < count; i++)
	{
		strcpy_s(path[i], sizeof(char) * 100, lines.at(i).c_str());
	}
	

	/******************************************************************/
	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < count; i++)
	{
		texture[i].data = stbi_load(path[i], &texture[i].width, &texture[i].height, &texture[i].nrChannels, STBI_rgb_alpha);
		std::stringstream ss;
		ss << "LOADING TEXTURE: " << path[i];
		Trace::Message(ss.str());
		if (!texture[i].data)
		{
			//debug
			std::ostringstream os;
			os << "FAILED TO LOAD TEXTURE: " << path[i];
			Trace::Message(os.str().c_str());
		}
	}

	TextureBufferInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextureManagerShutdown()
{
	for (int i = 0; i < count; i++)
	{
		free(path[i]);
	}
	free(path);
	
	for (int i = 0; i < count; i++)
	{
		stbi_image_free(texture[i].data);
	}
	free(texture);
	free(texture_buffer);
}

GLuint GetTexture(const int id)
{
	if (texture[id].data)
	{
		if (texture_buffer[id])
		{
			return texture_buffer[id];
		}
	}
	return 0;
}

void TextureBufferInit()
{
	for (int i = 0; i < count; i++)
	{
		Texture* temp = &texture[i];
		//GLuint texture;
		glGenTextures(1, &texture_buffer[i]);
		glBindTexture(GL_TEXTURE_2D, texture_buffer[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											   // set the texture wrapping parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//// load image, create texture and generate mipmaps
		//int width, height, nrChannels;

		//stbi_set_flip_vertically_on_load(true);
		//unsigned char *data = stbi_load(temp->path, &width, &height, &nrChannels, STBI_rgb_alpha);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, temp->width, temp->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->data);
		GLenum result = glGetError();
		if (result != GLEW_OK)
		{
			std::stringstream ss;
			ss << "Error on " << __LINE__ << " " << __FILE__ << " " << __FUNCTION__ << " Code: " << result;
			Trace::Message(ss.str());
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}