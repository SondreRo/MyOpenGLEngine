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

	bool IsPlayer = false;
};