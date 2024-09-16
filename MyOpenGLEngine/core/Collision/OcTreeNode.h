#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"
#include <queue>
struct OcTreeNode {
	OcTreeNode(glm::vec3 inPos, glm::vec3 inSize, std::vector<Mesh*> inMeshes)
	{
		Pos = inPos;
		Size = inSize;
		aabb = AxisAlignedBoundingBox(Pos, Size);

		Meshes = inMeshes;
	}



	OcTreeNode(glm::vec3 inPos, glm::vec3 inSize)
	{
		Pos = inPos;
		Size = inSize;
		aabb = AxisAlignedBoundingBox(Pos, Size);
	}
	~OcTreeNode();

	glm::vec3 Pos;
	glm::vec3 Size;
	AxisAlignedBoundingBox aabb;

	std::vector<Mesh*> Meshes; // Meshes in node

	std::vector<OcTreeNode*> child_nodes;
	OcTreeNode* parent;


};

inline OcTreeNode::~OcTreeNode()
{
	for (int i = 0; i < child_nodes.size(); ++i)
	{
		delete child_nodes[i];
	}
}
