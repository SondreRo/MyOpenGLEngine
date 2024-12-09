#include "ECSManager.h"

// Components
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/MeshComponent.h"
#include "Components/CombatComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereColliderComponent.h"
#include "Components/ColorComponent.h"

// Systems
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/AICombatSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/ColorSystem.h"

// Extra
#include "Application.h"
#include "MeshGenerator.h"
#include "Window.h"


ECSManager::ECSManager()
{
	BoxMesh = new MeshBase();
	//MeshGenerator::GenerateSphere(BoxMesh, 1, 10, 10);
	MeshGenerator::GenerateIcosahedron(BoxMesh, 3);
	//MeshGenerator::GenerateCubeWithHardEdges(BoxMesh, glm::vec3(1.f));



}


void ECSManager::Setup()
{

	
	dod_components_map["ColorComponent"] = new DODColorComponentInternal();

	ShaderProgram* newShader = Application::get().mScene.Shaders["DefaultShader"];
	BoxMesh->shaderProgram = newShader;
	BoxMesh->material.diffuse = glm::vec3(1, 0, 0);


	/*MeshBase* PlayerMesh = new MeshBase();
	MeshGenerator::GenerateCubeWithHardEdges(PlayerMesh, glm::vec3(2.f));
	PlayerMesh->shaderProgram = newShader;
	PlayerMesh->material.diffuse = glm::vec3(0, 0, 1);*/

	//// Create Entities
	//Entity* Player = CreateEntity();
	//Player->AddComponent<TransformComponent>(new TransformComponent(glm::vec3(0, 1, -50)));
	//Player->AddComponent<VelocityComponent>(new VelocityComponent(glm::vec3(0, 0, 0)));
	//Player->AddComponent<MeshComponent>(new MeshComponent(PlayerMesh));
	//Player->AddComponent<InputComponent>(new InputComponent());
	//Player->AddComponent<CombatComponent>(new CombatComponent(100, 10, nullptr));



	for (int i = 0; i < 20; i++)
	{
		// Random float between -50 and 50
		float RandX = rand() % 100 - 50;
		float RandZ = rand() % 100 - 50;

		CreateEnemy({RandX, 1, RandZ});
		
	}
  



}

void ECSManager::SystemSetup()
{
	Systems.emplace_back(new MovementSystem());
	Systems.emplace_back(new RenderSystem());
	Systems.emplace_back(new AICombatSystem());
	Systems.emplace_back(new CollisionSystem());
	
	//Systems.emplace_back(new ColorSystem(static_cast<DODColorComponentInternal*>(dod_components_map["ColorComponent"])));
	Systems.emplace_back(new ColorSystem((DODColorComponentInternal*)(dod_components_map["ColorComponent"])));


	for (auto& system : Systems)
	{
		for (auto& entity : Entities)
		{
			system->Setup(entity, this);
		}
	}
}

void ECSManager::Update(float DeltaTime)
{
	for (auto& system : Systems)
	{
		for (auto& entity : Entities)
		{
			system->Update(entity, DeltaTime);
		}
	}

	if (!EntitiesToDelete.empty())
	{
		for (auto entity : EntitiesToDelete)
		{
			if (!entity) continue;

			if (InputComponent* IC = entity->GetComponent<InputComponent>())
			{
				std::cout << "Player died" << std::endl;
				std::cout << "Quitting Game" << std::endl;
			}

			Entities.erase(std::remove(Entities.begin(), Entities.end(), entity), Entities.end());
			delete entity;
			CreateEnemy(glm::vec3(rand() % 100 - 50, 1, (rand() % 100 - 50)));
		}
		EntitiesToDelete.clear();

		
	}

	
}

void ECSManager::Render()
{
	for (auto& system : Systems)
	{
		for (auto& entity : Entities)
		{
			system->Render(entity);
		}
	}
}

void ECSManager::CreateEnemy(glm::vec3 Position)
{
	Entity* E1 = CreateEntity();



	E1->AddComponent<TransformComponent>(new TransformComponent(Position));

	//Random float from -5 to 5
	float RandVelX = (rand() % 10 - 5) / 10.f;
	float RandVelZ = (rand() % 10 - 5) / 10.f;

	RandVelX *= 10;
	RandVelZ *= 10;


	E1->AddComponent<VelocityComponent>(new VelocityComponent(glm::vec3(RandVelX, 0, RandVelZ)));
	E1->AddComponent<MeshComponent>(new MeshComponent(BoxMesh));
	E1->AddComponent<SphereColliderComponent>(new SphereColliderComponent(1.0f));

	E1->AddComponent<ColorComponent>(new ColorComponent((DODColorComponentInternal*)(dod_components_map["ColorComponent"]), glm::vec3(0, 1, 0)));


	//E1->AddComponent<CombatComponent>(new CombatComponent(100, 10, Entities[0]));

}

void ECSManager::DeleteEntity(Entity* entity)
{
	EntitiesToDelete.emplace_back(entity);
}

void ECSManager::MouseMoveCallback(Window* window, double xpos, double ypos)
{
	
}

void ECSManager::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		for (auto SystemB : Systems)
		{
			for (auto Entity : Entities)
			{
				if (InputComponent* IC = Entity->GetComponent<InputComponent>())
				{
					SystemB->MouseButtonCallback(Entity);

				}
			}
		}
	}
}

void ECSManager::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{

}

void ECSManager::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{

}

void ECSManager::ProcessInput(Window* window, float DeltaTime)
{
	float Speed = 40;
	for (auto entity : Entities)
	{
		if (InputComponent* IC = entity->GetComponent<InputComponent>())
		{
			if (VelocityComponent* VC = entity->GetComponent<VelocityComponent>())
			{
				if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_UP) == GLFW_PRESS)
					VC->Velocity.z -= Speed * DeltaTime;

				if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
					VC->Velocity.z += Speed * DeltaTime;

				if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
					VC->Velocity.x -= Speed * DeltaTime;

				if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
					VC->Velocity.x += Speed * DeltaTime;

				//VC->Velocity *= 0.99f;
			}
				
		}
	}
}

Entity* ECSManager::CreateEntity()
{

	Entities.emplace_back(new Entity());
	return Entities.back();
}
