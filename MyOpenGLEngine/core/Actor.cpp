#include "Actor.h"
#include "ActorComponent.h"

void Actor::CleanUp()
{
	for (auto component : components)
	{
		delete component;
	}
	components.clear();
}