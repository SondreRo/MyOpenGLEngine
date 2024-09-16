#include "OcTree.h"
#include "Application.h"
OcTree::OcTree(glm::vec3 inPos, glm::vec3 inSize)
{
	Root = new OcTreeNode(inPos, inSize);
	//Root->Meshes = meshes;
	//SplitNode(Root);

	//for (auto chi : Root->Children)
	//{
	//	SplitNode(chi);
	//	//for (auto ch : chi->Children)
	//	//{
	//	//	SplitNode(ch);
	//	//}
	//}
}

void OcTree::SplitNode(OcTreeNode* node)
{


	glm::vec3 half = node->Size / 4.0f;
	glm::vec3 childSize = node->Size / 2.0f;

	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(half.x, half.y, half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(half.x, half.y, -half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(half.x, -half.y, half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(half.x, -half.y, -half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(-half.x, half.y, half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(-half.x, half.y, -half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(-half.x, -half.y, half.z), childSize));
	node->child_nodes.push_back(new OcTreeNode(node->Pos + glm::vec3(-half.x, -half.y, -half.z), childSize));
}

void OcTree::DrawTree()
{
	std::queue<OcTreeNode*> nodes;
	nodes.push(Root);

		while (!nodes.empty())
		{
			OcTreeNode* node = nodes.front();
			nodes.pop();

			DrawNode(node);

			for (int i = 0; i < node->child_nodes.size(); i++)
			{
				nodes.push(node->child_nodes[i]);
			}
		}
}

void OcTree::DrawNode(OcTreeNode* node)
{
	Application::get().mScene.lineMesh->AddBoxMinMax(node->aabb.min, node->aabb.max);
}
