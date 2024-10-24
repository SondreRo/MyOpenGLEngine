#pragma once
#include "SystemBase.h"
#include "../Components/PickupComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/InputComponent.h"
#include "glfw-3.3.9/include/GLFW/glfw3.h"

class PickupSystem : public SystemBase
{
public:
	virtual void Update(Entity* entityfloat, float DeltaTime) override
	{
		if (entityfloat->GetComponent<PickupComponent>())
		{
			if (auto TC = entityfloat->GetComponent<TransformComponent>())
			{
				TC->transform.AddRotation(glm::vec3(0, 180 * DeltaTime, 0));

				if (auto TC = ecsManager->Entities[0]->GetComponent<TransformComponent>())
				{
					if (glm::distance(TC->transform.GetLocation(), entityfloat->GetComponent<TransformComponent>()->transform.GetLocation()) < 2)
					{
						ecsManager->Entities[0]->GetComponent<InputComponent>()->Speed *= entityfloat->GetComponent<PickupComponent>()->SpeedMultipier;
						ecsManager->DeleteEntity(entityfloat);
					}
				}
			}
		}
	}
};
