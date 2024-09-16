#pragma once
#include <queue>
#include <string>
#include <unordered_map>

#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "LineMesh.h"
#include "Collision/CollisionManager.h"
#include "MeshGenerator.h"
#include <unordered_map>
class Window;

class Scene {

public:
	Scene(const std::string& name);
	~Scene();

	CollisionManager* collision_manager;

	virtual void LoadContent();
	virtual void UnloadContent();

	int width, height;

	void Bind();
	void BindSceneGraph(Mesh* mesh);
	int subCount = 1;

	void Init();
	void Update(float DeltaTime);
	void UpdateSceneGraph(float DeltaTime, Mesh* mesh, glm::mat4 ParentTransform);
	void Render(float DeltaTime, int width, int height);
	void RenderSceneGraph(Mesh* mesh, float DeltaTime, int width, int height, glm::mat4 ParentTransform);
	void ReadyRenderData(ShaderProgram* shader, Mesh* mesh, glm::mat4 parentTransform, int width, int height);

	template <typename T>
	T* CreateAndRegisterMesh(std::string name, glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale, ShaderProgram* shader_program, T* parent, bool UseCollision);

	void RenderSceneGraphVisuals();
	void RenderSceneGraphVisualsChldren(Mesh* mesh, int Depth);
	void SelectMesh(Mesh* mesh);
	Mesh* SelectedMesh = nullptr;

	unsigned int LastShader;

	Camera* camera;

	LineMesh* lineMesh;

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


	std::unordered_map<std::string, float> Timers;
	

	void StartTimer(std::string TimerName);

	void EndTimer(std::string TimerName);
	void RenderTimer(float dt);
};
