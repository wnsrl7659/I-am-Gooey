/******************************************************************************/
/*!
\file   SpriteOrder.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "SpriteOrder.h"
#include "Render.h"
#include "../system/TrueType.h"

static Render* render;

static std::map<GLuint, NeedforSprite> Sprites[7];
static std::map<GLuint, NeedforFont> Fonts;
static std::map<GLuint, NeedforLine> Lines;

static NeedforSprite lastused;

static int count[7];

Particle* pArray[1000] = {};
int pCount = 0;

void SpriteOrderInit()
{
	render = new Render(0);

	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	count[3] = 0;
	count[4] = 0;
	count[5] = 0;
	count[6] = 0;

	NeedforSprite Sprite = {
		999, 999, 999, glm::vec2(0,0), glm::vec2(0,0), 0, 999, 0, 0, 0, 0 };
	lastused = Sprite;
}

void SpriteOrderUpdate(float dt)
{
	dt = 0.0f;

	for (int i = 0; i < 7; i++)
	{
		if (i == 4)
		{
			for (int j = 0; j < count[i]; j++)
			{
				RenderText(Fonts[j].text, Fonts[j].x, Fonts[j].y, Fonts[j].scale, Fonts[j].color, Fonts[j].id, Fonts[j].mode);
			}
		}
		else if (i == 5)
		{
			for (int j = 0; j < count[i]; j++)
			{
				render->setShader(Lines[j].shaderid);

				render->loadMesh(Lines[j].meshid);

				render->setTransform(Lines[j].pos.x, Lines[j].pos.y);
				render->setScale(Lines[j].scale.x, Lines[j].scale.y);
				render->setRotate(Lines[j].rotate);

				render->setModelToWorld(mainCam);

				glUseProgram(render->getShader());

				render->displayEdges(Lines[j].color);
			}
		}
		else
		{
			for (int j = 0; j < count[i]; j++)
			{
				if (lastused.shaderid != Sprites[i][j].shaderid)
				{
					NeedforSprite Sprite = {
						999, 999, 999, glm::vec2(0,0), glm::vec2(0,0), 0, 999, 0, 0, 0, 0 };
					lastused = Sprite;

					lastused.shaderid = Sprites[i][j].shaderid;
					render->setShader(Sprites[i][j].shaderid);
				}

				if (lastused.meshid != Sprites[i][j].meshid)
				{
					lastused.meshid = Sprites[i][j].meshid;
					render->loadMesh(Sprites[i][j].meshid);
				}

				if (lastused.textureid != Sprites[i][j].textureid)
				{
					lastused.textureid = Sprites[i][j].textureid;
					render->loadTexture(Sprites[i][j].textureid);
				}

				//if (lastused.pos.x != Sprites[i][j].pos.x || lastused.pos.y != Sprites[i][j].pos.y)
				//{
				//	lastused.pos = Sprites[i][j].pos;
					render->setTransform(Sprites[i][j].pos.x, Sprites[i][j].pos.y);
				//}

				//if (lastused.scale.x != Sprites[i][j].scale.x || lastused.scale.y != Sprites[i][j].scale.y)
				//{
				//	lastused.scale = Sprites[i][j].scale;
					render->setScale(Sprites[i][j].scale.x, Sprites[i][j].scale.y);
				//}

				//if (lastused.rotate != Sprites[i][j].rotate)
				//{
				//	lastused.rotate = Sprites[i][j].rotate;
					render->setRotate(Sprites[i][j].rotate);
				//}

				render->setModelToWorld(mainCam);
				if (lastused.alpha != Sprites[i][j].alpha)
				{
					lastused.alpha = Sprites[i][j].alpha;
					render->setAlpha(Sprites[i][j].alpha);
					render->setAlphaUniform();
				}

				glUseProgram(render->getShader());
				if (Sprites[i][j].anim)
				{
					if (lastused.anim != Sprites[i][j].anim)
					{
						lastused.anim = Sprites[i][j].anim;
						Sprites[i][j].anim->setUniforms();
					}
				}
				if (Sprites[i][j].LR)
				{
					if (lastused.LR != Sprites[i][j].LR)
					{
						lastused.LR = Sprites[i][j].LR;
						Sprites[i][j].LR->setUniforms();
					}
				}
				if (Sprites[i][j].color)
				{
					if (lastused.color != Sprites[i][j].color)
					{
						lastused.color = Sprites[i][j].color;
						Sprites[i][j].color->setUniforms();
					}
				}

				render->displayTexture();
			}

		}
		if (i == 4)
		{
			Fonts.clear();
		}
		else if (i == 5)
		{
			Lines.clear();
			NeedforSprite Sprite = {
				999, 999, 999, glm::vec2(0,0), glm::vec2(0,0), 0, 999, 0, 0, 0, 0 };
			lastused = Sprite;
		}
		else
		{
			Sprites[i].clear();
		}
	}
	/*delete map*/
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	count[3] = 0;
	count[4] = 0;
	count[5] = 0;
	count[6] = 0;
	
	/*delete particle*/
	for (int i = 0; i < pCount; i++)
	{
		delete[] pArray[i];
	}
	pCount = 0;
}
void SpriteOrderShutdown()
{
	render->unloadMesh();
	delete render;
	for (int i = 0; i < 7; i++)
	{
		Sprites[i].clear();
	}
	Fonts.clear();
	Lines.clear();
}
void AddSprite(
	int				prior,
	GLuint			shaderid,
	int				meshid,
	int				textureid,
	glm::vec2		pos,
	glm::vec2		scale,
	float			rotate,
	float			alpha,
	Animation*		anim,
	LightReciever*	LR,
	Color*			color,
	GameObject*     owner)
{
	NeedforSprite Sprite = {
		shaderid, meshid, textureid, 
		pos, scale, rotate, alpha, anim, LR, color, owner
	};

	Sprites[prior].insert(std::pair<GLuint, NeedforSprite>(count[prior], Sprite));
	count[prior]++;
}

void AddLine(GLuint	shaderid, int meshid, glm::vec2 pos, glm::vec2 scale, float rotate, glm::vec3 color)
{
	NeedforLine Line = { shaderid, meshid, pos, scale, rotate, color };

	Lines.insert(std::pair<GLuint, NeedforLine>(count[5], Line));
	count[5]++;
}

void AddFont(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, int id, int mode)
{
	NeedforFont Font = {text, x, y, scale, color, id, mode};
	Fonts.insert(std::pair<GLuint, NeedforFont>(count[4], Font));
	count[4]++;
}
