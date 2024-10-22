#pragma once
#include <vector>

#include "Vertex.h"
#include "glm/glm.hpp"

class BSplineSurface
{
private:

public:

	BSplineSurface();
	static float bSplineBasis2(int i, float t, int m);
	static glm::vec3 evaluateBSplineSurface(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints);
	static std::vector<glm::vec3> generateBSplineMesh(const std::vector<std::vector<glm::vec3>>& controlPoints, int resolutionU, int resolutionV);
	static std::vector<unsigned int> triangulate(int resolutionU, int resolutionV);
};