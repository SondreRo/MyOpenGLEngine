#pragma once
#include "Ball.h"
#include "Mesh.h"
#include "Colliders.h"
#include "LineMesh.h"
#include "OcTree.h"
#include "Triangle.h"
#include "Landscape/LandscapeMesh.h"

class CollisionManager {

	// Meshes registered for collision
	std::vector<Mesh*> meshes;

	void BruteForceCollisionChecks();

	// used to switch y and z axis in a vector
	static void Switch(glm::vec3& vector);

public:
	CollisionManager();

	OcTree* oc_tree;

	bool RegisterMeshForCollision(Mesh* mesh);
	bool UnRegisterMeshForCollision(Mesh* mesh);

	void Update(float DeltaTime);
	void Render(LineMesh* inLineMesh);

	void CollisionCheckDag(Ball* b1, Ball* b2, float dt);

	void PhysicsUpdate(float DeltaTime, LandscapeMesh* landscape, std::vector<Ball*> balls);
	void UpdateBall(Ball* ball, float DeltaTime, LandscapeMesh* landscape);

	bool RenderLines = true;
	bool UpdateLines = true;
	bool CheckCollision = true;
	bool FreezeUpdates = false;
	float Timer;
	bool ResetBalls = false;

	// Returns the height of the triangle at a given position
	static glm::vec3 BarycentricCheck(Triangle triangle, glm::vec3 position);
};
