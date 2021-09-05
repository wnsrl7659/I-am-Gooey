/******************************************************************************/
/*!
\file   FontRender.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "FontRender.h"
#include "../gameobject/gameobject.h"
#include "../spaces/spacemanager.h"
#include "SpriteOrder.h"


FontRender::FontRender()
{
	x = 0;
	y = 0;
	scale = 1.0f;
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
}
FontRender::~FontRender()
{

}

void FontRender::SetPos(float rx, float ry)
{
	x = rx;
	y = ry;
}
void FontRender::SetSize(float rscale)
{
	scale = rscale;
}

void FontRender::SetText(std::wstring rtext)
{
	text = rtext;
}

void FontRender::SetColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void FontRender::Update(float dt)
{
	if (isTranslated == true)
	{
		translated = boost::locale::translate(text);
	}
	else
	{
		translated = text;
	}
}
void FontRender::Draw()const
{
	if ((owner->space) && (owner->space->Active()))
	{
		if (type == 999)
		{
			AddFont(translated, x, y, scale, color, fonttype, mode);
		}
		else
		{
			AddFont(translated, x, y, scale, color, type, mode);

		}
		//RenderText(translated, x, y, scale, color, fonttype);
	}
}