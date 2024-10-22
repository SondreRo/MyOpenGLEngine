#pragma once

#include "SystemBase.h"

#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/CombatComponent.h"


#include "ECSManager.h"
#include <chrono>

class AICombatSystem : public SystemBase
{
public:
	AICombatSystem() = default;

	void Update(Entity* entity, float DeltaTime) override
	{
		if (CombatComponent* CC = entity->GetComponent<CombatComponent>()) {
			if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
			{
				if (VelocityComponent* VC = entity->GetComponent<VelocityComponent>())
				{
					if (CC->Target != nullptr && !CC->Target->components.empty())
					{
						if (TransformComponent* TargetTC = CC->Target->GetComponent<TransformComponent>())
						{
							glm::vec3 Direction = TargetTC->transform.GetLocation() - TC->transform.GetLocation();
							Direction = glm::normalize(Direction);
							Direction.y = 0;
							VC->Velocity += Direction * 20.f * DeltaTime;

							float Distance = glm::distance(TC->transform.GetLocation(), TargetTC->transform.GetLocation());

							if (Distance < 1)
							{
								//std::cout << "Entity took damage" << std::endl;
								TakeDamage(CC->Target, CC->Damage);
								if (VelocityComponent* TargetVC = CC->Target->GetComponent<VelocityComponent>())
								{
									Direction = glm::normalize(Direction);
									TargetVC->Velocity += Direction * 80.f;
								}
							}

							//// Random float between -1 and 1
							//float RandX = (rand() % 20 - 10) / 10.f;
							//float RandZ = (rand() % 20 - 10) / 10.f;

							//RandX *= 2;
							//RandZ *= 2;
							//

							//VC->Velocity.x += RandX;
							//VC->Velocity.z += RandZ;
						}
					}
				}
			}
		}
	}

	void ArealAttack(Entity* entity, float Radius, int Damage)
	{
		if (CombatComponent* CC = entity->GetComponent<CombatComponent>())
		{
			for (auto OtherEntity : ecsManager->Entities)
			{
				if (OtherEntity == entity)
					continue;
				if (CombatComponent* OtherCC = OtherEntity->GetComponent<CombatComponent>())
				{
					if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
					{
						if (TransformComponent* OtherTC = OtherEntity->GetComponent<TransformComponent>())
						{
							if (glm::distance(TC->transform.GetLocation(), OtherTC->transform.GetLocation()) < Radius)
							{

								VelocityComponent* VC = OtherEntity->GetComponent<VelocityComponent>();
								if (VC)
								{
									glm::vec3 Direction = glm::normalize(OtherTC->transform.GetLocation() - TC->transform.GetLocation());
									float Distance = glm::distance(TC->transform.GetLocation(), OtherTC->transform.GetLocation());

									float Percentage = 1 - (Distance / Radius);

									Direction.y = 0;
									VC->Velocity += Direction * 200.f * Percentage;
								}

								TakeDamage(OtherEntity, Damage);
								//std::cout << "Entity took damage" << std::endl;
							}
						}
					}
				}
			}
		}
	}

	virtual void MouseButtonCallback(Entity* entity) override
	{
		
		ArealAttack(entity, 20.f, 10);
		

	}
	void TakeDamage(Entity* entity, int Damage)
	{
		if (CombatComponent* CC = entity->GetComponent<CombatComponent>())
		{
			if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
			{

				auto now = std::chrono::system_clock::now();

				// Check if 2 seconds have passed since the last hit
				if (now < CC->TimerLastHit + std::chrono::seconds(1)) {
					//std::cout << "Too soon!" << std::endl;
					return;
				}

				// Store the current time for future comparisons
				CC->TimerLastHit = now;  // Store time as time_point instead of float

				//std::cout << "Entity took damage" << std::endl;
				float Percentage = (float)CC->Health / (float)CC->MaxHealth;
				TC->transform.SetScale(glm::vec3(Percentage));
			}
			CC->Health -= Damage;
			if (CC->Health <= 0)
			{
				//std::cout << "Entity died" << std::endl;
				ecsManager->DeleteEntity(entity);
			}
		}
	}
};