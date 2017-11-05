#pragma once
#include "vertex.h"

class triangle;

class mesh
{
public:
	std::vector<vertex> vertices;
	std::vector<GLuint> elements;
	glm::mat4 transform;

	bool use_elements = false;

	mesh();
	mesh(bool use_elements);
	mesh(std::vector<vertex> &vertices, std::vector<GLuint> &elements);
	mesh(std::vector<vertex> &vertices);

	mesh(const mesh& other);
	mesh(mesh&& other) noexcept;
	mesh& operator=(const mesh& other);
	mesh& operator=(mesh&& other) noexcept;

	virtual ~mesh();
	void draw(GLuint shader) const;
	void add(triangle triangle);
	void add(std::vector<triangle> &triangles);
	void update();

private:
	GLuint VAO = NULL, VBO = NULL, EBO = NULL;
};

