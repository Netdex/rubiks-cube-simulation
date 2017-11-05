#include "stdafx.h"
#include "triangle.h"


triangle::triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c -a;
	glm::vec3 normal = normalize(cross(ab, ac));

	vertices[0] = vertex(a, normal, color);
	vertices[1] = vertex(b, normal, color);
	vertices[2] = vertex(c, normal, color);
}


triangle::~triangle()
{
}

