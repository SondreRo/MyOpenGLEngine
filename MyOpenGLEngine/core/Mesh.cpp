#include <iostream>

#include "mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

Mesh::Mesh(std::string inName)
{
	name = inName;
}

Mesh::~Mesh()
{
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);

	if (indices.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
}

void Mesh::Update(float DeltaTime)
{
	Transformmat = transform.GetMatrix();
	UpdateAABB();
}

void Mesh::Bind()
{
	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
   
	Vertex::BindAttributes();
}

void Mesh::Unbind()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::RenderProperties()
{
	ImGui::Text(name.c_str());
	glm::vec3 position = transform.GetLocation();
	ImGui::DragFloat3("Position", glm::value_ptr(position), 0.05f);
	//ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f);
	transform.SetLocation(position);

	glm::vec3 rotation = transform.GetRotation();
	ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.05f);

	//ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -360.0f, 360.0f);
	transform.SetRotation(rotation);

	glm::vec3 scale = transform.GetScale();
	ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.05f);
	transform.SetScale(scale);

	std::string VertexCount = "Vertex Count: " + std::to_string(vertices.size());
	ImGui::Text(VertexCount.c_str());
	std::string IndexCount = "Index Count: " + std::to_string(indices.size());
	ImGui::Text(IndexCount.c_str());

	ImGui::NewLine();
	ImGui::Text("Material");
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(material.diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(material.specular));
	ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 256.0f);
	ImGui::SliderFloat("Transparency", &material.transparency, 0.0f, 1.0f);
	ImGui::NewLine();


	ImGui::Checkbox("UseCollision", &UseCollision);
}

AxisAlignedBoundingBox Mesh::GetAABB()
{
	return aabb;
}

void Mesh::UpdateAABB()
{
	aabb.min = glm::vec3(FLT_MAX);
	aabb.max = glm::vec3(-FLT_MAX);

	for (auto corner : MeshCorners)
	{
		corner = glm::vec3(GetGlobalTransform() * glm::vec4(corner, 1));
		aabb.min = glm::min(aabb.min, corner);
		aabb.max = glm::max(aabb.max, corner);
	}
}

void Mesh::AddChild(Mesh* mesh)
{
	mesh->Parent = this;
	Children.push_back(mesh);
}

glm::mat4 Mesh::GetTransformMat()
{
	return Transformmat;
}

glm::mat4 Mesh::GetGlobalTransform()
{
	return ParentMatrix * GetTransformMat();
}