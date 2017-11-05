#include "stdafx.h"

#include "glsl_io.h"
#include "vertex.h"
#include "mesh.h"
#include "triangle.h"
#include "axis.h"
#include <glad/glad.h>
#include "shader.h"
#include "rubiks_cube.h"
#include "camera.h"

/*
 * The magical journey of learning OpenGL
 */

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

bool wireframe = false;
bool focus = true;

GLFWwindow* window;
//shader shdr;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_E:
			wireframe = !wireframe;
			if (wireframe)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLFW_KEY_LEFT_ALT:
			focus = !focus;
			if (focus)		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			else			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		}
	}
}

int main(const int argc, const char *argv[])
{
	// initialize opengl through glfw
	assert(glfwInit());
	
	// init gl hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	// init gl window and props
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearningOpenGL",
		nullptr /* set to glfwGetPrimaryMonitor() for fullscreen */, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);

	// load opengl functions
	assert(gladLoadGL());

	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// set general opengl settings
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

	shader sr;
	sr
		.attach("vertex.vert")
		.attach("fragment.frag")
		.link()
		.activate();
	
	camera c(window, SCREEN_WIDTH, SCREEN_HEIGHT);

	/* define world objects */
	axis axis(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
	rubiks_cube rc;
	int face = 0;
	bool rotating = false;
	bool cw = true;
	/* end define world objects */

	// game loop relevant variables
	
	
	float lastTick = (float) glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float now = (float) glfwGetTime();
		float deltaTime = now - lastTick;
		lastTick = now;
		printf("\r%f ms/frame                        ", 1000.0f * deltaTime);
		// parse input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		c.update(deltaTime, focus);
		sr.bind("view", c.view);
		sr.bind("camera_position", c.position);
		sr.bind("proj", c.proj);

		if(rotating)
		{
			if(rc.rotate_step(face, cw, (float)(0.05 + sin(rc.rot_prog * 2)) * glm::pi<float>() / 28))
			{
				rotating = false;
			}
		}
		
		// rubik's cube control
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			if (!rotating) {
				face = 0;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (!rotating) {
				face = 1;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			if (!rotating) {
				face = 2;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (!rotating) {
				face = 3;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (!rotating) {
				face = 4;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
			if (!rotating) {
				face = 5;
				rotating = true;
				cw = !glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		axis.draw(sr.get());
		rc.draw(sr.get());
		rc.transform = 
			glm::eulerAngleXYZ(0.25f * glm::pi<float>() * cos(now), 0.25f * glm::pi<float>() * sin(now), 0.25f * glm::pi<float>() * cos(now));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}