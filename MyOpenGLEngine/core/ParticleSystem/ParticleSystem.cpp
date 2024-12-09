#include "ParticleSystem.h"
#include <imgui.h>

#include "Application.h"
#include "glm/gtc/type_ptr.hpp"

void ParticleSystem::AddParticle(glm::vec3 position, glm::vec3 velocity)
{
	if (ParticleCount >= MaxParticles) return;


	ParticleCount++;
	particles.position.emplace_back(position);
	particles.velocity.emplace_back(velocity);
}

void ParticleSystem::RemoveParticle(unsigned int index)
{
	ParticleCount--;
	particles.position.erase(particles.position.begin() + index);
	particles.velocity.erase(particles.velocity.begin() + index);
}

void ParticleSystem::ReserveParticles(unsigned int count)
{
	ParticleCount = count;
	particles.position.reserve(count);
	particles.velocity.reserve(count);
}

void ParticleSystem::ClearParticles()
{
	ParticleCount = 0;
	particles.position.clear();
	particles.velocity.clear();
}

void ParticleSystem::Update(float deltaTime)
{
		
	Timer += deltaTime;

	for (unsigned int i = 0; i < ParticleCount; i++)
	{
		particles.position[i] += particles.velocity[i] * deltaTime;
		particles.velocity[i].y += Gravity * deltaTime;
		particles.velocity[i] += glm::vec3(RandomFloat(-2.f, 2.f), RandomFloat(-2.f, 2.f), RandomFloat(-2.f, 2.f)) * deltaTime;
		particles.velocity[i] += Wind * deltaTime;

		if (particles.position[i].y < 0)
		{
			RemoveParticle(i);
			//SpawnSnowParticle();
		}
	}

	if (Timer > 0.1f)
	{
		Timer = 0;
		if (ParticleCount < MaxParticles)
		{
			for (int i = 0; i < spawnRate; i++)
			{
				SpawnSnowParticle();
			}
		}
	}
}

void ParticleSystem::Draw()
{
	if (ParticleMesh == nullptr) return;
	if (particles.position.empty()) return;
	if (ParticleCount <= 0) return;

	for (unsigned int i = 0; i < ParticleCount; i++)
	{
		//ParticleMesh->transform.SetLocation(particles.position[i]);
		//Application::get().mScene.ReadyRenderData(ParticleMesh->shaderProgram, ParticleMesh, ParticleMesh->ParentMatrix, Application::get().mScene.width, Application::get().mScene.height);
		// Mesh location

		ParticleMesh->transform.SetLocation(particles.position[i]);
		//ParticleMesh->transform.SetRotation(particles.velocity[i]);
		//ParticleMesh->transform.SetScale(particles.velocity[i]);

		glUniformMatrix4fv(glGetUniformLocation(ParticleMesh->shaderProgram->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(ParticleMesh->transform.GetMatrix()));
		//Mesh Color
		glUniform3fv(glGetUniformLocation(ParticleMesh->shaderProgram->shaderProgram, "diffuse"), 1, glm::value_ptr(ParticleMesh->material.diffuse));


		ParticleMesh->Draw();
		// Draw All Particles
	}

}

void ParticleSystem::GenerateStartingParticles(int Amount)
{
	srand(time(NULL));
	//ReserveParticles(Amount);
	for (int i = 0; i < Amount; i++)
	{
		SpawnSnowParticle();
	}
}

void ParticleSystem::SpawnSnowParticle()
{
	glm::vec3 RandomPosition = glm::vec3(
		RandomFloat(SpawnMin.x, SpawnMax.x),
		RandomFloat(SpawnMin.y, SpawnMax.y),
		RandomFloat(SpawnMin.z, SpawnMax.z)
	);


	glm::vec3 RandomVelocity = glm::vec3(
		RandomFloat(-2, 2),
		RandomFloat(-2, 0),
		RandomFloat(-2, 2)
	);


	AddParticle(RandomPosition, RandomVelocity);
}

float ParticleSystem::RandomFloat(float min, float max)
{
	return ((rand() % 100) / 100.0f) * (max - min) + min;
}

void ParticleSystem::DrawDebugMenu()
{

	ImGui::Begin("Particle System");

	ImGui::Text("Particle Count: %d", ParticleCount);

	int MaxParticleI = MaxParticles;
	ImGui::DragInt("Max Particles", &MaxParticleI, 1.0f, 0, 10000);
	MaxParticles = MaxParticleI;

	ImGui::DragFloat3("Wind", glm::value_ptr(Wind), 0.05f);
	ImGui::DragFloat("Gravity", & Gravity, 0.05f);

	ImGui::DragFloat3("Spawn Min", glm::value_ptr(SpawnMin), 0.05f);
	ImGui::DragFloat3("Spawn Max", glm::value_ptr(SpawnMax), 0.05f);
	ImGui::DragInt("Spawn Rate", &spawnRate, 1.0f, 0, 100);


	ImGui::End();
}
