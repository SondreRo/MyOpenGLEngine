#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 normal = glm::vec3(0.f,1.f,0.f);
	glm::vec2 textureCoord = glm::vec2(0.f);

	Vertex() = default;
	Vertex(glm::vec3 pos) : position(pos) {}
	Vertex(float x, float y, float z) : position(glm::vec3(x, y, z)) {}
	Vertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor) {}
	Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : position(pos), normal(nor), textureCoord(tex) {}

	// Bind Vertex Attributes for the shader. Remember to change the shader to accept the corrent attributes
	static void BindAttributes() {
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		// Texture Coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoord));
		glEnableVertexAttribArray(2);
		
		/*
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, mTexCoord));
		glEnableVertexAttribArray(2);*/
	}

};