#pragma once
#include <vector>
#include <unordered_map>
#include "HandlerInterface.h"

template <typename T>
class ComponentHandler : public HandlerInterface
{
public:
	std::vector<T> components;
	std::unordered_map<unsigned int, unsigned int> entityToComponentMap;
};