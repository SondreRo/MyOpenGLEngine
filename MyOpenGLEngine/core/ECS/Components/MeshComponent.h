#pragma once
#include "ComponentBase.h"
#include "meshbase.h"
class MeshComponent : public ComponentBase
{
public:


	MeshComponent() = default;
	MeshComponent(MeshBase* mesh)
	{
		Mesh = mesh;
	}

	MeshBase* Mesh = nullptr;

};