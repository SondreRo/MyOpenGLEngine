#pragma once

#include "ComponentBase.h"

class InputComponent : public ComponentBase
{

public:
	InputComponent() = default;
	InputComponent(bool isPlayer)
	{
		IsPlayer = isPlayer;
	}
	float Speed = 1.0f;
	bool IsPlayer = false;
};