#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

class FrameBuffer;
class Scene;

class Window {

public:
	Window(std::string name, Scene* scene, int width, int height);
	~Window();

	void Init();
	void RegisterWindowCallback();
	bool LoadContent();
	void StartFrame();
	void Update(float DeltaTime);
	void Render(float DeltaTime);
	void EndFrame();


	void OpenDockSpace(bool* p_open);

	bool IsClosed();

	// Callbacks
	void FramebufferSizeCallback(struct GLFWwindow* window, int width, int height);
	void MouseMoveCallback(struct GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseScrollCallback(struct GLFWwindow* window, double xoffset, double yoffset);
	void KeyCallback(struct GLFWwindow* window, int key, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* window, unsigned int c);

	void ProcessInput(GLFWwindow* window, float DeltaTime);

	// Setters
	void SetWidth(int width);
	void SetHeight(int height);

	// Getters
	int GetWidth() const;
	int GetHeight() const;

	FrameBuffer* GetFrameBuffer() const { return mFramebuffer; };

	struct GLFWwindow* GetGLFWWindow() const { return mGLFWWindow; };


private:
	FrameBuffer* mFramebuffer;
	std::string mName;
	int mWidth, mHeight;

	Scene* mScene{ nullptr };
	struct GLFWwindow* mGLFWWindow{ nullptr };
};
