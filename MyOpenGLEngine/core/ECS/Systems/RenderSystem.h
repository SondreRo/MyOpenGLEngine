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
				MC->Mesh->Draw();
			}
		}
	}
};