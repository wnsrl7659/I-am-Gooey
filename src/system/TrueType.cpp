/******************************************************************************/
/*!
\file   TrueType.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "TrueType.h"
#include "../graphics/Shader.h"
#include <iostream>
#include "localize.h"
#include "../component/button.h"
#include "../system/Window.h"
#include "../gameobject/gameobject.h"
#include "../physics/Transform.h"
#include "../graphics/Camera.h"
#include "../graphics/Sprite.h"
#include "../spaces/spacemanager.h"

static GLuint shader_font;
static GLuint VAO, VBO;
static std::map<GLuint, Character> Characters[4];

static GameObject* quitbox;
static Transform* quitboxtrs;
static Sprite* quitboxspr;
static GameObject* quityes;
static Transform* quityestrs;
static Sprite* quityesspr;
static GameObject* quitno;
static Transform* quitnotrs;
static Sprite* quitnospr;

int fonttype = 0;
int fontcount = 1;
bool fullscreentextswtich = false;
bool ffullscreentextswtich = false;
bool mutetext = false;

void LoadTTF(int id, const char* path)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 100);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		Characters[id].insert(std::pair<GLuint, Character>(c, character));
	}
	if (id == 0)
	{
		for (GLuint c = 44032; c < 55203; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters[id].insert(std::pair<GLuint, Character>(c, character));
		}
	}
	else if (id == 1)
	{
		for (GLuint c = 12288; c < 12544; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters[id].insert(std::pair<GLuint, Character>(c, character));
		}
		for (GLuint c = 19968; c < 40880; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters[id].insert(std::pair<GLuint, Character>(c, character));
		}
		for (GLuint c = 65280; c < 65520; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters[id].insert(std::pair<GLuint, Character>(c, character));
		}
	}
	else if (id == 2)
	{
		for (GLuint c = 1024; c < 1280; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)face->glyph->advance.x
			};
			Characters[id].insert(std::pair<GLuint, Character>(c, character));
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}

void TrueTypeInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader_font = ShaderCreate("./assets/shaders/Vert_Font.txt", "./assets/shaders/Frag_Font.txt");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1728), 0.0f, static_cast<GLfloat>(1120));
	glUseProgram(shader_font);
	glUniformMatrix4fv(glGetUniformLocation(shader_font, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	LoadTTF(0, "./assets/NotoSansKR-Regular.otf");
	LoadTTF(1, "./assets/NotoSansJP-Regular.otf");
	LoadTTF(2, "./assets/OpenSans-Regular.ttf");
	LoadTTF(3, "./assets/UncialAntiqua-Regular.ttf");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/******************************************************************************************/
	quitbox = new GameObject;

	quitboxtrs = quitbox->AddComponent<Transform>();
	quitboxtrs->Init();
	quitboxtrs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1]));
	quitboxspr = quitbox->AddComponent<Sprite>();

	quitboxspr->Init(Shader_BasicTexture);
	quitboxspr->SetTexture(Tex_Box);
	quitboxspr->SetSize(432, 350);
	quitboxspr->setPrior(6);
	//quitboxspr->setAlpha(0.0f);

	quityes = new GameObject;

	quityestrs = quityes->AddComponent<Transform>();
	quityestrs->Init();
	quityestrs->Translation(Vector2D(mainCam.GetPosition()[0] - 200.0f, mainCam.GetPosition()[1] - 200.0f));
	quityesspr = quityes->AddComponent<Sprite>();

	quityesspr->Init(Shader_BasicTexture);
	quityesspr->SetTexture(Tex_Box);
	quityesspr->SetSize(156, 64);
	quityesspr->setPrior(6);

	quitno = new GameObject;

	quitnotrs = quitno->AddComponent<Transform>();
	quitnotrs->Init();
	quitnotrs->Translation(Vector2D(mainCam.GetPosition()[0] + 200.0f, mainCam.GetPosition()[1] - 200.0f));
	quitnospr = quitno->AddComponent<Sprite>();

	quitnospr->Init(Shader_BasicTexture);
	quitnospr->SetTexture(Tex_Box);
	quitnospr->SetSize(156, 64);
	quitnospr->setPrior(6);
}


void TrueTypeUpdate(float dt)
{
	if (fullscreentextswtich == true)
	{
		std::wstring text1 = boost::locale::translate(L"On");
		std::wstring text2 = boost::locale::translate(L"Off");
		if (fullscreentext == true)
		{
			RenderText(text1, 1640.0f, 190.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
		else if (fullscreentext == false)
		{
			RenderText(text2, 1640.0f, 190.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}

		if (mutetext == true)
		{
			RenderText(text1, 1640.0f, 330.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
		else if (mutetext == false)
		{
			RenderText(text2, 1640.0f, 330.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
	}
	if (ffullscreentextswtich == true)
	{
		std::wstring text1 = boost::locale::translate(L"On");
		std::wstring text2 = boost::locale::translate(L"Off");
		if (fullscreentext == true)
		{
			RenderText(text1, 864.0f + 110.0f, 194.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
		else if (fullscreentext == false)
		{
			RenderText(text2, 864.0f + 110.0f, 194.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
		if (mutetext == true)
		{
			RenderText(text1, 864.0f + 110.0f, 330.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
		else if (mutetext == false)
		{
			RenderText(text2, 864.0f + 110.0f, 330.0f, 0.35f, glm::vec3(1.0, 1.0f, 0.0f), fonttype);
		}
	}
	if (quitconfirmflag == 1 || menuconfirmflag == 1)
	{
		quitboxtrs->Translation(Vector2D(mainCam.GetPosition()[0], mainCam.GetPosition()[1]));
		quityestrs->Translation(Vector2D(mainCam.GetPosition()[0] - 100.0f, mainCam.GetPosition()[1] - 100.0f));
		quitnotrs->Translation(Vector2D(mainCam.GetPosition()[0] + 100.0f, mainCam.GetPosition()[1] - 100.0f));
		quitbox->Update(dt);
		quityes->Update(dt);
		quitno->Update(dt);
		std::wstring text1 = boost::locale::translate(L"Are you sure?");
		std::wstring text2 = boost::locale::translate(L"Yes");
		std::wstring text3 = boost::locale::translate(L"No");

		RenderText(text1, (float)(710), (float)(1120 / 2) + 50.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f), fonttype);

		if (yesorno == 1)
		{
			quityesspr->SetTexture(Tex_Selectedbox);
			quitnospr->SetTexture(Tex_Box);
			RenderText(text2, (float)(725), (float)(1120 / 2) - 120.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f), fonttype);
			RenderText(text3, (float)(730) + 200.0f, (float)(1120 / 2) - 120.0f, 0.5f, glm::vec3(0.0, 0.0f, 0.0f), fonttype);
		}
		else
		{
			quityesspr->SetTexture(Tex_Box);
			quitnospr->SetTexture(Tex_Selectedbox);
			RenderText(text2, (float)(725), (float)(1120 / 2) - 120.0f, 0.5f, glm::vec3(0.0, 0.0f, 0.0f), fonttype);
			RenderText(text3, (float)(730) + 200.0f, (float)(1120 / 2) - 120.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f), fonttype);
		}
	}
}

void TrueTypeShutdown()
{
	for (int i = 0; i < 3; i++)
	{
		Characters[i].clear();
	}
	delete quitbox;
	delete quityes;
	delete quitno;
}

void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int id, int mode)
{
	// Activate corresponding render state	
	glUseProgram(shader_font);

	glUniform3f(glGetUniformLocation(shader_font, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	GLfloat sumw = 0.0f;
	if (mode == 1)
	{
		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[id][*c];
			sumw += (ch.Advance >> 6) * scale;
		}
		x = x - sumw;
	}
	else if (mode == 2)
	{
		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[id][*c];
			sumw += (ch.Advance >> 6) * scale;
		}
		x = x - sumw / 2;
	}


	// Iterate through all characters
	std::wstring::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[id][*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
