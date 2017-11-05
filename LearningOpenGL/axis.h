#pragma once
#include "vertex.h"

class axis
{
public:
	glm::vec3 o;
	glm::vec3 x, y, z;

	axis(glm::vec3 o, glm::vec3 x, glm::vec3 y, glm::vec3 z);
	virtual ~axis();
	void draw(GLuint shader) const;
private:
	std::vector<vertex> vertices;
	GLuint VAO = NULL, VBO = NULL;
	void setup();
};

