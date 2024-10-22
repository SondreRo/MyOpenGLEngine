#include "Entity.h"

#include <vcruntime_typeinfo.h>
#include <iostream>


Entity::~Entity()
{
	for (auto& component : components)
	{
		delete component.second;
	}
}

void Entity::RemoveComponent(ComponentBase* component)
{
	if (!CheckIfComponentExists(component))
	{
		//std::cout << "Component does not exist" << std::endl;
		return;
	}
	std::string name = typeid(*component).name();
	components.erase(name);
}

bool Entity::CheckIfComponentExists(ComponentBase* component)
{
	std::string name = typeid(*component).name();
	return components.count(name) > 0;
}
