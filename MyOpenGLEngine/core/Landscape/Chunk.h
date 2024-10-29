#pragma once
#include "Transform.h"
#include "Vertex.h"
#include <vector>

class Chunk {

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;
	bool isBound = false;

};