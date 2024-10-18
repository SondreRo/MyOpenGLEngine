#pragma once
#include "glm/glm.hpp"
#include "ComponentBase.h"

class MovementComponent : public ComponentBase {
public:
	MovementComponent() = default;
	MovementComponent(glm::vec3 invelocity) {
		Velocity = invelocity;
	}

	glm::vec3 Velocity = glm::vec3(0.f, 0.f, 0.f);

};