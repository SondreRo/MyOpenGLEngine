#pragma once
#include <vector>
#include "Vertex.h"

#include <filesystem>
#include <fstream> 

#include "ShaderProgram.h"
#include "Material.h"
class PunktSky {

public:
	std::vector<Vertex> vertices;

	void ReadFile(std::filesystem::path path);

	ShaderProgram* shaderProgram;
	Material material;

	unsigned int VAO, VBO;
	bool isBound = false;

	void Bind();
	void Draw();
};