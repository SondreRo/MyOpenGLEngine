#pragma once

class tempTest {
public:
	static glm::vec3 GetNormalAfterCollision(glm::vec3 velocity, glm::vec3 collisionNormal) {


		glm::vec3 relativeVelocity = velocity;
		float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
		if (velocityAlongNormal > 0)
		{

			return {};
		}
		float e = 1.f;
		float j = -(1 + e) * velocityAlongNormal;
		glm::vec3 impulse = j * collisionNormal;

		if (impulse.length() > 0.001f && impulse.length() < 1000.f)
		{
			velocity += impulse;  // Apply the impulse to the ball only
		}


		return velocity;
	}

	static void GetNormalballToball() {
		Ball* ball1 = new Ball("Ball1");
		Ball* ball2 = new Ball("Ball2");
		ball1->Radius = 1 / 4;
		ball2->Radius = 1 / 4;
		ball1->velocity = { 2,0,-1 };
		ball2->velocity = { 2,0,1 };
	
		ball1->sphereCollider = SphereCollider({ 1,0,5 }, 1 / 4);
		ball2->sphereCollider = SphereCollider({ 2,0,1 }, 1 / 4);


		if (ball1 != nullptr && ball2 != nullptr)
		{
			//CollisionData data = SphereCollider::CheckCollision(ball1->GetSphereCollider(), ball2->GetSphereCollider());
			if (true)
			{

				glm::vec3 collisionNormal = glm::normalize(ball1->sphereCollider.center - ball2->sphereCollider.center);

				//Application::get().mScene.lineMesh->AddLine(ball1->GetSphereCollider().center, ball2->GetSphereCollider().center);
				//std::cout << "Ball Collision between " << meshes[i]->name << " and " << meshes[j]->name << std::endl;
				//ball1->transform.AddLocation(glm::normalize(data.collisionNormal) * (data.penetration/2) + 0.01f);
				//ball2->transform.AddLocation(-glm::normalize(data.collisionNormal) * (data.penetration / 2) + 0.01f);

				// Collision response

				glm::vec3 relativeVelocity = ball1->velocity - ball2->velocity;
				float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
				if (velocityAlongNormal > 0)
				{
					return;
				}
				float e = 1.f;
				float j = -(1 + e) * velocityAlongNormal;
				j /= 1 + 1;
				glm::vec3 impulse = j * collisionNormal;
				if (impulse.length() > 0.001f && impulse.length() < 1000.f)
				{
					ball1->velocity += impulse;
					ball2->velocity -= impulse;
				}


				//// Positional correction
				//const float percent = 0.2f;
				//const float slop = 0.01f;
				//glm::vec3 correction = std::max(data.penetration - slop, 0.f) / (1) * percent * data.collisionNormal;


				//if (correction.length() > 0.0001f && correction.length() < 1000.f)
				//{
				//	ball1->transform.AddLocation(correction / 2.f);
				//	ball2->transform.AddLocation(-correction / 2.f);
				//}


				std::cout << "Ball1 " << ball1->velocity.x << " " << ball1->velocity.y << " " << ball1->velocity.z << "\n";
				std::cout << "Ball2 " << ball2->velocity.x << " " << ball2->velocity.y << " " << ball2->velocity.z << "\n";
			}
		
		}

	}

};