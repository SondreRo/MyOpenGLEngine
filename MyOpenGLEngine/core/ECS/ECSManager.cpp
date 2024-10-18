#include "ECSManager.h"
#include "ComponentHandler/ComponentHandler.h"

// Components
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/HealthComponent.h"

// Extra
#include "MeshGenerator.h"
#include "Application.h"

ECSManager::ECSManager()
{
	Movement_System = MovementSystem();
}

void ECSManager::Setup()
{
	// Create Entities
    Entity E1 = CreateEntity();
    Entity E2 = CreateEntity();
	Entity E3 = CreateEntity();

	
	// Add Components
	AddComponent<TransformComponent>(E1);
	AddComponent<TransformComponent>(E2);
	AddComponent<TransformComponent>(E3);

	AddComponent<MovementComponent>(E1, glm::vec3(1.0f, 0.0f, -1.0f));
	AddComponent<MovementComponent>(E2, glm::vec3(5.0f, 0.0f, 0.0f));
	AddComponent<MovementComponent>(E3, glm::vec3(0.0f, 0.0f, -3.0f));

	AddComponent<MeshComponent>(E1);
	AddComponent<MeshComponent>(E2);
	AddComponent<MeshComponent>(E3);

	AddComponent<HealthComponent>(E1, 100);
	AddComponent<HealthComponent>(E2, 100);
	AddComponent<HealthComponent>(E3, 100);

	AddComponent<CombatComponent>(E1);
	AddComponent<CombatComponent>(E2);
	AddComponent<CombatComponent>(E3);


	// Make sure the systems has the correct handlers
    Movement_System.MovementHandler = GetComponentHandler<MovementComponent>();
	Movement_System.TransformHandler = GetComponentHandler<TransformComponent>();

    Movement_System.TransformHandler = GetComponentHandler<TransformComponent>();
	Mesh_System.TransformHandler = GetComponentHandler<TransformComponent>();
	Mesh_System.MeshHandler = GetComponentHandler<MeshComponent>();

	combat_system.TransformHandler = GetComponentHandler<TransformComponent>();
	combat_system.HealthHandler = GetComponentHandler<HealthComponent>();
	combat_system.CombatHandler = GetComponentHandler<CombatComponent>();

	

	Mesh* Mesh1 = new Mesh();
	MeshGenerator::GenerateCubeWithHardEdges(Mesh1, glm::vec3(1));

	Mesh_System.AddMesh(E1, Mesh1->vertices, Mesh1->indices, Material(), Application::get().mScene.Shaders["DefaultShader"]);
	Mesh_System.AddMesh(E2, Mesh1->vertices, Mesh1->indices, Material(), Application::get().mScene.Shaders["DefaultShader"]);
	Mesh_System.AddMesh(E3, Mesh1->vertices, Mesh1->indices, Material(), Application::get().mScene.Shaders["DefaultShader"]);


	// For loop creating 100 entities
	for (int i = 0; i < 3000; i++)
	{
		CreateEnemy();
	}

	delete Mesh1;

	Mesh_System.BindAllEntities();
}

void ECSManager::CreateEnemy()
{
	Entity E = CreateEntity();


	// Random float from -1 to 1
	float RandomX = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
	float RandomY = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
	float RandomZ = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
	AddComponent<TransformComponent>(E, glm::vec3(RandomX * 100, 0, RandomZ * 100));

	AddComponent<MovementComponent>(E, glm::vec3(RandomX, 0, RandomZ));
	AddComponent<MeshComponent>(E);
	AddComponent<HealthComponent>(E, 100);
	AddComponent<CombatComponent>(E);


	Mesh* Mesh1 = new Mesh();
	MeshGenerator::GenerateCubeWithHardEdges(Mesh1, glm::vec3(1));

	Mesh_System.AddMesh(E, Mesh1->vertices, Mesh1->indices, Material(), Application::get().mScene.Shaders["DefaultShader"]);

	delete Mesh1;
}

void ECSManager::Update(float DeltaTime)
{
	for (auto& entity : Entities)
	{
		Movement_System.MoveEntity(entity, DeltaTime);
	}
	//Mesh_System.RenderAllEntities();
	combat_system.AttackAll(Entities[0]);
}

void ECSManager::Render()
{
	Mesh_System.RenderAllEntities();
}

void ECSManager::RemoveEntity(Entity& entity)
{
	// remove from enteties vector
	//Entities.erase(Entities.begin() + entity.ID);
	
	//
}

Entity& ECSManager::CreateEntity()
{
	Entities.emplace_back();

	Entities.back().ID = Entities.size() - 1;

	return Entities.back();
}

const std::unordered_map<std::string, HandlerInterface*>& ECSManager::GetComponentMap()
{
	return ComponentHandlers;
}


template<typename T, typename ...Args>
void ECSManager::AddComponent(Entity& entity, Args && ...args)
{
	std::string Component = typeid(T).name();

	// Ensure that the type T can be constructed with Args...
	static_assert(std::is_constructible<T, Args&&...>::value,
		"Error: T cannot be constructed with the provided arguments.");

	//std::cout << "Creating a " << Component << std::endl;
	if (ComponentHandlers.contains(Component))
	{
		//std::cout << "Map Already has this type of handler \n";

		ComponentHandler<T>* ComHandler = GetComponentHandler<T>();
		ComHandler->components.emplace_back(std::forward<Args>(args)...);
		ComHandler->entityToComponentMap[entity.ID] = ComHandler->components.size() - 1;

	 	//return ComHandler.components.back();
	}
	else
	{
		//std::cout << "Component NOT Found in map, creating a new one\n";

		ComponentHandlers[Component] = new ComponentHandler<T>();
		auto Handler = static_cast<ComponentHandler<T>*>(ComponentHandlers[Component]);
		Handler->components.emplace_back(std::forward<Args>(args)...);
		Handler->entityToComponentMap[entity.ID] = Handler->components.size() - 1;
	}
	//std::cout << "Component Added " << Component << " There is now " << GetComponentHandler<T>()->components.size() << " components in the map\n";
}

template <typename T>
ComponentHandler<T>* ECSManager::GetComponentHandler()
{
	std::string Component = typeid(T).name();
	if (ComponentHandlers.contains(Component))
	{
		return static_cast<ComponentHandler<T>*>(ComponentHandlers[Component]);
	}
	else
	{
		std::cout << "Component: " << Component << " not found \n";
		// Throw an exception or return a default value here to prevent undefined behavior
		throw std::runtime_error("Component not found");
	}
}
