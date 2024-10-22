#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include "Components/ComponentBase.h"
class Entity {

public:

	~Entity();


	template <typename T>
	void AddComponent(T* component);

	void RemoveComponent(ComponentBase* component);

	bool CheckIfComponentExists(ComponentBase* component);

	template <typename T>
	T* GetComponent()
	{
		std::string name = typeid(T).name();
		if (components.count(name) == 0)
		{
			//std::cout << "Component does not exist" << std::endl;
			return nullptr;
		}
		return static_cast<T*>(components[name]);
	}

	std::unordered_map<std::string, ComponentBase*> components;
};

template <typename T>
void Entity::AddComponent(T* component)
{
	if (CheckIfComponentExists(component))
	{
		std::cout << "Component already exists" << std::endl;
		return;
	}
	std::string name = typeid(*component).name();
	components[name] = component;
}
