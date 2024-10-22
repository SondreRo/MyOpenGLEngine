#pragma once
#include "componentBase.h"
#include "../core/Transform.h"

class TransformComponent : public ComponentBase
{
public:
	TransformComponent() = default;

	TransformComponent(glm::vec3 position)
	{
		transform.SetLocation(position);
	}

	TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		transform.SetLocation(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
	}

	Transform transform;
};