#pragma once
#include "Transform.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"


class Camera {
public:

	Camera();
	~Camera();


	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));

	float yaw = -90;
	float pitch = 0;

	float CameraSpeed = 4.f;

	bool firstMouse = true;
	double lastX{ 0 }, lastY{ 0 };
	bool UseCamera = false;

	Transform transform;

	glm::mat4 GetViewMatrix();


	void ProcessInput(GLFWwindow* window, float deltaTime);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
