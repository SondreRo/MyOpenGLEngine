#include "CollisionManager.h"

#include <iostream>
#include "Timer.h"
#include "Ball.h"
#include "Application.h"

void CollisionManager::BruteForceCollisionChecks()
{
	//Application::get().mScene.StartTimer("BruteColCheck");
	for (int i = 0; i < meshes.size(); i++)
	{
		if (!meshes[i]->UseCollision)
			continue;

		if (meshes[i]->Static)
		{
			continue;
		}

		for (int j = 0; j < meshes.size(); j++)
		{
			if (meshes[i] == meshes[j])
				continue;

			if (!meshes[i]->UseCollision || !meshes[j]->UseCollision)
				continue;

			// Check if the AABBs are colliding
			

			//if (AxisAlignedBoundingBox::CheckCollision(meshes[i]->GetAABB(), meshes[j]->GetAABB()).isColliding)
			{

				// if both AABBs are colliding and are balls we do e do a sphere collision check
				Ball* ball1 = dynamic_cast<Ball*>(meshes[i]);
				Ball* ball2 = dynamic_cast<Ball*>(meshes[j]);

				if (ball1 != nullptr && ball2 != nullptr)
				{
					CollisionData data = SphereCollider::CheckCollision(ball1->GetSphereCollider(), ball2->GetSphereCollider());
					if (data.isColliding)
					{
						//Application::get().mScene.lineMesh->AddLine(ball1->GetSphereCollider().center, ball2->GetSphereCollider().center);
						//std::cout << "Ball Collision between " << meshes[i]->name << " and " << meshes[j]->name << std::endl;
						//ball1->transform.AddLocation(glm::normalize(data.collisionNormal) * (data.penetration/2) + 0.01f);
						//ball2->transform.AddLocation(-glm::normalize(data.collisionNormal) * (data.penetration / 2) + 0.01f);

						// Collision response

						glm::vec3 relativeVelocity = ball1->velocity - ball2->velocity;
						float velocityAlongNormal = glm::dot(relativeVelocity, data.collisionNormal);
						if (velocityAlongNormal > 0)
						{
							continue;
						}
						float e = 1.f;
						float j = -(1 + e) * velocityAlongNormal;
						j /= 1 + 1;
						glm::vec3 impulse = j * data.collisionNormal;
						if (impulse.length() > 0.001f && impulse.length() < 1000.f)
						{
							ball1->velocity += impulse;
							ball2->velocity -= impulse;
						}
				

						// Positional correction
						const float percent = 0.2f;
						const float slop = 0.01f;
						glm::vec3 correction = std::max(data.penetration - slop, 0.f) / (1) * percent * data.collisionNormal;

						
						if (correction.length() > 0.0001f && correction.length() < 1000.f)
						{
							ball1->transform.AddLocation(correction / 2.f);
							ball2->transform.AddLocation(-correction / 2.f);
						}
				

						continue;
					}
					else
					{
						continue;
					}
				}

				// if only one of the meshes is a ball, we do sphere to AABB collision check
				if (ball1 != nullptr && ball2 == nullptr)
				{
					//std::cout << "Checking Sphere to AABB Collision" << std::endl;
					//Application::get().mScene.lineMesh->AddLine(ball1->GetSphereCollider().center, meshes[j]->GetAABB().min);

					CollisionData data = SphereCollider::CheckCollisionWithAABB(ball1->GetSphereCollider(), meshes[j]->GetAABB());
					if (data.isColliding)
					{
						//std::cout << "Ball & AABB Collision between " << meshes[i]->name << " and " << meshes[j]->name << std::endl;

						glm::vec3 collisionNormal = glm::normalize(data.collisionNormal);

						//Application::get().mScene.lineMesh->AddLine(ball1->GetSphereCollider().center, ball1->GetSphereCollider().center + data.collisionNormal * 3.f);
						//Collision response

						glm::vec3 relativeVelocity = ball1->velocity;
						float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
						if (velocityAlongNormal > 0)
						{

							continue;
						}
						float e = 1.f;
						float j = -(1 + e) * velocityAlongNormal;
						glm::vec3 impulse = j * collisionNormal;

						if (impulse.length() > 0.001f && impulse.length() < 1000.f)
						{
							ball1->velocity += impulse;  // Apply the impulse to the ball only

							if (fabs(collisionNormal.y) > 0.9f)  // Assuming the floor is along the Y-axis
							{
								ball1->velocity.y *= 0.8f;  // Reduce the Y velocity after the collision
							}
						}


						// Positional correction
						const float percent = 0.2f;
						const float slop = 0.01f;
						glm::vec3 correction = std::max(data.penetration - slop, 0.f) / (1) * percent * collisionNormal;
						if (correction.length() > 0.001f && correction.length() < 1000.f)
						{
							ball1->transform.AddLocation(correction / 2.f);

						}

						continue;

						//glm::vec3 relativeVelocity = ball1->velocity;
						//float velocityAlongNormal = glm::dot(relativeVelocity, glm::normalize(meshes[j]->GetAABB().min - ball1->GetSphereCollider().center));
						//if (velocityAlongNormal > 0)
						//{
						//	continue;
						//}
						//float e = 1.f;
						//float j = -(1 + e) * velocityAlongNormal;
						//j /= 1 + 1;
						//glm::vec3 impulse = j * glm::normalize(meshes[j]->GetAABB().min - ball1->GetSphereCollider().center);
						//ball1->velocity += impulse;
						//

						continue;
					}
				}
				//// if only one of the meshes is a ball, we do sphere to AABB collision check
				//if (ball2 != nullptr || ball1 == nullptr)
				//{
				//	if (SphereCollider::CheckCollisionWithAABB(ball2->GetSphereCollider(), meshes[i]->GetAABB()).isColliding)
				//	{
				//		std::cout << "Ball & AABB Collision between " << meshes[j]->name << " and " << meshes[i]->name << std::endl;

				//	}
				//	return;
				//}


				// If all above cases fail, we just do AABB to AABB collision check
				//std::cout << "Collision between " << meshes[i]->name << " and " << meshes[j]->name << std::endl;
				continue;

			}

		}
	}
	//Application::get().mScene.EndTimer("BruteColCheck");

}

CollisionManager::CollisionManager()
{
	oc_tree = new OcTree(glm::vec3(0, 0, 0), glm::vec3(200, 200, 200));
}

bool CollisionManager::RegisterMeshForCollision(Mesh* mesh)
{
	/*if (mesh->UseCollision)
	{
		meshes.push_back(mesh);
		return true;
	}
	else
	{
		std::cout << "Mesh " << mesh->name << " does not have collision enabled." << std::endl;
		return false;
	}*/
	meshes.push_back(mesh);
	return true;
}

bool CollisionManager::UnRegisterMeshForCollision(Mesh* mesh)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] == mesh)
		{
			meshes.erase(meshes.begin() + i);
			return true;

		}
	}
	return false;
}




void CollisionManager::Update(float DeltaTime)
{
	//Application::get().mScene.StartTimer("UpdateCollisionManager");

	//Timer::startTimer("BruteColCheck");
	BruteForceCollisionChecks();
	//oc_tree->TEMPCHECK(meshes);
	//oc_tree->SortMeshesIntoNodes(meshes);
	//Timer::stopTimer("BruteColCheck");

	/*oc_tree->DoCollisionCheck(oc_tree->root, meshes);
	oc_tree->TreeCollision();*/
	//Application::get().mScene.EndTimer("UpdateCollisionManager");

}

void CollisionManager::Render(LineMesh* inLineMesh)
{
	oc_tree->DrawTree();
	//Application::get().mScene.StartTimer("renderCollisionManager");
	//Application::get().mScene.EndTimer("renderCollisionManager");
}

void CollisionManager::CollisionCheckDag(Ball* b1, Ball* b2, float dt)
{
	// Bruke likning (9.22)
	glm::vec3 p1 = b1->transform.GetLocation() + b1->velocity;

}
