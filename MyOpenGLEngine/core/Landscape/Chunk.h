#pragma once
#include "Transform.h"
#include "Vertex.h"
#include <vector>
#include "Material.h"

class Chunk {

public:
	std::vector<Vertex> vertices;
	std::vector<Vertex> verticesTriangulated;
	
	std::vector<unsigned int> indices;

	float chunkSize = 0;
	float TriangleSize = 0;

	float MinX = FLT_MAX;
	float MinY = FLT_MAX;
	float MinZ = FLT_MAX;

	float MaxX = -FLT_MAX;
	float MaxY = -FLT_MAX;
	float MaxZ = -FLT_MAX;

	int xCount;
	int zCount;
	
	Material material;

	unsigned int VAO, VBO, EBO;

	float Friction = 0.0f;

	bool isBound = false;
	bool hide = false;

	glm::vec3 GetCenter()
	{
		return glm::vec3((MinX + MaxX) / 2, (MinY + MaxY) / 2, (MinZ + MaxZ) / 2);
	}
};