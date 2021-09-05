/******************************************************************************/
/*!
\file   TrueType.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum TTFfont
{
	font_KOR = 0,
	font_JAP,
	font_RUS
};

void TrueTypeInit();

void TrueTypeUpdate(float dt);

void TrueTypeShutdown();

void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int id, int mode = 0);

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

extern int fonttype;
extern int fontcount;
extern bool fullscreentextswtich;
extern bool ffullscreentextswtich;
extern bool mutetext;