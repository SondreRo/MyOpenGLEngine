#include "Colliders.h"
#include "Application.h"

// This function will check if the two AABBs are colliding
CollisionData AxisAlignedBoundingBox::CheckCollision(AxisAlignedBoundingBox Box1, AxisAlignedBoundingBox Box2)
{
	CollisionData data;
	if (Box1.max.x < Box2.min.x || Box1.min.x > Box2.max.x)
	{
		return data;
	}
	if (Box1.max.y < Box2.min.y || Box1.min.y > Box2.max.y)
	{
		return data;
	}
	if (Box1.max.z < Box2.min.z || Box1.min.z > Box2.max.z)
	{
		return data;
	}
	data.isColliding = true;
	return data;

}

// This only returns true if Box1 is entirely inside Box2
CollisionData AxisAlignedBoundingBox::CheckCollisionFullyInside(AxisAlignedBoundingBox Box1,
	AxisAlignedBoundingBox Box2)
{
		CollisionData data;
		if (Box1.min.x > Box2.min.x && Box1.max.x < Box2.max.x)
		{
			if (Box1.min.y > Box2.min.y && Box1.max.y < Box2.max.y)
			{
				if (Box1.min.z > Box2.min.z && Box1.max.z < Box2.max.z)
				{
					data.isColliding = true;
					return data;
				}
			}
		}
		return data;
}

CollisionData SphereCollider::CheckCollisionWithAABB(SphereCollider InSphere, AxisAlignedBoundingBox Box)
{
	CollisionData data;
	glm::vec3 ClosestPoint = glm::vec3(0);
	ClosestPoint = glm::clamp(InSphere.center, Box.min, Box.max);
	/*ClosestPoint.x = glm::clamp(InSphere.center.x, Box.min.x, Box.max.x);
	ClosestPoint.y = glm::clamp(InSphere.center.y, Box.min.y, Box.max.y);
	ClosestPoint.z = glm::clamp(InSphere.center.z, Box.min.z, Box.max.z);*/


	//std::cout << "Closest Point: " << ClosestPoint.x << " " << ClosestPoint.y << " " << ClosestPoint.z << std::endl;

	/*Application::get().mScene.lineMesh->AddBox(ClosestPoint, glm::vec3(0.1f));
	Application::get().mScene.lineMesh->AddLine(ClosestPoint, InSphere.center);*/



	float distance = glm::distance(InSphere.center, ClosestPoint);
	if (distance < InSphere.radius)
	{
		data.isColliding = true;
		data.collisionNormal = glm::normalize(InSphere.center - ClosestPoint);
		data.penetration = InSphere.radius - distance;

		return data;
	}

	return data;

}
