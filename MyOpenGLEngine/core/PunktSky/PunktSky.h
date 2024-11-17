#pragma once
#include "Vertex.h"
#include <vector>
#include <filesystem>

//#include <queue>

//#include <thread>
#include "mesh.h"



#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

class PunktSky {

public:
	static float ChunkSize;
	static float MeshResolution;
	static std::queue<Mesh*> inputQueue;
	static std::queue<Mesh*> outputQueue;
	std::mutex queueMutex;
	std::condition_variable cv;

	void worker();

	//static void ReadFile(std::filesystem::path path, Mesh* mesh);

	void CreateChunks(Mesh* mesh, glm::vec3 min, glm::vec3 max, std::vector<Mesh*>& chunks);

	void ReadFileMesh(std::filesystem::path path, Mesh* mesh, std::vector<Mesh*>& finalchunks, float inChunkSize, float inMeshResolution, bool useVertexColors);

	void TriangulateMesh(Mesh* mesh, Mesh* triangulatedMesh);

};
