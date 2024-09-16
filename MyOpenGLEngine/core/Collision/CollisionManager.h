#pragma once
#include "Ball.h"
#include "Mesh.h"
#include "Colliders.h"
#include "LineMesh.h"
#include "OcTree.h"

class CollisionManager {

	// Meshes registered for collision
	std::vector<Mesh*> meshes;

	void BruteForceCollisionChecks();

public:
	CollisionManager();

	OcTree* oc_tree;

	bool RegisterMeshForCollision(Mesh* mesh);
	bool UnRegisterMeshForCollision(Mesh* mesh);

	void Update(float DeltaTime);
	void Render(LineMesh* inLineMesh);

	void CollisionCheckDag(Ball* b1, Ball* b2, float dt);
};