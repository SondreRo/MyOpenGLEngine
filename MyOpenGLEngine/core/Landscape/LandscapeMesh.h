#pragma once
#include <unordered_map>

#include "Chunk.h"
#include "Mesh.h"
#include "Triangle.h"

class LandscapeMesh : public Mesh
{
public:
	LandscapeMesh(std::string inName);

	void FillchunkMap();

	int XSelect = 0;
	int ZSelect = 0;

	virtual void Bind() override;
	virtual void Draw() override;
	virtual void RenderProperties() override;

	bool RenderFrictionColor = false;

	float Friction = 0.f;
	std::vector<Chunk*> chunks;

	std::vector<std::vector<Chunk*> > chunksGrid;
	std::unordered_map<int, std::unordered_map<int, Chunk*>> chunkMap;
	Chunk* SelectedChunk = nullptr;
	Chunk* GetChunkFromPosition(glm::vec3 inPosition);
	
	std::pair<bool, Triangle> GetTriangleFromPosition(glm::vec3 inPosition);
};