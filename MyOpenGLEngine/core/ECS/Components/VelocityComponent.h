#pragma once
#include "ComponentBase.h"
#include "glm/vec3.hpp"

class VelocityComponent : public ComponentBase
{
public:
	VelocityComponent() = default;
	VelocityComponent(glm::vec3 velocity)
	{
		Velocity = velocity;
	}

	glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
};