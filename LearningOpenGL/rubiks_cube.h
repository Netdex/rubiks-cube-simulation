#pragma once
#include "mesh.h"



struct cubie_info
{
	// graphical positional x, y, z, and angular x,y,z
	glm::vec3 p;
	glm::mat4 rot;

	// perceived x, y, z, modified by rotation state
	int pe[3];
	mesh *mesh;
};
class rubiks_cube
{
	const float CUBIE_SPACING = 0.1f;

	cubie_info cubies[27];
public:
	float rot_prog = 0;		// rotation progress
	glm::mat4 transform;

	rubiks_cube();
	~rubiks_cube();

	void draw(GLuint shaderProgram);
	void rotate(cubie_info* cubie, int face, float a) const;
	bool rotate_step(int face, bool cw, float ang);

private:
	

	/*
	 * Colors of cube in order URFDLB
	 */
	static mesh* build_cubie(int p[], glm::vec3 colors[]);
	glm::mat4 make_transform(cubie_info *cubie) const;
};

