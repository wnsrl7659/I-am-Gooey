/******************************************************************************/
/*!
\file   FontRender.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef FONTRENDER_H
#define FONTRENDER_H

#include "../component/component.h"
#include "../system/TrueType.h"
#include "../system/localize.h"


class FontRender : public Component<FontRender>
{
public:
	COMPONENT_NAME(FontRender);

	FontRender();
	~FontRender();
	void SetPos(float rx, float ry);
	void SetSize(float rscale);
	void SetText(std::wstring rtext);
	void SetColor(float r, float g, float b);
	void Update(float dt);
	void Draw()const;
	void setMode(int temp)
	{
		mode = temp;
	}
	void setType(int ttype)
	{
		type = ttype;
	}
	void setTranslation(bool temp)
	{
		isTranslated = temp;
	}

private:
	float x; 
	float y;
	float scale;
	std::wstring text;
	std::wstring translated;
	glm::vec3 color;
	int mode = 0;
	int type = 999;
	int isTranslated = true;
};

#endif