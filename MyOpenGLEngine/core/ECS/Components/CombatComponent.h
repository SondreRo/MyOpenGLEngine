#pragma once
#include <chrono>

#include "ComponentBase.h"
#include "Entity.h"

class CombatComponent : public ComponentBase
{
public:
	CombatComponent() = default;
	CombatComponent(int health, int damage, Entity* target)
	{
		Health = health;
		Damage = damage;
		Target = target;
		MaxHealth = health;
	}

	int Health = 100;
	int MaxHealth = 100;
	int Damage = 10;
	Entity* Target = nullptr;

	std::chrono::system_clock::time_point TimerLastHit;
};