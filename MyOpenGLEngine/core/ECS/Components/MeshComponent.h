#pragma once
#include <string>
#include <vector>
#include "Vertex.h"
#include "ComponentBase.h"
#include "Material.h"
#include "ShaderProgram.h"

class MeshComponent : public ComponentBase
{
public:
	MeshComponent() = default;

	// Mesh Data
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;
	Material material;
	ShaderProgram* shaderProgram;
	
};