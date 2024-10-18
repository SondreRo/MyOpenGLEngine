#pragma once
#include "ComponentBase.h"

class HealthComponent : public ComponentBase {
public:
	HealthComponent() = default;
	HealthComponent(int inHealth) {
		health = inHealth;
	}

	int health = 100;
};