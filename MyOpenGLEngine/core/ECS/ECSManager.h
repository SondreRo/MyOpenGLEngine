#pragma once

#include <vector>
#include "Entity.h"

#include "Systems/SystemBase.h"

// Extra
#include "MeshBase.h"
#include "Window.h"

class ECSManager {

public:

	ECSManager();

	void Setup();
	void SystemSetup();

	void Update(float DeltaTime);
	void Render();

	void CreateEnemy(glm::vec3 Position);

	void DeleteEntity(Entity* entity);
	MeshBase* BoxMesh;


	void MouseMoveCallback(Window* window, double xpos, double ypos);
	void MouseButtonCallback(Window* window, int button, int action, int mods);
	void MouseScrollCallback(Window* window, double xoffset, double yoffset);
	void KeyCallback(Window* window, int key, int scancode, int action, int mods);

	void ProcessInput(Window* window, float DeltaTime);

	std::vector<Entity*> Entities;

	std::vector<Entity*> EntitiesToDelete;

private:
	Entity* CreateEntity();
	std::vector<SystemBase*> Systems;
};


