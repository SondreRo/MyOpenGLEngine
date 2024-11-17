#pragma once
#include "Chunk.h"
#include <filesystem>
#include <queue>

#include <vector>
#include <mutex>
#include <condition_variable>

class Landscape {

public:
	std::vector<Chunk*> chunks;

	void ReadPointCloudFile(std::filesystem::path filePath);


	void Update();
	void Render();
	std::vector<Vertex> vertices;

private:
	void GetVerticesFromFile(std::ifstream& file, glm::vec3& min, glm::vec3& max);
	void CreateEmptyChunks(glm::vec3& min, glm::vec3& max);
	void StartFillChunks();
	void StartTriangulateChunks();
	
	// This function is only ran on the threads
	void Thread_ChunkFill_worker();
	// This function is only ran on the threads
	void FillChunksWithVertex(Chunk* inChunk);

	void Thread_ChunkTriangulate_worker();

	void TriangulateChunk(Chunk* inChunk);

	bool IsNumber(const std::string& s);

	void RenderChunk(Chunk* chunk);
	void BindChunk(Chunk* chunk);

	std::mutex queueMutex;
	static std::queue<Chunk*> chunkQueueInput;
	static std::queue<Chunk> chunkQueueOutput;

	float ChunkSize = 20;
	float TriangleSize = 0.5f;


	std::condition_variable cv;

};