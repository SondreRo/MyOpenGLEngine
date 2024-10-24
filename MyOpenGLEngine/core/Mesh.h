#pragma once
#include <vector>

#include "ShaderProgram.h"
#include "Vertex.h"
#include "Transform.h"
#include "Material.h"
#include "Collision/Colliders.h"
#include <queue>


class Mesh {
public:
	Mesh() = default;
	Mesh(std::string inName);
	~Mesh();

	// Public Tools
	virtual void Draw();
	virtual void Update(float DeltaTime);
	virtual void Bind();
	virtual void Rebind();
	virtual void Unbind();

	virtual void RenderProperties();

	// Mesh Data
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	bool ReadingFirst = true;
	std::queue<Vertex> incomming_vertices;
	std::queue<Vertex> incomming_vertices2;


	// Render Data
	Material material;
	ShaderProgram* shaderProgram;
	bool renderDots = false;
	float DotsSize = 1.0f;
	bool NormalAsColor = false;
	bool isBound = false;
	bool BindDynamic = false;
	bool HasBoundOnce = false;

	// Transform Data
	Transform transform;
	glm::mat4 ParentMatrix = glm::mat4(1);
	glm::mat4 Transformmat = glm::mat4(1);
	glm::mat4 GetTransformMat();


	glm::mat4 GetGlobalTransform();

	// Collision Data
	bool UseCollision = false;
	bool Static = false;
	std::vector<glm::vec3> MeshCorners;
	AxisAlignedBoundingBox GetAABB();
	void UpdateAABB();
	AxisAlignedBoundingBox aabb;

	// SceneGraph Data
	std::vector<Mesh*> Children;
	void AddChild(Mesh* mesh);
	Mesh* Parent = nullptr;

private:

protected:
	unsigned int VAO, VBO, EBO;

};