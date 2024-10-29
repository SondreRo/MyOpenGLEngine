#pragma once
#include "Vertex.h"
#include <vector>
#include <filesystem>

//#include <queue>

//#include <thread>
#include "mesh.h"

class PunktSky {

public:

	static void ReadFile(std::filesystem::path path, Mesh* mesh);

	static void CreateChunks(Mesh* mesh, glm::vec3 min, glm::vec3 max, std::vector<Mesh*>& chunks);

	static void ReadFileMesh(std::filesystem::path path, Mesh* mesh, std::vector<Mesh*>& finalchunks);

	static void TriangulateMesh(Mesh* mesh, glm::vec3 min, glm::vec3 max, float Resolution, Mesh* triangulatedMesh);

};
