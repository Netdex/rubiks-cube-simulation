#include "stdafx.h"
#include "rubiks_cube.h"
#include "triangle.h"

/*
 * Primary corners of a cubie
 */
static glm::vec3 CUBIE_BASE_POINTS[] = {
	glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1),
	glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1),
};

/*
 * Cubie vertice indices for building a cubie
 */
static int CUBIE_FACE_VERTEX_INDEX[] = {
	2,1,0,	2,3,1,	// U +y
	7,1,3,	7,5,1,	// R +x
	6,3,2,	6,7,3,	// F +z
	5,7,6,	6,4,5,	// D -y
	2,0,4,	2,4,6,	// L -x
	0,1,5,	5,4,0   // B -z
};

/*
 * Cubie color values in order URFDLB
 */
static glm::vec3 CUBIE_COLORS[] = {
	glm::vec3(1, 1, 1), glm::vec3(0, .5, 0), glm::vec3(1, 0, 0),
	glm::vec3(1, 1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0.647, 0)
};

/*
 * Index i is the axis that is preserved after a rotation in the ith face, index ij is the value of that preserved axis
 * Used for flipping rotations depending on the side, and for locating cubies on a face
 */
static int CUBIE_FACE_COORD[6][2] = {
	{1, 1}, {0, 1}, {2, 1},
	{1, -1},{ 0, -1 },{ 2, -1 },
};

/*
 * Index i is the axis to rotate around to emulate a rotation around the ith face
 */
static glm::vec3 CUBIE_FACE_AXIS[6] = {
	glm::vec3(0,-1,0),glm::vec3(-1,0,0),glm::vec3(0,0,-1),
	glm::vec3(0,1,0),glm::vec3(1,0,0),glm::vec3(0,0,1),
};

rubiks_cube::rubiks_cube() : transform(glm::mat4())
{
	int c = 0;
	for (int z = -1; z <= 1; z++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				int p[] = { x,y,z };
				mesh* cubie_mesh = build_cubie(p, CUBIE_COLORS);
				cubie_info cubie = {
					glm::vec3(x * (2 + CUBIE_SPACING), y * (2 + CUBIE_SPACING), z * (2 + CUBIE_SPACING)),
					glm::mat4(),
					{x,y,z},
					cubie_mesh };
				cubies[c++] = cubie;
			}
		}
	}
}


rubiks_cube::~rubiks_cube()
{
	for (int i = 0; i < 27; i++)
	{
		delete cubies[i].mesh;
	}
}

void rubiks_cube::draw(GLuint shaderProgram) 
{
	for (int i = 0; i < 27; i++)
	{
		cubies[i].mesh->transform = make_transform(cubies + i);
		cubies[i].mesh->draw(shaderProgram);
	}
}

void rubiks_cube::rotate(cubie_info* cubie, int face, float a) const
{
	cubie->rot = glm::rotate(glm::mat4(), a, CUBIE_FACE_AXIS[face]) * cubie->rot;
}

bool rubiks_cube::rotate_step(int face, bool cw, float ang)
{
	if (rot_prog > glm::pi<float>() / 2)
	{
		//printf("\n");
		for (int i = 0; i < 27; i++)
		{
			cubie_info *cubie = cubies + i;
			if (cubie->pe[CUBIE_FACE_COORD[face][0]] == CUBIE_FACE_COORD[face][1])
			{
				rotate(cubie, face, (cw ? 1 : -1) * (-rot_prog + glm::pi<float>() / 2));
				//printf("%d: %d %d %d", CUBIE_FACE_COORD[face][0], cubie->pe[0], cubie->pe[1], cubie->pe[2]);
				bool determinant = !(cw ^ CUBIE_FACE_COORD[face][1] == 1);
				switch(CUBIE_FACE_COORD[face][0])
				{
				case 0:			// rotation about yz-plane
					std::swap(cubie->pe[1], cubie->pe[2]);
					cubie->pe[determinant ? 2 : 1] *= -1;
					break;
				case 1:			// rotation about xz-plane
					std::swap(cubie->pe[0], cubie->pe[2]);
					cubie->pe[determinant ? 0 : 2] *= -1;
					break;
				case 2:			// rotation about xy-plane
					std::swap(cubie->pe[0], cubie->pe[1]);
					cubie->pe[determinant ? 1 : 0] *= -1;
					break;
				}
				//printf(" -> %d %d %d\n", cubie->pe[0], cubie->pe[1], cubie->pe[2]);
			}
		}
		rot_prog = 0;
		return true;
	}
	else {
		for (int i = 0; i < 27; i++)
		{
			cubie_info *cubie = cubies + i;
			if (cubie->pe[CUBIE_FACE_COORD[face][0]] == CUBIE_FACE_COORD[face][1])
			{
				rotate(cubie, face, cw ? ang : -ang);
			}
		}
		rot_prog += ang;
		return false;
	}
}

mesh* rubiks_cube::build_cubie(int p[], glm::vec3 c[])
{
	std::vector<triangle> vertices;
	for (int f = 0; f < 6; f++)
	{
		for (int i = 0; i < 2; i++)
		{
			vertices.push_back(triangle(
				// determine the vertices of the triangle based on the lookup table
				CUBIE_BASE_POINTS[CUBIE_FACE_VERTEX_INDEX[6 * f + 3 * i]],
				CUBIE_BASE_POINTS[CUBIE_FACE_VERTEX_INDEX[6 * f + 3 * i + 1]],
				CUBIE_BASE_POINTS[CUBIE_FACE_VERTEX_INDEX[6 * f + 3 * i + 2]],

				// determine the color of the inside of the cube
				p[CUBIE_FACE_COORD[f][0]] == CUBIE_FACE_COORD[f][1] ? c[f] : glm::vec3(0.1,0.1,0.1)	
			));
		}
	}
	mesh *m = new mesh();
	m->add(vertices);
	m->update();
	return m;
}

glm::mat4 rubiks_cube::make_transform(cubie_info *cubie) const
{
	return transform *
		glm::translate(
			cubie->rot,
			glm::vec3(cubie->p.x, cubie->p.y, cubie->p.z)
		);
}
