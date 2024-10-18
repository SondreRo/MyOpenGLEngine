#include "MovementSystem.h"
#include "Application.h"
void MovementSystem::MoveEntity(Entity entity, float DeltaTime)
{
	// Get the movement component
	unsigned int MovementID = MovementHandler->entityToComponentMap[entity.ID];
	unsigned int TransformID = TransformHandler->entityToComponentMap[entity.ID];

	////Get random float between -.5 and .5
	

	//// Set the velocity
	//MovementHandler->components[MovementID].Velocity += glm::vec3(randomx, randomy, randomz);

	glm::vec3 entityLoc = TransformHandler->components[TransformID].transform.GetLocation();


	
	glm::vec3 camLoc = Application::get().mScene.camera->cameraPos;
	//std::cout << "CamLoc: " << camLoc.x << ", " << camLoc.y << ", " << camLoc.z << "\n";

	//Application::get().mScene.camera->transform.GetLocation();




	glm::vec3 direction = camLoc - entityLoc;
	direction.y = 0;
	direction = glm::normalize(direction);
	direction *= 2.f;

	MovementHandler->components[MovementID].Velocity = direction;

	// Move the entity

	TransformHandler->components[TransformID].transform.AddLocation(MovementHandler->components[MovementID].Velocity * DeltaTime);

	// Print the location
	//glm::vec3 Location = TransformHandler->components[TransformID].transform.GetLocation();
	//std::cout << "Entity: " << entity.ID << " Location: " << Location.x << ", " << Location.y << ", " << Location.z << "\n";
}
