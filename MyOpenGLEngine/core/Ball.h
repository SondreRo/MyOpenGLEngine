#pragma once
#include "Mesh.h"

class Ball : public Mesh {
public:
	Ball(std::string inName);
	~Ball();


	virtual void Update(float DeltaTime) override;
	virtual void RenderProperties() override;

	glm::vec3 velocity = glm::vec3(0);
	float Radius = 1.f;
	SphereCollider sphereCollider;
	SphereCollider GetSphereCollider();


	std::vector<glm::vec3> LinePoints;
	std::vector<Vertex> TracePoints;

	glm::vec3 SpawnLocation;
	bool FirstUpdate = true;

	void Resetball();

	int TestVariable;


	
private:
	int SubCount = 0;
};