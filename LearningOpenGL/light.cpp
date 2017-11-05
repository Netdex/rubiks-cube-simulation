#include "stdafx.h"
#include "light.h"

light::light(glm::vec3 l, glm::vec3 c) : location(l), color(c)
{
	std::vector<vertex> vertices = {
		vertex(glm::vec3(+0.1f,+0.1f,+0.1f), c),
		vertex(glm::vec3(-0.1f,+0.1f,+0.1f), c),
		vertex(glm::vec3(+0.1f,-0.1f,+0.1f), c),
		vertex(glm::vec3(+0.1f,+0.1f,-0.1f), c),
		vertex(glm::vec3(-0.1f,-0.1f,+0.1f), c),
		vertex(glm::vec3(+0.1f,-0.1f,-0.1f), c),
		vertex(glm::vec3(-0.1f,+0.1f,-0.1f), c),
		vertex(glm::vec3(-0.1f,-0.1f,-0.1f), c),
	};
	std::vector<GLuint> elements = {
		0,1,2,
		1,2,3,
		2,3,4,
		3,4,5,
		4,5,6,
		5,6,7,
		2,1,0,
		3,2,1,
		4,3,2,
		5,4,3,
		6,5,4,
		7,6,5,
	};
	light_mesh = new mesh(vertices, elements);
	light_mesh->update();
}

light::~light()
{
	delete light_mesh;
}

void light::draw(GLuint shader)
{
	light_mesh->transform = translate(glm::mat4(), location);

	GLuint uniLightPosition = glGetUniformLocation(shader, "light_position");
	glUniform3fv(uniLightPosition, 1, value_ptr(location));

	GLuint uniLightColor = glGetUniformLocation(shader, "light_color");
	glUniform3fv(uniLightColor, 1, value_ptr(color));
	light_mesh->draw(shader);
}
