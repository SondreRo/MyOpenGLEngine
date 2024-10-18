#include "CombatSystem.h"

#include <iostream>

void CombatSystem::AttackEntity(Entity entity, Entity target)
{
	if (CombatHandler->entityToComponentMap.count(entity.ID) == 0 || 
		HealthHandler->entityToComponentMap.count(target.ID) == 0 || 
		TransformHandler->entityToComponentMap.count(entity.ID) == 0 ||
		TransformHandler->entityToComponentMap.count(target.ID) == 0 )
	{
		return;
	}

	unsigned int SelfCombatID = CombatHandler->entityToComponentMap[entity.ID];
	unsigned int TargetHealthID = HealthHandler->entityToComponentMap[target.ID];
	unsigned int TargetTransformID = TransformHandler->entityToComponentMap[target.ID];
	unsigned int SelfTransformID = TransformHandler->entityToComponentMap[entity.ID];

	

	int damage = CombatHandler->components[SelfCombatID].damage;

	glm::vec3 direction = TransformHandler->components[SelfTransformID].transform.GetLocation() - TransformHandler->components[TargetTransformID].transform.GetLocation();

	float distance = glm::length(direction);

	if (distance < CombatHandler->components[SelfCombatID].StandardRange)
	{
		HealthHandler->components[TargetHealthID].health -= damage;
	}
}

void CombatSystem::AttackAll(Entity entity)
{
	if (CombatHandler->entityToComponentMap.count(entity.ID) == 0 ||
		TransformHandler->entityToComponentMap.count(entity.ID) == 0)
	{
		return;
	}

	unsigned int SelfCombatID = CombatHandler->entityToComponentMap[entity.ID];
	unsigned int SelfTransformID = TransformHandler->entityToComponentMap[entity.ID];

	int damage = CombatHandler->components[SelfCombatID].damage;

	glm::vec3 position = TransformHandler->components[SelfTransformID].transform.GetLocation();

	for (auto& newentity : CombatHandler->entityToComponentMap)
	{
		if (newentity.first == entity.ID)
		{
			continue;
		}

		unsigned int TargetHealthID = HealthHandler->entityToComponentMap[newentity.first];
		unsigned int TargetTransformID = TransformHandler->entityToComponentMap[newentity.first];

		glm::vec3 direction = position - TransformHandler->components[TargetTransformID].transform.GetLocation();

		float distance = glm::length(direction);

		if (distance < CombatHandler->components[SelfCombatID].StandardRange)
		{
			std::cout << "Attacking Entity: " << newentity.first << "\n";
			HealthHandler->components[TargetHealthID].health -= damage;

			if (HealthHandler->components[TargetHealthID].health <= 0)
			{
				std::cout << "Entity: " << newentity.first << " has died\n";
			}
		}

	}
}
