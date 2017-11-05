#include "stdafx.h"
#include "mesh.h"
#include "triangle.h"
#include <glad/glad.h>

mesh::mesh(std::vector<vertex> &vertices, std::vector<GLuint> &elements)
{
	this->vertices = vertices;
	this->elements = elements;
	this->use_elements = true;
	setup();
}

mesh::mesh(std::vector<vertex> &vertices)
{
	this->vertices = vertices;
	setup();
}

mesh::mesh(std::vector<triangle>& triangles)
{
	this->add(triangles);
	setup();
}

mesh::mesh(const mesh& other)
	: vertices(other.vertices),
	elements(other.elements),
	transform(other.transform),
	use_elements(other.use_elements),
	VAO(other.VAO),
	VBO(other.VBO),
	EBO(other.EBO)
{
}

mesh::mesh(mesh&& other) noexcept
	: vertices(std::move(other.vertices)),
	elements(std::move(other.elements)),
	transform(std::move(other.transform)),
	use_elements(other.use_elements),
	VAO(other.VAO),
	VBO(other.VBO),
	EBO(other.EBO)
{
}

mesh& mesh::operator=(const mesh& other)
{
	if (this == &other)
		return *this;
	vertices = other.vertices;
	elements = other.elements;
	transform = other.transform;
	use_elements = other.use_elements;
	VAO = other.VAO;
	VBO = other.VBO;
	EBO = other.EBO;
	return *this;
}
mesh& mesh::operator=(mesh&& other) noexcept
{
	if (this == &other)
		return *this;
	vertices = std::move(other.vertices);
	elements = std::move(other.elements);
	transform = std::move(other.transform);
	use_elements = other.use_elements;
	VAO = other.VAO;
	VBO = other.VBO;
	EBO = other.EBO;
	return *this;
}

mesh::~mesh()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void mesh::draw(GLuint shader) const
{
	GLuint uniModel = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);

	if (use_elements) {
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);			// this is required because my intel graphics card
		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
	}

	else {
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	glBindVertexArray(NULL);
}

void mesh::update()
{
	// HACK: untested
	assert(VBO && EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof vertex, (void*)&vertices[0], GL_STATIC_DRAW);

	if (use_elements) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof GLuint, (void*)&elements[0], GL_STATIC_DRAW);
	}
}

void mesh::add(triangle triangle) // TODO make this abstract
{
	this->vertices.insert(vertices.end(), triangle.vertices, triangle.vertices + 3);
}

void mesh::add(std::vector<triangle> &triangles)
{
	for (auto i = triangles.begin(); i != triangles.end(); ++i)
		this->vertices.insert(vertices.end(), i->vertices, i->vertices + 3);
}

void mesh::setup()
{
	// HACK: untested
	assert(!VAO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof vertex, (void*)&vertices[0], GL_STATIC_DRAW);

	if (use_elements) {
		// setup element buffer for elements
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof GLuint, (void*)&elements[0], GL_STATIC_DRAW);
	}

	vertex::set_vertex_attrib();
	glBindVertexArray(NULL);
}


