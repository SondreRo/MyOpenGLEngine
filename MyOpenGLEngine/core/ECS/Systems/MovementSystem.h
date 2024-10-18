#pragma once
#include <vector>

#include "systembase.h"
#include "../ComponentHandler/ComponentHandler.h"
#include "../Components/MovementComponent.h"
#include "../Components/TransformComponent.h"
#include "ECS/Entity.h"

class MovementSystem : public SystemBase{
public:

	ComponentHandler<MovementComponent>* MovementHandler;
	ComponentHandler<TransformComponent>* TransformHandler;

	void MoveEntity(Entity entity, float DeltaTime);

};
