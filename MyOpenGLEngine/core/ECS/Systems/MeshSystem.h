#pragma once
#include "SystemBase.h"
#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"
class MeshSystem : public SystemBase
{
public:
	
	ComponentHandler<MeshComponent>* MeshHandler;
	ComponentHandler<TransformComponent>* TransformHandler;

	void AddMesh(Entity entity, std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material, ShaderProgram* shaderProgram)
	{
		unsigned int MeshID = MeshHandler->entityToComponentMap[entity.ID];
		MeshComponent& mesh = MeshHandler->components[MeshID];

		mesh.vertices = vertices;
		mesh.indices = indices;
		mesh.material = material;
		mesh.shaderProgram = shaderProgram;
	}

	void BindAllEntities() 
	{
		for (auto& component : MeshHandler->components)
		{
			// VAO
			glGenBuffers(1, &component.VBO);

			// VAO
			glGenVertexArrays(1, &component.VAO);
			glBindVertexArray(component.VAO);

			// VBO
			glGenBuffers(1, &component.EBO);


			glBindBuffer(GL_ARRAY_BUFFER, component.VBO);
			glBufferData(GL_ARRAY_BUFFER, component.vertices.size() * sizeof(Vertex), component.vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, component.indices.size() * sizeof(unsigned int), component.indices.data(), GL_STATIC_DRAW);

			Vertex::BindAttributes();
		}
	}

	void RenderAllEntities()
	{
		for (auto& meshIndex : MeshHandler->entityToComponentMap)
		{
			unsigned int MeshID = meshIndex.second;
			unsigned int TransformID = TransformHandler->entityToComponentMap[meshIndex.first];

			// Get the mesh component
			MeshComponent& mesh = MeshHandler->components[MeshID];
			TransformComponent& transform = TransformHandler->components[TransformID];

			// Render the mesh
			mesh.shaderProgram->UseProgram();
			mesh.shaderProgram->SetUniformMat4("model", transform.transform.GetMatrix());
            mesh.material.BindMaterial(mesh.shaderProgram);
			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			//glBindVertexArray(0);
		}
	}

};