#pragma once
#include "systemBase.h"

#include "../Components/TransformComponent.h"
#include "../Components/SphereColliderComponent.h"
#include "../Components/VelocityComponent.h"

class CollisionSystem : public SystemBase
{

public:
	virtual void Setup(Entity* entity, ECSManager* manager) override
	{
		SystemBase::Setup(entity, manager);
	}

	virtual void Update(Entity* entity, float DeltaTime) override
	{
		if (SphereColliderComponent* SCC = entity->GetComponent<SphereColliderComponent>())
		{
			if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
			{
				if (VelocityComponent* VC = entity->GetComponent<VelocityComponent>())
				{
                    float MaxX = 25;
                    float MinX = -25;
                    float MaxZ = 25;
                    float MinZ = -25;

                    //Check X min
                    if (TC->transform.GetLocation().x - SCC->Radius < MinX)
                    {
                    TC->transform.SetLocation(glm::vec3(MinX + SCC->Radius, TC->transform.GetLocation().y, TC->transform.GetLocation().z));
                    VC->Velocity.x = -VC->Velocity.x;
                    }

                    //Check X max
                    if (TC->transform.GetLocation().x + SCC->Radius > MaxX)
                    {
                    TC->transform.SetLocation(glm::vec3(MaxX - SCC->Radius, TC->transform.GetLocation().y, TC->transform.GetLocation().z));
                    VC->Velocity.x = -VC->Velocity.x;
                    }

                    //Check Z min
                    if (TC->transform.GetLocation().z - SCC->Radius < MinZ)
                    {
                    TC->transform.SetLocation(glm::vec3(TC->transform.GetLocation().x, TC->transform.GetLocation().y, MinZ + SCC->Radius));
                    VC->Velocity.z = -VC->Velocity.z;
                    }

                    //Check Z max
                    if (TC->transform.GetLocation().z + SCC->Radius > MaxZ)
                    {
                    TC->transform.SetLocation(glm::vec3(TC->transform.GetLocation().x, TC->transform.GetLocation().y, MaxZ - SCC->Radius));
                    VC->Velocity.z = -VC->Velocity.z;
                    }



					// CHeck all other entities

					for (auto otherEntity : ecsManager->Entities)
					{
						if (otherEntity == entity) continue;

						if (SphereColliderComponent* otherSCC = otherEntity->GetComponent<SphereColliderComponent>())
						{
							if (TransformComponent* otherTC = otherEntity->GetComponent<TransformComponent>())
							{
								if (VelocityComponent* otherVC = otherEntity->GetComponent<VelocityComponent>())
								{
									float Distance = glm::distance(TC->transform.GetLocation(), otherTC->transform.GetLocation());
									float CombinedRadius = SCC->Radius + otherSCC->Radius;

									if (Distance < CombinedRadius)
									{
										// Check if the other entity is moving away from us
										//if (glm::dot(VC->Velocity, otherVC->Velocity) > 0) continue;


										glm::vec3 Direction = glm::normalize(TC->transform.GetLocation() - otherTC->transform.GetLocation());
										TC->transform.SetLocation(TC->transform.GetLocation() + Direction * (CombinedRadius - Distance));
										VC->Velocity = glm::reflect(VC->Velocity, Direction);

										Direction = -Direction;
										otherTC->transform.SetLocation(otherTC->transform.GetLocation() + Direction * (CombinedRadius - Distance));
										otherVC->Velocity = glm::reflect(otherVC->Velocity, Direction);
									}
								}
							}
						}
					}

				}
			}
		}


		
	}

	virtual void Render(Entity* entity) override
	{

	}
};