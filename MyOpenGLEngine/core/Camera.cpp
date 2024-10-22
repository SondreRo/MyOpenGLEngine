#include "Camera.h"
#include <imgui.h>

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}


void Camera::ProcessInput(GLFWwindow* window, float deltaTime)
{
	float AdjustedCameraSpeed = CameraSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		AdjustedCameraSpeed *= 2;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * AdjustedCameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * AdjustedCameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += AdjustedCameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= AdjustedCameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= glm::normalize(cameraUp) * AdjustedCameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += glm::normalize(cameraUp) * AdjustedCameraSpeed;


	float speed = 100.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pitch += speed;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= speed;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw -= speed;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw += speed;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		CameraSpeed += 100.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
		if (CameraSpeed > 0.2f)
			CameraSpeed -= 100.f * deltaTime;

}

void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		
	}
}

void Camera::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (!UseCamera)
	{
		return;
	}

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		UseCamera = true;
		firstMouse = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		UseCamera = false;
	}
}

void Camera::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!UseCamera)
	{
		return;
	}

	CameraSpeed += yoffset / 5;

	if (CameraSpeed < 0.2f)
	{
		CameraSpeed = 0.2f;
	}
}
