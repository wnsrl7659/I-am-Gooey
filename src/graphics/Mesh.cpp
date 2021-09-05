/******************************************************************************/
/*!
\file   Mesh.cpp
\author Junki Kim
\par    email: junki.kim\@digipen.edu
\par    Course: GAM250

\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Mesh.h"

Mesh::Mesh(MeshList id)
{

	if (id == Mesh_Square)
	{
		vCount = 4;
		fCount = 2;
		eCount = 4;
		cen = glm::vec2(0, 0);
		dim = glm::vec2(2, 2);

		vertices = new glm::vec2[vCount];
		faces = new glm::vec3[fCount];
		edges = new glm::vec2[eCount];
		colors = new glm::vec3[vCount];
		texts = new glm::vec2[vCount];

		vertices[0] = glm::vec2(-0.5, -0.5);
		vertices[1] = glm::vec2(0.5, -0.5);
		vertices[2] = glm::vec2(0.5, 0.5);
		vertices[3] = glm::vec2(-0.5, 0.5);

		faces[0] = glm::vec3(0, 1, 2);
		faces[1] = glm::vec3(0, 2, 3);

		edges[0] = glm::vec2(0, 1);
		edges[1] = glm::vec2(1, 2);
		edges[2] = glm::vec2(2, 3);
		edges[3] = glm::vec2(3, 0);

		colors[0] = glm::vec3(1, 1, 1);
		colors[1] = glm::vec3(1, 1, 1);
		colors[2] = glm::vec3(1, 1, 1);
		colors[3] = glm::vec3(1, 1, 1);

		texts[0] = glm::vec2(0, 0);
		texts[1] = glm::vec2(1, 0);
		texts[2] = glm::vec2(1, 1);
		texts[3] = glm::vec2(0, 1);
	}
	else if (id == Mesh_Line)
	{
		vCount = 2;
		fCount = 1;
		eCount = 1;
		cen = glm::vec2(0, 0);
		dim = glm::vec2(2, 2);

		vertices = new glm::vec2[vCount];
		faces = new glm::vec3[fCount];
		edges = new glm::vec2[eCount];
		colors = new glm::vec3[vCount];
		texts = new glm::vec2[vCount];

		vertices[0] = glm::vec2(0.0, 0.0);
		vertices[1] = glm::vec2(0.5, 0);

		faces[0] = glm::vec3(0, 1, 0);

		edges[0] = glm::vec2(0, 1);

		colors[0] = glm::vec3(1, 1, 1);
		colors[1] = glm::vec3(1, 1, 1);
	}
	else
	{

	}
}
Mesh::~Mesh()
{
	delete[] vertices;
	delete[] faces;
	delete[] edges;
	delete[] colors;
	delete[] texts;
}
int Mesh::vertexCount(void)
{
	return vCount;
}
const glm::vec2* Mesh::vertexArray(void)
{
	return vertices;
}
glm::vec2 Mesh::dimensions(void)
{
	return dim;
}

glm::vec2 Mesh::center(void)
{
	return cen;
}
int Mesh::faceCount(void)
{
	return fCount;
}
const glm::vec3* Mesh::faceArray(void)
{
	return faces;
}
int Mesh::edgeCount(void)
{
	return eCount;
}
const glm::vec2* Mesh::edgeArray(void)
{
	return edges;
}
const glm::vec3* Mesh::colorArray(void)
{
	return colors;
}
const glm::vec2* Mesh::textArray(void)
{
	return texts;
}