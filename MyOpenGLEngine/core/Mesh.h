#pragma once
#include <vector>

#include "ShaderProgram.h"
#include "Vertex.h"
#include "Transform.h"
#include "Material.h"

class Mesh {
public:
	Mesh() = default;
	Mesh(std::string inName);
	~Mesh();

	virtual void Draw();
	virtual void Update(float DeltaTime);
	virtual void Bind();
	virtual void Unbind();

	virtual void RenderProperties();

	std::string name;

	Material material;

	ShaderProgram* shaderProgram;

	Transform transform;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	glm::mat4 model = glm::mat4(1);


	std::vector<Mesh*> Children;

private:
	unsigned int VAO, VBO, EBO;

};