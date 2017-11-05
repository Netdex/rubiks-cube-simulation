#pragma once
class vertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	

	vertex();
	vertex(glm::vec3 position, glm::vec3 color);
	vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color);
	~vertex();

	static void set_vertex_attrib();
	
};

