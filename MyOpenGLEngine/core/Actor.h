#pragma once
#include <string>
#include <vector>

class Actor {
	std::string name; // Used as a key in maps, this must be unique

	std::vector<class ActorComponent*> components; // Holds all components this actor has

public:
	Actor(std::string inName);
	virtual ~Actor();

	virtual void Update(float DeltaTime);
	virtual void RenderProperties();








	// Cleanup
	void CleanUp();
};