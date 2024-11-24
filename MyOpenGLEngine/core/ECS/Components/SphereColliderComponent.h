#pragma once

#include "ComponentBase.h"

class SphereColliderComponent : public ComponentBase
{
public:
	SphereColliderComponent(float Radius) : Radius(Radius) {}

	float Radius;
};
