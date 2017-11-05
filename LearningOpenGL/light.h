#pragma once
#include "mesh.h"

class light
{
public:
	light(glm::vec3 location, glm::vec3 color);
	virtual ~light();
	void draw(GLuint shader);

	glm::vec3 location;
	glm::vec3 color;
	mesh *light_mesh;
private:
	
};

