#pragma once
#include "ComponentBase.h"
#include "../core/Transform.h"

class TransformComponent : public ComponentBase {
public:
	TransformComponent() = default;
	TransformComponent(Transform inTransform) {
		transform = inTransform;
	}
	TransformComponent(glm::vec3 inLocation) {
		transform.SetLocation(inLocation);
	}

	Transform transform = Transform();
};