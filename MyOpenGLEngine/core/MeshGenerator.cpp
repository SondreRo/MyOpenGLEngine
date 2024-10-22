#include <chrono>
#include <iostream>

#include "Meshgenerator.h"

glm::vec3 MeshGenerator::GetNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
		glm::vec3 A = p2 - p1;
		glm::vec3 B = p3 - p1;
		return -(glm::normalize(glm::cross(A, B)));
}

void MeshGenerator::GenerateNormals(Mesh* inMesh)
{
	for (int i = 0; i < inMesh->indices.size(); i += 3)
	{
		Vertex ver1 = inMesh->vertices[inMesh->indices[i]];
		Vertex ver2 = inMesh->vertices[inMesh->indices[i + 1]];
		Vertex ver3 = inMesh->vertices[inMesh->indices[i + 2]];

		glm::vec3 Normal = GetNormal(ver1.position, ver2.position, ver3.position);

		inMesh->vertices[inMesh->indices[i]].normal = -Normal;
		inMesh->vertices[inMesh->indices[i + 1]].normal = -Normal;
		inMesh->vertices[inMesh->indices[i + 2]].normal = -Normal;
	}
}

void MeshGenerator::GenerateCube(Mesh* inMesh, glm::vec3 Size)
{
	glm::vec3 v1 = { -0.5f * Size.x, -0.5f * Size.y, -0.5f * Size.z };
	glm::vec3 v2 = { 0.5f * Size.x, -0.5f * Size.y, -0.5f * Size.z };
	glm::vec3 v3 = { 0.5f * Size.x, 0.5f * Size.y, -0.5f * Size.z };
	glm::vec3 v4 = { -0.5f * Size.x, 0.5f * Size.y, -0.5f * Size.z };
	glm::vec3 v5 = { -0.5f * Size.x, -0.5f * Size.y, 0.5f * Size.z };
	glm::vec3 v6 = { 0.5f * Size.x, -0.5f * Size.y, 0.5f * Size.z };
	glm::vec3 v7 = { 0.5f * Size.x, 0.5f * Size.y, 0.5f * Size.z };
	glm::vec3 v8 = { -0.5f * Size.x, 0.5f * Size.y, 0.5f * Size.z };

	std::vector<Vertex> Vertices;
	//Vertices.emplace_back(v1, glm::normalize(v1));
	//Vertices.emplace_back(v2, glm::normalize(v2));
	//Vertices.emplace_back(v3, glm::normalize(v3));
	//Vertices.emplace_back(v4, glm::normalize(v4));
	//Vertices.emplace_back(v5, glm::normalize(v5));
	//Vertices.emplace_back(v6, glm::normalize(v6));
	//Vertices.emplace_back(v7, glm::normalize(v7));
	//Vertices.emplace_back(v8, glm::normalize(v8));

	Vertices.emplace_back(v1);
	Vertices.emplace_back(v2);
	Vertices.emplace_back(v3);
	Vertices.emplace_back(v4);
	Vertices.emplace_back(v5);
	Vertices.emplace_back(v6);
	Vertices.emplace_back(v7);
	Vertices.emplace_back(v8);

	inMesh->vertices = Vertices;
	inMesh->indices = {
		0, 1, 2,	 0, 2, 3,
		1, 5, 6,	 1, 6, 2,
		5, 4, 7,	 5, 7, 6,
		4, 0, 3,	 4, 3, 7,
		3, 2, 6,	 3, 6, 7,
		4, 5, 1,	 4, 1, 0 };

	for (int i = 0; i < inMesh->indices.size(); i += 3)
	{
				Vertex ver1 = inMesh->vertices[inMesh->indices[i]];
				Vertex ver2 = inMesh->vertices[inMesh->indices[i + 1]];
				Vertex ver3 = inMesh->vertices[inMesh->indices[i + 2]];

				glm::vec3 Normal = GetNormal(ver1.position, ver2.position, ver3.position);

				inMesh->vertices[inMesh->indices[i]].normal = Normal;
				inMesh->vertices[inMesh->indices[i + 1]].normal = Normal;
				inMesh->vertices[inMesh->indices[i + 2]].normal = Normal;
	}

	for (auto vertex : inMesh->vertices)
	{
		vertex.normal = glm::normalize(vertex.normal);
	}

	std::cout << "Generated Cube" << std::endl;
}

void MeshGenerator::GenerateCubeWithHardEdges(Mesh* inMesh, glm::vec3 Size)
{
	glm::vec3 halfSize = Size * 0.5f;

	// Define the 8 vertices of the cube
	glm::vec3 v0 = { -halfSize.x, -halfSize.y, -halfSize.z };
	glm::vec3 v1 = { halfSize.x, -halfSize.y, -halfSize.z };
	glm::vec3 v2 = { halfSize.x,  halfSize.y, -halfSize.z };
	glm::vec3 v3 = { -halfSize.x,  halfSize.y, -halfSize.z };
	glm::vec3 v4 = { -halfSize.x, -halfSize.y,  halfSize.z };
	glm::vec3 v5 = { halfSize.x, -halfSize.y,  halfSize.z };
	glm::vec3 v6 = { halfSize.x,  halfSize.y,  halfSize.z };
	glm::vec3 v7 = { -halfSize.x,  halfSize.y,  halfSize.z };

	// Define the faces and normals for each face of the cube
	glm::vec3 normals[6] = {
		{ 0.0f,  0.0f, -1.0f},  // Front face
		{ 0.0f,  0.0f,  1.0f},  // Back face
		{ 0.0f,  1.0f,  0.0f},  // Top face
		{ 0.0f, -1.0f,  0.0f},  // Bottom face
		{-1.0f,  0.0f,  0.0f},  // Left face
		{ 1.0f,  0.0f,  0.0f},  // Right face
	};

	// Define the vertex positions and normals for each face (6 faces, 2 triangles per face)
	// Each face is defined by 4 vertices and split into 2 triangles
	std::vector<glm::vec3> faceVertices[6] = {
		{v0, v1, v2, v3},  // Front face
		{v5, v4, v7, v6},  // Back face
		{v3, v2, v6, v7},  // Top face
		{v0, v4, v5, v1},  // Bottom face
		{v0, v3, v7, v4},  // Left face
		{v1, v5, v6, v2},  // Right face

	};
	
	// Add vertices and indices for each face
	for (int i = 0; i < 6; ++i) {
		glm::vec3 normal = normals[i];

		// Define two triangles per face (faceVertices[i] contains the 4 vertices of the face)
		// First triangle (v0, v1, v2)
		inMesh->vertices.emplace_back(faceVertices[i][0], normal);
		inMesh->vertices.emplace_back(faceVertices[i][1], normal);
		inMesh->vertices.emplace_back(faceVertices[i][2], normal);

		// Second triangle (v2, v3, v0)
		inMesh->vertices.emplace_back(faceVertices[i][2], normal);
		inMesh->vertices.emplace_back(faceVertices[i][3], normal);
		inMesh->vertices.emplace_back(faceVertices[i][0], normal);

		// Add the indices (6 indices for two triangles)
		unsigned int startIndex = i * 6; // Each face adds 6 vertices (2 triangles)
		inMesh->indices.push_back(startIndex + 2);
		inMesh->indices.push_back(startIndex + 1);
		inMesh->indices.push_back(startIndex + 0);

		inMesh->indices.push_back(startIndex + 5);
		inMesh->indices.push_back(startIndex + 4);
		inMesh->indices.push_back(startIndex + 3);
	}

	GetMeshCorners(inMesh);
	inMesh->UseCollision = true;
}

void MeshGenerator::GenerateCubeWithHardEdges(MeshBase* inMesh, glm::vec3 Size)
{
	glm::vec3 halfSize = Size * 0.5f;

	// Define the 8 vertices of the cube
	glm::vec3 v0 = { -halfSize.x, -halfSize.y, -halfSize.z };
	glm::vec3 v1 = { halfSize.x, -halfSize.y, -halfSize.z };
	glm::vec3 v2 = { halfSize.x,  halfSize.y, -halfSize.z };
	glm::vec3 v3 = { -halfSize.x,  halfSize.y, -halfSize.z };
	glm::vec3 v4 = { -halfSize.x, -halfSize.y,  halfSize.z };
	glm::vec3 v5 = { halfSize.x, -halfSize.y,  halfSize.z };
	glm::vec3 v6 = { halfSize.x,  halfSize.y,  halfSize.z };
	glm::vec3 v7 = { -halfSize.x,  halfSize.y,  halfSize.z };

	// Define the faces and normals for each face of the cube
	glm::vec3 normals[6] = {
		{ 0.0f,  0.0f, -1.0f},  // Front face
		{ 0.0f,  0.0f,  1.0f},  // Back face
		{ 0.0f,  1.0f,  0.0f},  // Top face
		{ 0.0f, -1.0f,  0.0f},  // Bottom face
		{-1.0f,  0.0f,  0.0f},  // Left face
		{ 1.0f,  0.0f,  0.0f},  // Right face
	};

	// Define the vertex positions and normals for each face (6 faces, 2 triangles per face)
	// Each face is defined by 4 vertices and split into 2 triangles
	std::vector<glm::vec3> faceVertices[6] = {
		{v0, v1, v2, v3},  // Front face
		{v5, v4, v7, v6},  // Back face
		{v3, v2, v6, v7},  // Top face
		{v0, v4, v5, v1},  // Bottom face
		{v0, v3, v7, v4},  // Left face
		{v1, v5, v6, v2},  // Right face

	};

	// Add vertices and indices for each face
	for (int i = 0; i < 6; ++i) {
		glm::vec3 normal = normals[i];

		// Define two triangles per face (faceVertices[i] contains the 4 vertices of the face)
		// First triangle (v0, v1, v2)
		inMesh->vertices.emplace_back(faceVertices[i][0], normal);
		inMesh->vertices.emplace_back(faceVertices[i][1], normal);
		inMesh->vertices.emplace_back(faceVertices[i][2], normal);

		// Second triangle (v2, v3, v0)
		inMesh->vertices.emplace_back(faceVertices[i][2], normal);
		inMesh->vertices.emplace_back(faceVertices[i][3], normal);
		inMesh->vertices.emplace_back(faceVertices[i][0], normal);

		// Add the indices (6 indices for two triangles)
		unsigned int startIndex = i * 6; // Each face adds 6 vertices (2 triangles)
		inMesh->indices.push_back(startIndex + 2);
		inMesh->indices.push_back(startIndex + 1);
		inMesh->indices.push_back(startIndex + 0);

		inMesh->indices.push_back(startIndex + 5);
		inMesh->indices.push_back(startIndex + 4);
		inMesh->indices.push_back(startIndex + 3);
	}

	GetMeshCorners(inMesh);
}

void MeshGenerator::GenerateSphere(Mesh* inMesh, float Radius, int Sectors, int Stacks)
{



	//for (int i = 0; i < Stacks - 1; i++)
	//{
	//	double phi = double(i + 1) / double(Stacks);
	//	for (int j = 0; j < Sectors; j++)
	//	{
	//		auto theta = 2.0f * glm::pi<float>() * double(j) / double(Sectors);
	//		auto x = std::sin(phi) * std::cos(theta);
	//		auto y = std::cos(phi);
	//		auto z = std::sin(phi) * std::sin(theta);
	//		inMesh->vertices.push_back(Vertex(x, y, z));
	//	}
	//}
}

void MeshGenerator::GenerateIcosahedron(Mesh* inMesh, int n)
{
	//start timer
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int Amount = 3 * 8 * std::pow(4,n);
	std::cout << "Getting ready to create " << Amount << " vertices" << std::endl;
	inMesh->vertices.reserve(Amount);

	auto v0 = Vertex(0, 0, 1);
	auto v1 = Vertex(1, 0, 0);
	auto v2 = Vertex(0, 1, 0);
	auto v3 = Vertex(-1, 0, 0);
	auto v4 = Vertex(0, -1, 0);
	auto v5 = Vertex(0, 0, -1);

	Subdivide(v0, v1, v2, n, inMesh);
	Subdivide(v0, v2, v3, n, inMesh);
	Subdivide(v0, v3, v4, n, inMesh);
	Subdivide(v0, v4, v1, n, inMesh);
	Subdivide(v5, v2, v1, n, inMesh);
	Subdivide(v5, v3, v2, n, inMesh);
	Subdivide(v5, v4, v3, n, inMesh);
	Subdivide(v5, v1, v4, n, inMesh);

	std::cout << "Sphere generated with: " << inMesh->vertices.size() << " vertices" << std::endl;


	//ToUnitSphere(inMesh);

	// add indices
	/*std::vector<unsigned int> Indices;
	inMesh->indices = {
		2, 1, 0,
		1, 2, 3,
		5, 4, 3,
		4, 8, 3,
		7, 6, 0,
		6, 9, 9,
		11, 10, 4,
		10, 11, 6,
		9, 5, 2,
		5, 9, 11,
		8, 7, 1,
		7, 8, 10,
		2, 5, 3,
		2, 5, 3,
		8, 1, 3,
		9, 2, 0,
		1, 7, 0,
		11, 9, 6,
		7, 10, 6,
		5, 11, 4,
		10, 8, 4
	};


	for (int i = 0; i < inMesh->indices.size(); i += 3)
	{
		Vertex ver1 = inMesh->vertices[inMesh->indices[i]];
		Vertex ver2 = inMesh->vertices[inMesh->indices[i + 1]];
		Vertex ver3 = inMesh->vertices[inMesh->indices[i + 2]];

		glm::vec3 Normal = GetNormal(ver1.position, ver2.position, ver3.position);

		inMesh->vertices[inMesh->indices[i]].normal = Normal;
		inMesh->vertices[inMesh->indices[i + 1]].normal = Normal;
		inMesh->vertices[inMesh->indices[i + 2]].normal = Normal;
	}*/

	//end timer
	inMesh->UseCollision = true;

	GetMeshCorners(inMesh);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time to generate sphere: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

void MeshGenerator::GeneratePoolTable(Mesh* floorMesh, Mesh* topWall, Mesh* bottomWall, Mesh* rightWall, Mesh* leftWall,
                                      glm::vec3 size)
{
	float floorHeight = 0.1f;
	GenerateCubeWithHardEdges(floorMesh, glm::vec3(size.x, floorHeight/2, size.z));
	floorMesh->transform.SetLocation(glm::vec3(0.0f, -floorHeight/2, 0.0f));

	float wallWidth = 0.1f;

	GenerateCubeWithHardEdges(rightWall, glm::vec3(wallWidth, size.y, size.z));
	rightWall->transform.SetLocation(glm::vec3(size.x/2 + wallWidth/2, 0.0f + size.y/2, 0.0f));

	GenerateCubeWithHardEdges(leftWall, glm::vec3(wallWidth, size.y, size.z));
	leftWall->transform.SetLocation(glm::vec3(-size.x/2 - wallWidth / 2, 0.0f + size.y / 2 , 0.0f));

	GenerateCubeWithHardEdges(topWall, glm::vec3(size.x, size.y, wallWidth));
	topWall->transform.SetLocation(glm::vec3(0.0f, 0.0f + size.y / 2, size.z/2 + wallWidth/2));

	GenerateCubeWithHardEdges(bottomWall, glm::vec3(size.x, size.y, wallWidth));
	bottomWall->transform.SetLocation(glm::vec3(0.0f, 0.0f + size.y / 2, -size.z/2 - wallWidth/2));

}

void MeshGenerator::ToUnitSphere(Mesh* inMesh)
{
		for (auto& vertex : inMesh->vertices)
		{
			vertex.position = glm::normalize(vertex.position);
		}
}

void MeshGenerator::Subdivide(Vertex a, Vertex b, Vertex c, int n, Mesh* inMesh)
{
	if (n > 0)
	{
		glm::vec3 v1 = glm::normalize(a.position + b.position);
		glm::vec3 v2 = glm::normalize(a.position + c.position);
		glm::vec3 v3 = glm::normalize(c.position + b.position);
		Subdivide(a, Vertex(v1), Vertex(v2), n - 1, inMesh);
		Subdivide(c,Vertex(v2), Vertex(v3), n - 1, inMesh);
		Subdivide(b, Vertex(v3), Vertex(v1), n - 1, inMesh);
		Subdivide(Vertex(v3), Vertex(v2), Vertex(v1), n - 1, inMesh);
	} else
	{
		a.normal = glm::normalize(a.position);
		b.normal = glm::normalize(b.position);
		c.normal = glm::normalize(c.position);

		inMesh->vertices.push_back(a);
		inMesh->vertices.push_back(b);
		inMesh->vertices.push_back(c);
		
	}
}

void MeshGenerator::GetMeshCorners(Mesh* inMesh)
{
	//Find all corners of the mesh
	float MaxX = -FLT_MAX;
	float MaxY = -FLT_MAX;
	float MaxZ = -FLT_MAX;

	float MinX = FLT_MAX;
	float MinY = FLT_MAX;
	float MinZ = FLT_MAX;


	for (auto vertex: inMesh->vertices)
	{
		//Find the min and max of the x, y and z values

		MaxX = glm::max(MaxX, vertex.position.x);
		MaxY = glm::max(MaxY, vertex.position.y);
		MaxZ = glm::max(MaxZ, vertex.position.z);

		MinX = glm::min(MinX, vertex.position.x);
		MinY = glm::min(MinY, vertex.position.y);
		MinZ = glm::min(MinZ, vertex.position.z);

	}

	std::vector<glm::vec3> corners;
	corners.resize(8);

	corners = {
		glm::vec3(MinX, MinY, MinZ),
		glm::vec3(MaxX, MinY, MinZ),
		glm::vec3(MaxX, MaxY, MinZ),
		glm::vec3(MinX, MaxY, MinZ),
		glm::vec3(MinX, MinY, MaxZ),
		glm::vec3(MaxX, MinY, MaxZ),
		glm::vec3(MaxX, MaxY, MaxZ),
		glm::vec3(MinX, MaxY, MaxZ)
	};
	inMesh->MeshCorners = corners;
}

void MeshGenerator::GetMeshCorners(MeshBase* inMesh)
{
	//Find all corners of the mesh
	float MaxX = -FLT_MAX;
	float MaxY = -FLT_MAX;
	float MaxZ = -FLT_MAX;

	float MinX = FLT_MAX;
	float MinY = FLT_MAX;
	float MinZ = FLT_MAX;


	for (auto vertex : inMesh->vertices)
	{
		//Find the min and max of the x, y and z values

		MaxX = glm::max(MaxX, vertex.position.x);
		MaxY = glm::max(MaxY, vertex.position.y);
		MaxZ = glm::max(MaxZ, vertex.position.z);

		MinX = glm::min(MinX, vertex.position.x);
		MinY = glm::min(MinY, vertex.position.y);
		MinZ = glm::min(MinZ, vertex.position.z);

	}

	std::vector<glm::vec3> corners;
	corners.resize(8);

	corners = {
		glm::vec3(MinX, MinY, MinZ),
		glm::vec3(MaxX, MinY, MinZ),
		glm::vec3(MaxX, MaxY, MinZ),
		glm::vec3(MinX, MaxY, MinZ),
		glm::vec3(MinX, MinY, MaxZ),
		glm::vec3(MaxX, MinY, MaxZ),
		glm::vec3(MaxX, MaxY, MaxZ),
		glm::vec3(MinX, MaxY, MaxZ)
	};
	inMesh->MeshCorners = corners;
};

