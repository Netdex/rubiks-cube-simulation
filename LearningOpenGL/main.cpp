#include "stdafx.h"

#include "glsl_io.h"
#include "vertex.h"
#include "mesh.h"
#include "triangle.h"
#include "axis.h"
#include <glad/glad.h>
#include "shader.h"
#include "rubiks_cube.h"

/*
 * The magical journey of learning OpenGL
 */

#define SCREEN_WIDTH	1280.f
#define SCREEN_HEIGHT	720.f

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
	if (!glfwInit())
	{
		printf("glfwInit() failed\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearningOpenGL",
		nullptr /* set to glfwGetPrimaryMonitor() for fullscreen */, nullptr);
	glfwMakeContextCurrent(window);
	
	// load opengl functions
	if (!gladLoadGL())
	{
		printf("gladLoadGL() failed\n");
		return -1;
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// set general opengl settings
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

	// load shaders
	GLuint vertexShader = 0;
	if (!GLSL_LoadShader("vertex.g", GL_VERTEX_SHADER, vertexShader))
	{
		GLSL_PrintShaderLog(vertexShader);
		printf("failed to compile vertex shader\n");
		system("pause");
		return -1;
	}
	GLuint fragmentShader = 0;
	if (!GLSL_LoadShader("fragment.g", GL_FRAGMENT_SHADER, fragmentShader))
	{
		printf("failed to compile fragment shader\n");
		GLSL_PrintShaderLog(fragmentShader);
		system("pause");
		return -1;
	}
	// create shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "render_color");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// get variables for MVP
	GLuint uniModel = glGetUniformLocation(shaderProgram, "model");
	GLuint uniView = glGetUniformLocation(shaderProgram, "view");
	float fov = glm::radians(45.0f);

	// initialize variables for camera
	GLuint uniCamera = glGetUniformLocation(shaderProgram, "camera_position");
	glm::vec3 position = glm::vec3(0, 0, 5);
	// horizontal angle : toward -Z
	float yaw = glm::pi<float>();
	// vertical angle : 0, look at the horizon
	float pitch = 0.0f;
	float speed = 10.0f;
	float mouseSpeed = 0.1f;
	glm::vec3 up(0, 1, 0);

	axis axis(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));


	rubiks_cube rc;
	int face = 0;
	bool rotating = false;
	bool cw = true;

	// last-minute display settings
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);
	
	float lastTick = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float now = glfwGetTime();
		float deltaTime = now - lastTick;
		lastTick = now;
		printf("\r%f ms/frame                        ", 1000.0f * deltaTime);
		// parse input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// calculate mouse movement
		if (focus) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			yaw += mouseSpeed * deltaTime * float(SCREEN_WIDTH / 2 - xpos);
			pitch += mouseSpeed * deltaTime * float(SCREEN_HEIGHT / 2 - ypos);
			if (pitch > glm::pi<float>() / 2 - 0.01)
				pitch = glm::pi<float>() / 2 - 0.01;
			if (pitch < -glm::pi<float>() / 2 + 0.01)
				pitch = -glm::pi<float>() / 2 + 0.01;
		}
		// calculate camera vectors
		glm::vec3 direction = normalize(glm::vec3(
			cos(pitch) * sin(yaw),
			sin(pitch),
			cos(pitch) * cos(yaw)
		));
		//printf("\r%f %f %f", position.x, position.y, position.z);
		glm::vec3 right = normalize(cross(direction, up));

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			position -= glm::vec3(0, 1, 0) * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			position += glm::vec3(0, 1, 0) * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
			if(fov > 0.01)
				fov -= 0.01;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
			if(fov < glm::pi<float>() - 0.01)
			fov += 0.01;
		}
		// update values changed by controls
		glUniform3fv(uniCamera, 1, value_ptr(position));

		glm::mat4 view = lookAt(
			position,
			position + direction,
			up
		);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, value_ptr(view));

		glm::mat4 proj = glm::perspective(fov, SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 100.0f);
		GLuint uniProj = glGetUniformLocation(shaderProgram, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		if(rotating)
		{
			if(rc.rotate_step(face, cw, (0.05 + sin(rc.rot_prog * 2)) * glm::pi<float>() / 28))
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
		
		axis.draw(shaderProgram);
		rc.draw(shaderProgram);
		rc.transform = 
			glm::eulerAngleXYZ(0.25f * glm::pi<float>() * cos(now), 0.25f * glm::pi<float>() * sin(now), 0.25f * glm::pi<float>() * cos(now));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glfwTerminate();

	return 0;
}