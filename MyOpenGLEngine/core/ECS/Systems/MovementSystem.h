#pragma once

#include "SystemBase.h"

// Components
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"

#include <iostream>

class MovementSystem : public SystemBase
{
public:
	virtual void Setup(Entity* entity, ECSManager* manager) override
	{
		SystemBase::Setup(entity, manager);
	}

	virtual void Update(Entity* entity, float DeltaTime) override
	{
		if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
		{
			if (VelocityComponent* VC = entity->GetComponent<VelocityComponent>())
			{
				TC->transform.AddLocation(VC->Velocity * DeltaTime);
				//VC->Velocity *= 1 - (1 * DeltaTime);
				//std::cout << "Velocity: " << VC->Velocity.x << ", " << VC->Velocity.y << ", " << VC->Velocity.z << std::endl;
			}
		}
	}

	virtual void Render(Entity* entity) override
	{

	}
};