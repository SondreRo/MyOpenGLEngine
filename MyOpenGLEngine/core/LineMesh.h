#pragma once
#include "Mesh.h"

class LineMesh : public Mesh {

public:
	LineMesh(std::string inName);

	virtual void Draw() override;
	void AddLine(glm::vec3 start, glm::vec3 end);
	void AddLine(Vertex start, Vertex end);

	void AddBox(glm::vec3 Location, glm::vec3 Size);
	void AddBoxMinMax(glm::vec3 min, glm::vec3 max);

	void Rebind();

	float LineWith = 5.f;

	virtual void RenderProperties() override;
};