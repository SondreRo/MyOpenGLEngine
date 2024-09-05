#pragma once
#include <string>
#include <unordered_map>

#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"

class Window;

class Scene {

public:
	Scene(const std::string& name);
	~Scene();

	virtual void LoadContent();
	virtual void UnloadContent();

	void Bind();
	void BindSceneGraph(Mesh* mesh);
	int subCount = 1;

	void Init();
	void Update(float DeltaTime);
	void UpdateSceneGraph(float DeltaTime, Mesh* mesh);
	void Render(float DeltaTime, int width, int height);
	void RenderSceneGraph(Mesh* mesh, float DeltaTime, int width, int height, glm::mat4 ParentTransform);

	void RenderSceneGraphVisuals();
	void RenderSceneGraphVisualsChldren(Mesh* mesh, int Depth);
	void SelectMesh(Mesh* mesh);
	Mesh* SelectedMesh = nullptr;

	Camera* camera;

	Mesh* RootMesh;
	Mesh* SunMesh;
	//ShaderProgram* shaderProgram;

	std::unordered_map<std::string, Mesh*> Meshes;


	// Callbacks for window events
	void FramebufferSizeCallback(Window* window, int width, int height);
	void MouseMoveCallback(Window* window, double xpos, double ypos);
	void MouseButtonCallback(Window* window, int button, int action, int mods);
	void MouseScrollCallback(Window* window, double xoffset, double yoffset);
	void KeyCallback(Window* window, int key, int scancode, int action, int mods);

	void ProcessInput(Window* window, float DeltaTime);


};
