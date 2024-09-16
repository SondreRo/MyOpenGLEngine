#include "Ball.h"

#include "imgui.h"
#include "MeshGenerator.h"

Ball::Ball(std::string inName)
{
	name = inName;
}

Ball::~Ball()
{

}

void Ball::Update(float DeltaTime)
{
	Mesh::Update(DeltaTime);

	// Check that the velocity is valid
	if (velocity.x != velocity.x)
	{
		velocity.x = 0;
	}
	if (velocity.y != velocity.y)
	{
		velocity.y = 0;

	}
	if (velocity.z != velocity.z)
	{
		velocity.z = 0;
	}


	transform.AddLocation(velocity * DeltaTime);

	//Gravity

	

	velocity.y -= 9.8f * DeltaTime;

	if (transform.GetLocation().y < -10)
	{
		Resetball();

	}
}

void Ball::RenderProperties()
{
	Mesh::RenderProperties();

	ImGui::Text("Velocity");
	ImGui::SliderFloat("X", &velocity.x, -30, 30);
	ImGui::SliderFloat("Y", &velocity.y, -30, 30);
	ImGui::SliderFloat("Z", &velocity.z, -30, 30);

	if (ImGui::Button("Resetball"))
	{
		Resetball();
	}

	ImGui::NewLine(); if (ImGui::SliderInt("Subdivision Count", &SubCount, 0, 6))
	{
		Unbind();
		vertices.clear();
		indices.clear();

		vertices.resize(0);
		MeshGenerator::GenerateIcosahedron(this, SubCount);
		Bind();
	}

}

SphereCollider Ball::GetSphereCollider()
{
	//SphereCollider collider{ GetGlobalTransform() * glm::vec4(transform.GetLocation(), 1), Radius};
	SphereCollider collider{transform.GetLocation(), Radius};
	return collider;
}

void Ball::Resetball()
{
		transform.SetLocation(glm::vec3(0,2,0));
		velocity = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
}
