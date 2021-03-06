#pragma once
class camera
{
	GLFWwindow *window = NULL;

	float width, height;
	float fov = glm::radians(45.0f);
	float yaw = glm::pi<float>();			// horizontal angle : toward -Z
	float pitch = 0.0f;						// vertical angle : 0, look at the horizon
	float speed = 10.0f;
	float mouseSpeed = 0.1f;
	glm::vec3 up = glm::vec3(0, 1, 0);
	
public:
	glm::vec3 position = glm::vec3(0, 0, 5);
	glm::mat4 view, proj;

	camera(GLFWwindow *window, float width, float height);
	~camera();

	void update(float deltaTime, bool focus);
};

