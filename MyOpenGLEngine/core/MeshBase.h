#pragma once
#include <vector>

#include "Vertex.h"
#include "ShaderProgram.h"
#include "Material.h"

class MeshBase {
public:

	MeshBase() = default;


	unsigned int VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> MeshCorners;

	ShaderProgram* shaderProgram;
	Material material;

	bool isBound = false;

	void Draw() {

		if (!isBound) {
			Bind();
		}

		glBindVertexArray(VAO);

		material.BindMaterial(shaderProgram);

		if (indices.size() > 0)
		{
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		}
	}

	void Bind() {
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

		isBound = true;
	}

};