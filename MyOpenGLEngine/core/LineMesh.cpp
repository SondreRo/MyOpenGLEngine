#include "LineMesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"


LineMesh::LineMesh(std::string inName)
{
	name = inName;
}

void LineMesh::Draw()
{
	//return;
	Rebind();


	if (VertexColorAsColor)
	{
		shaderProgram->SetUniform1i("useColorNormal", 1);
	}
	else {
		shaderProgram->SetUniform1i("useColorNormal", 0);
	}


	glLineWidth(LineWith);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertices.size());
	vertices.clear();
}

void LineMesh::AddLine(glm::vec3 start, glm::vec3 end)
{
	//return;
	vertices.emplace_back(start);
	vertices.emplace_back(end);
	//Rebind();
}

void LineMesh::AddLine(Vertex start, Vertex end)
{
	//return;
	vertices.emplace_back(start);
	vertices.emplace_back(end);
	//Rebind();
}

void LineMesh::AddBox(glm::vec3 Location, glm::vec3 Size)
{
	//return;
	glm::vec3 min = Location - (Size * 0.5f);
	glm::vec3 max = Location + (Size * 0.5f);

	//AddBoxMinMax(min, max);

}

void LineMesh::AddBoxMinMax(glm::vec3 min, glm::vec3 max)
{
	// Front
	AddLine(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z));
	AddLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z));

	AddLine(glm::vec3(max.x, max.y, min.z), glm::vec3(min.x, max.y, min.z));
	AddLine(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, min.y, min.z));

	// Back
	AddLine(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z));
	AddLine(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z));

	AddLine(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z));
	AddLine(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, min.y, max.z));

	// Sides
	AddLine(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z));
	AddLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z));

	AddLine(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z));
	AddLine(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z));

	//Rebind();
}


void LineMesh::Rebind()
{
	//std::cout << "Rebinding LineMesh" << std::endl;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	Vertex::BindAttributes();

}

void LineMesh::RenderProperties()
{
	Mesh::RenderProperties();

	ImGui::DragFloat("LineWidth", &LineWith, 0.1f, 1.0f, 10.0f);
}
