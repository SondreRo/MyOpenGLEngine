#pragma once
#include "ComponentBase.h"

class CombatComponent : public ComponentBase {
public:
	CombatComponent() = default;
	CombatComponent(int inDamage) {
		damage = inDamage;
	}

	int damage = 10;
	float StandardRange = 2.f;

	float ArealRange = 25.f;
};