#pragma once
#include "SystemBase.h"

#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"


class RenderSystem : public SystemBase
{
public:
	void Setup(Entity* entity, ECSManager* manager) override
	{
		SystemBase::Setup(entity, manager);
	}
	void Update(Entity* entity, float DeltaTime) override
	{
		
	}
	void Render(Entity* entity) override {
		if (MeshComponent* MC = entity->GetComponent<MeshComponent>())
		{
			if (TransformComponent* TC = entity->GetComponent<TransformComponent>())
			{
				MC->Mesh->shaderProgram->SetUniformMat4("model", TC->transform.GetMatrix());

				if (ColorComponent* CC = entity->GetComponent<ColorComponent>())
				{
					MC->Mesh->material.diffuse = glm::vec3(CC->colorComponent->r[CC->index], CC->colorComponent->g[CC->index], CC->colorComponent->b[CC->index]);
					//MC->Mesh->shaderProgram->SetUniform3f("u_Color", glm::vec3(CC->colorComponent->r[CC->index], CC->colorComponent->g[CC->index], CC->colorComponent->b[CC->index]));
				}
				else
				{
					MC->Mesh->material.diffuse = glm::vec3(1, 1, 1);

					//MC->Mesh->shaderProgram->SetUniform3f("u_Color", glm::vec3(1, 1, 1));
				}


				MC->Mesh->Draw();
			}
		}
	}
};