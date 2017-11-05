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
	
	mesh(std::vector<vertex> &vertices, std::vector<GLuint> &elements);
	mesh(std::vector<vertex> &vertices);
	mesh(std::vector<triangle> &triangles);

	mesh(const mesh& other);
	mesh(mesh&& other) noexcept;
	mesh& operator=(const mesh& other);
	mesh& operator=(mesh&& other) noexcept;

	virtual ~mesh();
	void draw(GLuint shader) const;
	
	void update();
private:
	GLuint VAO = NULL, VBO = NULL, EBO = NULL;
	void setup();

	void add(triangle triangle);
	void add(std::vector<triangle> &triangles);
};

