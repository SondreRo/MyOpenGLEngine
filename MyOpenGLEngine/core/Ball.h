#pragma once
#include "Mesh.h"

class Ball : public Mesh {
public:
	Ball(std::string inName);
	~Ball();


	virtual void Update(float DeltaTime) override;
	virtual void RenderProperties() override;

	glm::vec3 velocity = glm::vec3(0);

	
private:
	int SubCount = 0;
};