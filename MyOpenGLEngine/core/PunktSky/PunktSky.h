#pragma once
#include <vector>
#include "Vertex.h"

#include <filesystem>
#include <fstream> 

#include "ShaderProgram.h"
#include "Material.h"
#include <queue>

#include <thread>
#include "mesh.h"

class PunktSky {

public:

	~PunktSky();
	bool shouldClose = false;
	std::vector<Vertex> vertices;
	
	void ReadFile(std::filesystem::path path, Mesh* mesh);

	ShaderProgram* shaderProgram;
	Material material;

	void ReadFileThreaded(std::filesystem::path path, Mesh* mesh);
	//std::queue<Vertex> verticesQueue;

	

	unsigned int VAO, VBO;
	bool isBound = false;

	void Bind();
	void Draw();
};
