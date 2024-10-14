#include "Ball.h"

#include "imgui.h"
#include "MeshGenerator.h"
#include "math.h"

Ball::Ball(std::string inName)
{
	name = inName;
	sphereCollider = SphereCollider(transform.GetLocation(), Radius);
}

Ball::~Ball()
{

}

void Ball::Update(float DeltaTime)
{
	Mesh::Update(DeltaTime);

	sphereCollider.center = transform.GetLocation();
	sphereCollider.radius = Radius;

	// Check that the velocity is valid

	if (isnan(velocity.x))
	{
		Resetball();
	}
	if (isnan(velocity.y))
	{
		Resetball();
	}
	if (isnan(velocity.z))
	{
		Resetball();
	}


	if (isnan(transform.GetLocation().x))
	{
		Resetball();
	}

	if (isnan(transform.GetLocation().y))
	{
		Resetball();
	}

	if (isnan(transform.GetLocation().z))
	{
		Resetball();
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
	return sphereCollider;
}

void Ball::Resetball()
{
		transform.SetLocation(glm::vec3(0,2,0));
		velocity = glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
}
