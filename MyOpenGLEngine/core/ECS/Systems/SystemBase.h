#pragma once
#include "../Entity.h"



class ECSManager;
class SystemBase {
public:

	SystemBase() = default;

	ECSManager* ecsManager = nullptr;

	virtual void Setup(Entity* entity, ECSManager* manager)
	{
		ecsManager = manager;

	}

	virtual void Update(Entity* entityfloat, float DeltaTime)
	{
		
	}

	virtual void Render(Entity* entity)
	{
		
	}

	virtual void MouseButtonCallback(Entity* entity)
	{
		
	}
};
