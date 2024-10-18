#pragma once

#include "systembase.h"
#include "../Components/CombatComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TransformComponent.h"
#include "../Entity.h"
class CombatSystem : public SystemBase {


public:

	ComponentHandler<CombatComponent>* CombatHandler;
	ComponentHandler<HealthComponent>* HealthHandler;
	ComponentHandler<TransformComponent>* TransformHandler;

	void AttackEntity(Entity entity, Entity target);

	void AttackAll(Entity entity);

};