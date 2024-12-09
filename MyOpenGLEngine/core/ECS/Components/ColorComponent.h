#pragma once
#include <vector>
#include "ComponentBase.h"

struct DODColorComponentInternal : public DODComponentBaseInternal
{
	DODColorComponentInternal()
	{
		r = std::vector<float>();
		g = std::vector<float>();
		b = std::vector<float>();
	}

	std::vector<float> r;
	std::vector<float> g;
	std::vector<float> b;

	unsigned int AddColor(float R, float G, float B)
	{
		r.push_back(R);
		g.push_back(G);
		b.push_back(B);
		return r.size() - 1;
	}

	void RemoveColor(unsigned int index)
	{
		r.erase(r.begin() + index);
		g.erase(g.begin() + index);
		b.erase(b.begin() + index);
	}
};

class ColorComponent : public DODComponentBase
{
public:
	ColorComponent(DODColorComponentInternal* inColorComponent, glm::vec3 inColor)
	{
		colorComponent = inColorComponent;
		index = colorComponent->AddColor(inColor.r, inColor.g, inColor.b);
	}
	DODColorComponentInternal* colorComponent;
	//unsigned int index;
};