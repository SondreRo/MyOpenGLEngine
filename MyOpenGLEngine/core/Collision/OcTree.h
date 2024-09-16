#pragma once
#include <glm/glm.hpp>
#include "OcTreeNode.h"

class OcTree
{
public:
	OcTree(glm::vec3 inPos, glm::vec3 inSize);
	~OcTree();
	OcTreeNode* Root;


	
	void SplitNode(OcTreeNode* node);
	void DrawTree();
	void DrawNode(OcTreeNode* node);

};