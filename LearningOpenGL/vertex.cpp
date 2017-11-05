#include "stdafx.h"
#include "vertex.h"


vertex::vertex() : vertex(glm::vec3(), glm::vec3(), glm::vec3())
{
}

vertex::vertex(glm::vec3 position, glm::vec3 color) : vertex(position, glm::vec3(), color)
{
}

vertex::vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color)
{
	this->position = position;
	this->normal = normal;
	this->color = color;
}


vertex::~vertex()
{
}

void vertex::set_vertex_attrib()
{
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof vertex, (void*)offsetof(vertex, position));
	// vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof vertex, (void*)offsetof(vertex, normal));
	// vertex color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof vertex, (void*)offsetof(vertex, color));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}


