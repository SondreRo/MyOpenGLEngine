#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <Vertex.h>
#include <vector>


struct CollisionData
{
	bool isColliding = false;
	glm::vec3 collisionNormal = glm::vec3(0);
	float penetration = 0.f;
};

class Collider {
public:

};


class AxisAlignedBoundingBox : public Collider {
public:

	glm::vec3 min = glm::vec3(1.f);
	glm::vec3 max = glm::vec3(-1.f);

	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(glm::vec3 Loc, glm::vec3 Size)
	{
		min = Loc - (Size * 0.5f);
		max = Loc + (Size * 0.5f);
	}
	AxisAlignedBoundingBox(std::vector<Vertex> vertices) {
		min = glm::vec3(FLT_MAX);
		max = glm::vec3(-FLT_MAX);

		for (auto vertex : vertices) {
			min = glm::min(min, vertex.position);
			max = glm::max(max, vertex.position);
			//min *= 0.5f;
			//max *= 0.5f;
		}
	}
	
	static CollisionData CheckCollision(AxisAlignedBoundingBox Box1, AxisAlignedBoundingBox Box2);
	static CollisionData CheckCollisionFullyInside(AxisAlignedBoundingBox Box1, AxisAlignedBoundingBox Box2);
};

class SphereCollider : public Collider {

public:
	glm::vec3 center = glm::vec3(0);
	float radius = 1.f;
	SphereCollider() = default;
	SphereCollider(glm::vec3 inCenter, float inRadius)
	{
		center = inCenter;
		radius = inRadius;
	}

	static CollisionData CheckCollision(SphereCollider Sphere1, SphereCollider Sphere2)
	{
		CollisionData data;
		float distance = glm::distance(Sphere1.center, Sphere2.center);
		data.collisionNormal = glm::normalize(Sphere1.center - Sphere2.center);

		if (distance < Sphere1.radius + Sphere2.radius)
		{
			data.isColliding = true;
			data.penetration = Sphere1.radius + Sphere2.radius - distance;
			return data;
		}
		return data;
	}

	static CollisionData CheckCollisionWithAABB(SphereCollider InSphere, AxisAlignedBoundingBox Box);
	

};