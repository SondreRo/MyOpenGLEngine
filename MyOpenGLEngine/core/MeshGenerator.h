#pragma once

#include <glm/glm.hpp>
#include "mesh.h"

class MeshGenerator {
public:

	static glm::vec3 GetNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);


	static void GenerateCube(Mesh* inMesh, glm::vec3 Size);
	static void GenerateCubeWithHardEdges(Mesh* inMesh, glm::vec3 Size);

	static void GenerateSphere(Mesh* inMesh, float Radius, int Sectors, int Stacks);
	static void GenerateIcosahedron(Mesh* inMesh, int n);

	static void GeneratePoolTable(Mesh* floorMesh, Mesh* topWall, Mesh* bottomWall, Mesh* rightWall, Mesh* leftWall, glm::vec3 size);

	static void ToUnitSphere(Mesh* inMesh);
	static void Subdivide(Vertex a, Vertex b, Vertex c, int n, Mesh* inMesh);
};