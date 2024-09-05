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
	transform.AddLocation(velocity * DeltaTime);
}

void Ball::RenderProperties()
{
	Mesh::RenderProperties();

	ImGui::Text("Velocity");
	ImGui::SliderFloat("X", &velocity.x, -10, 10);
	ImGui::SliderFloat("Y", &velocity.y, -10, 10);
	ImGui::SliderFloat("Z", &velocity.z, -10, 10);

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
