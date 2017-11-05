#include "stdafx.h"
#include "axis.h"
#include "vertex.h"


axis::axis(glm::vec3 o, glm::vec3 x, glm::vec3 y, glm::vec3 z) : o(o), x(x), y(y), z(z)
{
	vertices = std::vector<vertex>{ 
		vertex(glm::vec3(), glm::vec3(1, 0, 0)),
		vertex(x, glm::vec3(1, 0, 0)),
		vertex(glm::vec3(), glm::vec3(0, 1, 0)),
		vertex(y, glm::vec3(0, 1, 0)),
		vertex(glm::vec3(), glm::vec3(0, 0, 1)),
		vertex(z, glm::vec3(0, 0, 1)) 
	};
	setup();
}

axis::~axis()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void axis::draw(GLuint shader) const
{
	GLuint uniModel = glGetUniformLocation(shader, "model");
	GLuint uniLighting = glGetUniformLocation(shader, "do_lighting");
	glm::mat4 transform = glm::translate(glm::mat4(), o);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform1i(uniLighting, GL_FALSE);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(NULL);
}

void axis::setup()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof vertex, (void*)&vertices[0], GL_STATIC_DRAW);

	vertex::set_vertex_attrib();

	glBindVertexArray(NULL);
}
