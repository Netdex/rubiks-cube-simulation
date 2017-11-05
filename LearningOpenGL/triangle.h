#pragma once
#include "vertex.h"

class triangle
{
public:
	vertex vertices[3];

	triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color);
	~triangle();

};

