#pragma once
#include <vector>

#include "glm/vec3.hpp"
#include "Mesh.h"

struct ParticleInfo
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> velocity;
};

class ParticleSystem {
public:
	ParticleInfo particles;
	unsigned int ParticleCount = 0;

	unsigned int MaxParticles = 1000;
	float Timer = 0;

	glm::vec3 Wind = glm::vec3(0, 0, 0);
	float Gravity = -9.8f * 0.1f;

	glm::vec3 SpawnMin = glm::vec3(-10, 20, -10);
	glm::vec3 SpawnMax = glm::vec3(10, 25, 10);
	int spawnRate = 10;

	void AddParticle(glm::vec3 position, glm::vec3 velocity);
	void RemoveParticle(unsigned int index);
	void ReserveParticles(unsigned int count);
	void ClearParticles();

	void Update(float deltaTime);

	void Draw();

	void GenerateStartingParticles(int Amount);
	void SpawnSnowParticle();
	static float RandomFloat(float min, float max);

	void DrawDebugMenu();

	Mesh* ParticleMesh;
};