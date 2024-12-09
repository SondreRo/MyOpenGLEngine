#pragma once
#include "SystemBase.h"
#include "../Components/ColorComponent.h"

class ColorSystem : public SystemBase
{
	DODColorComponentInternal* dodInternal;
public:

	float Time = 0.f;

	ColorSystem();
	ColorSystem(DODColorComponentInternal* inColorComponent)
	{
		dodInternal = inColorComponent;
	}

	virtual void Setup(Entity* entity, ECSManager* manager) override
	{
		SystemBase::Setup(entity, manager);
	}
	virtual void Update(Entity* entity, float DeltaTime) override
	{
		Time += DeltaTime / 10;
		if (dodInternal) 
		{

			for (int i = 0; i < dodInternal->r.size(); i++) 
			{
				// Randomfloat between 0 and 1
		/*		float randomR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randomG = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float randomB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);*/

				//unsigned int index = entity->GetComponent<ColorComponent>()->index;

				dodInternal->r[i] = cos(Time + (i * 3.1)) * 0.34;
				dodInternal->g[i] = sin(Time + (i * 2.2)) * 0.22 + 1;
				dodInternal->b[i] = cos(Time + (i * 1.3)) * 2 + 0.5;
			}

			
		}
	}





};