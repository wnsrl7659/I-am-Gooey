/******************************************************************************/
/*!
\file   TextureManager.h
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM200
\brief
TextureManager class, which handle buffer of every Texture

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/glew.h> //Include first
#include <GL/gl.h> 
#include <GLFW/glfw3.h>

enum TextureList
{
	Tex_OliveSpriteSheet = 0,
	Tex_tile_set,
	Tex_numberfont,
	Tex_title,
	Tex_pause,
	Tex_play,
	Tex_options,
	Tex_credits,
	Tex_quit,
	Tex_resume,
	Tex_pause2,
	Tex_titlecredits,
	Tex_teamcredits,
	Tex_glfwcredits,
	Tex_splash,
	Tex_menu,
	Tex_particle,
	Tex_winScreen,
	Tex_select,
	Tex_Normal,
	Tex_Fade,
	Tex_Logo,
	Tex_Tower,
	Tex_MenuBG,
	Tex_LeftRightFade,
	Tex_TreesFG,
	Tex_Box,
	Tex_Selectedbox,
	Tex_TreesBG,
	Tex_MenuGround,
	Tex_TutorialBowl,
	Tex_TutorialHint,
	Tex_TutorialLoop,
	Tex_TutorialMove,
	Tex_specialcredits,
	Tex_instcredits,
	Tex_execcredits,
	Tex_EngineScreen,
	Tex_WinBG,
	Tex_WinTower,

	Tex_Count,
};


typedef struct Texture
{
	unsigned char * data = nullptr;
	int width = 0;
	int height = 0;
	int nrChannels = 0;
}Texture;

/**************************************************************************/
  /*!
	\brief  Initializer
  */
  /**************************************************************************/
void TextureManagerInit();

/**************************************************************************/
  /*!
	\brief  Update
	\param dt
		time value
  */
  /**************************************************************************/
void TextureManagerUpdate(float dt);

/**************************************************************************/
  /*!
	\brief  common Shutdown function(desturcter)
  */
  /**************************************************************************/
void TextureManagerShutdown();


/**************************************************************************/
  /*!
	\brief  Create the texture buffer
  */
  /**************************************************************************/
void TextureBufferInit();


/**************************************************************************/
  /*!
	\brief  return specific texture buffer depend on id
	\param id
		texture array's order
	\return GLuint
		one of texture array depend on id
  */
  /**************************************************************************/
GLuint GetTexture(const int id);



#endif

