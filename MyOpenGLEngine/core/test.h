#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "framebuffer.h"

#include "Camera.h"

class Test {

public:

	int Width = 1920;
	int Height = 1080;

	FrameBuffer* framebuffer;

	Camera* camera;

	int MyTest();
	void OpenDockSpace(bool* p_open);

	void WindowFramebufferSizeCallback(int width, int height);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	void ProcessInput(GLFWwindow* window);

	// Callbacks
	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};