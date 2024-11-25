#include "CollisionManager.h"

#include <iostream>
#include "Timer.h"
#include "Ball.h"
#include "Application.h"
#include "BSpline/BSpline.h"

void CollisionManager::BruteForceCollisionChecks()
{
	if(FreezeUpdates)
		return;

	if (CheckCollision == false)
	{
		return;
	}

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

void CollisionManager::Switch(glm::vec3& vector)
{
	glm::vec3 TempVec = vector;

	vector.x = TempVec.x;
	vector.y = TempVec.z;
	vector.z = TempVec.y;
}

void CollisionManager::CollisionCheckDag(Ball* b1, Ball* b2, float dt)
{
	// Bruke likning (9.22)
	glm::vec3 p1 = b1->transform.GetLocation() + b1->velocity;

}

void CollisionManager::PhysicsUpdate(float DeltaTime, LandscapeMesh* landscape, std::vector<Ball*> balls)
{


	ImGui::Begin("Physics");

    std::string amount = "Balls" + std::to_string(balls.size());


	ImGui::Text(amount.c_str());
	ImGui::Checkbox("Render Lines", &RenderLines);
	ImGui::Checkbox("Update Lines", &UpdateLines);

	if (ImGui::Button("ClearLines"))
	{
		for (auto ball : balls)
		{
			ball->TracePoints.clear();
		}
	}

	ImGui::Checkbox("CheckBallCollision", &CheckCollision);
	ImGui::Checkbox("FreezeUpdates", &FreezeUpdates);
	ImGui::Checkbox("ResetBalls", &ResetBalls);
	ImGui::End();

	if (!FreezeUpdates)
		Timer += DeltaTime;

	bool BTimer = false;
	if (Timer > 1)
	{
		Timer = 0;
		BTimer = true;
	}
	for (auto ball: balls)
	{
		if (UpdateLines && !FreezeUpdates)
		{
			if (BTimer == true)
			{
				if (glm::length(ball->velocity) > 0.1f)
				{
					Vertex NewPoint = Vertex();
					NewPoint.position = ball->transform.GetLocation();
					glm::vec3 Fastcolor = glm::vec3(1, 0, 0);
					glm::vec3 SlowColor = glm::vec3(0, 1, 0);
					float Speed = glm::length(ball->velocity);
					float MaxSpeed = 30;
					float MinSpeed = 0;
					float SpeedRange = MaxSpeed - MinSpeed;
					float SpeedPercent = (Speed - MinSpeed) / SpeedRange;
					NewPoint.color = glm::mix(SlowColor, Fastcolor, SpeedPercent);
					NewPoint.normal = NewPoint.color;
					ball->TracePoints.emplace_back(NewPoint);


				
				}
			}
		}
	

		if (RenderLines)
		{
			if (BTimer)
			{
				std::vector<glm::vec3> ControlPoints = {};
				for (auto trace_point : ball->TracePoints)
				{
					ControlPoints.push_back(trace_point.position);
				}
				std::vector<float> knots = {};
				ball->LinePoints = BSpline::GeneratePoints(ControlPoints, knots, 150);
			}
			//std::vector<Vertex> NewLinePoints = BSpline::GenerateBSplineCurve(ball->TracePoints, 4, 100);

			if (ball->LinePoints.size() > 3)
			{
				for (int i = 0; i < ball->LinePoints.size(); i++)
				{
					if (i + 1 < ball->LinePoints.size())
					{
						Application::get().mScene.lineMesh->AddLine(ball->LinePoints[i], ball->LinePoints[i + 1]);
					}
					else
					{
						Application::get().mScene.lineMesh->AddLine(ball->LinePoints[i], ball->transform.GetLocation());
					}
				}
			}
				

		
		}
		
		if (!FreezeUpdates)
			UpdateBall(ball, DeltaTime, landscape);
	}
}

void CollisionManager::UpdateBall(Ball* ball, float DeltaTime, LandscapeMesh* landscape)
{
	if (!ball || !landscape) return;

	glm::vec3 ballPosition = ball->transform.GetLocation();
	Chunk* chunk = landscape->GetChunkFromPosition(ballPosition);
	if (!chunk)
	{

		if (ResetBalls)
			ball->Resetball();

		return;
	} 


	auto triangleResult = landscape->GetTriangleFromPosition(ballPosition);
	if (!triangleResult.first)
	{

		if (ResetBalls)
		{
			//Random float between -0,5 and 0,5
			float RandomFloatX = (rand() % 100) / 100.0f - 0.5f;
			float RandomFloatY = (rand() % 100) / 100.0f - 0.5f;
			float RandomFloatZ = (rand() % 100) / 100.0f - 0.5f;

			ball->SpawnLocation += glm::vec3(RandomFloatX, RandomFloatY, RandomFloatZ);
			ball->Resetball();
		}
		
		return;
	}

	const Triangle& triangle = triangleResult.second;

	// Calculate height on triangle and normal
	glm::vec3 realPosition = CollisionManager::BarycentricCheck(triangle, ballPosition);
	glm::vec3 normal = glm::normalize(glm::cross(triangle.vB.position - triangle.vA.position,triangle.vC.position - triangle.vA.position));

	// Adjust normal to match coordinate system
	normal = glm::vec3(normal.x, normal.z, normal.y);

	// Calculate acceleration vector
	glm::vec3 acceleration = 9.81f * glm::vec3(
		normal.x * normal.z,
		normal.y * normal.z,
		(normal.z * normal.z) - 1
	);

	// Swap Y and Z to match coordinate system
	acceleration = glm::vec3(acceleration.x, acceleration.z, acceleration.y);

	// Apply friction
	float frictionCoefficient = triangle.Friction; // Get friction from triangle
	glm::vec3 velocity = ball->velocity;
	if (glm::length(velocity) > 0.0f) {
		glm::vec3 frictionForce = -frictionCoefficient * glm::length(acceleration) * glm::normalize(velocity);
		acceleration += frictionForce; // Friction reduces acceleration in velocity's direction
	}


	// Update velocity and position
	ball->velocity += acceleration * DeltaTime;

	glm::vec3 newPosition = ball->transform.GetLocation() + (ball->velocity * DeltaTime);

	if (ballPosition.y - ball->Radius <= realPosition.y)
	{
		newPosition.y = realPosition.y + ball->Radius;
		ball->velocity.y = 0.0f;

	}
	else if (ballPosition.y - 1 > realPosition.y)
	{
		ball->velocity.y -= 9.81f * DeltaTime; // Gravity

	}
	
	//{
	//	ball->velocity.y = 0.0f;
	//	newPosition.y = 0.0f + ball->Radius;
	//}


	ball->transform.SetLocation(newPosition);







	//glm::vec3 BallPos = ball->transform.GetLocation();
	//Chunk* myChunk = landscape->GetChunkFromPosition(BallPos);
	//if (myChunk == nullptr)
	//{
	//	return;
	//}

	//std::pair<bool, Triangle> Triangle = landscape->GetTriangleFromPosition(BallPos);
	//if (!Triangle.first)
	//{
	//	return;
	//}
	//// Get HeightOnTriangle
	//glm::vec3 RealPos = CollisionManager::BarycentricCheck(Triangle.second, BallPos);

	//// Get Normal
	//glm::vec3 Normal = glm::normalize(glm::cross(Triangle.second.vB.position - Triangle.second.vA.position, Triangle.second.vC.position - Triangle.second.vA.position));
	//Normal = glm::vec3(Normal.x, Normal.z, Normal.y);

	//// Get Acceleraion
	//glm::vec3 AccelerasjonsVector = 9.81f * glm::vec3(Normal.x * Normal.z, Normal.y * Normal.z, (Normal.z * Normal.z) - 1);
	//AccelerasjonsVector = glm::vec3(AccelerasjonsVector.x, AccelerasjonsVector.z, AccelerasjonsVector.y);


	//// Get Velocity
	//ball->velocity = ball->velocity + (AccelerasjonsVector * DeltaTime);




	//// Update Position
	//glm::vec3 NyPosition = ball->transform.GetLocation() + (ball->velocity * DeltaTime);
	//NyPosition.y = RealPos.y + ball->Radius;
	//ball->transform.SetLocation(NyPosition);
}


glm::vec3 CollisionManager::BarycentricCheck(Triangle triangle, glm::vec3 position)
{
	//glm::vec3 P = triangle.vA.position;
	//glm::vec3 Q = triangle.vB.position;
	//glm::vec3 R = triangle.vC.position;

	//glm::vec3 p = position;

	//P.y = 0;
	//Q.y = 0;
	//R.y = 0;
	//p.y = 0;
	//Switch(P);
	//Switch(Q);
	//Switch(R);
	//Switch(p);

	//float Ar = glm::length(glm::cross(Q - P, R - P));

	//float U = (glm::cross(Q - position, R - position).z) / Ar;
	//float V = (glm::cross(R - position, P - position).z) / Ar;
	//float W = (glm::cross(P - position, Q - position).z) / Ar;

	////float U = glm::length(glm::cross(Q - p, R - p)) / Ar; // Weight for Vertex A
	////float V = glm::length(glm::cross(R - p, P - p)) / Ar; // Weight for Vertex B
	////float W = glm::length(glm::cross(P - p, Q - p)) / Ar; // Weight for Vertex C

	////// Ensure the point lies within the triangle
	////if (U + V + W > 1.001f || U < 0 || V < 0 || W < 0) {
	////	std::cout << "Point is outside the triangle" << std::endl;
	////	return glm::vec3(0); // Or handle the error as needed
	////}

	////float xCoord = U * triangle.vA.position.x + V * triangle.vB.position.x + W * triangle.vC.position.x;
	////float yCoord = U * triangle.vA.position.y + V * triangle.vB.position.y + W * triangle.vC.position.y;
	////float zCoord = U * triangle.vA.position.z + V * triangle.vB.position.z + W * triangle.vC.position.z;


	//glm::vec3 a = P;
	//glm::vec3 b = Q;
	//glm::vec3 c = R;

	//P = triangle.vA.position;
	//Q = triangle.vB.position;
	//R = triangle.vC.position;

	//Switch(P);
	//Switch(Q);
	//Switch(R);

	//float xCoord = U * P.x + V * Q.x + W * R.x;
	//float yCoord = U * P.y + V * Q.y + W * R.y;
	//float zCoord = U * P.z + V * Q.z + W * R.z;



	//return { position.x, yCoord, position.z};

	glm::vec3 p1 = triangle.vA.position;
	glm::vec3 p2 = triangle.vB.position;
	glm::vec3 p3 = triangle.vC.position;
	glm::vec3 p4 = position;

	glm::vec3 p12 = p2 - p1;
	glm::vec3 p13 = p3 - p1;
	glm::vec3 cross = glm::cross(p13, p12);
	float area_123 = cross.y; // double the area
	glm::vec3 baryc; // for return

	// u
	glm::vec3 p = p2 - p4;
	glm::vec3 q = p3 - p4;
	glm::vec3 nu = glm::cross(q, p);
	// double the area of p4pq
	baryc.x = nu.y / area_123;

	// v
	p = p3 - p4;
	q = p1 - p4;
	glm::vec3 nv = glm::cross(q, p);
	// double the area of p4pq
	baryc.y = nv.y / area_123;

	// w
	p = p1 - p4;
	q = p2 - p4;
	glm::vec3 nw = (glm::cross(q, p));
	// double the area of p4pq
	baryc.z = nw.y / area_123;


	glm::vec3 Adjusted = p1 * baryc.x + p2 * baryc.y + p3 * baryc.z;
	return Adjusted;
}
