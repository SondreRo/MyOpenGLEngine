#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "Entity.h"
#include "ComponentHandler/ComponentHandler.h"
#include "ComponentHandler/HandlerInterface.h"

//Systems
#include "Systems/CombatSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/MeshSystem.h"

class ECSManager {

public:

	ECSManager();
	void Setup();

	void CreateEnemy();

	void Update(float DeltaTime);
	void Render();

	void RemoveEntity(Entity& entity);
	Entity& CreateEntity();

	std::vector<Entity> Entities;

	template <typename T, typename... Args >
	void AddComponent(Entity& entity, Args&&... args);

	// Returns the entire map of components
	const std::unordered_map<std::string, HandlerInterface*>& GetComponentMap();

	// Returns a specific componentHandler
	template <typename T>
	ComponentHandler<T>* GetComponentHandler();


	//Systems
	MovementSystem Movement_System;
	MeshSystem Mesh_System;
	CombatSystem combat_system;
private:
	// Map of all componentHandlers
	std::unordered_map<std::string, HandlerInterface*> ComponentHandlers;
};


