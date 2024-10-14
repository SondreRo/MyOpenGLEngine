#pragma once

class EntityComponentSystemManager {
public:
	EntityComponentSystemManager();
	~EntityComponentSystemManager();

	void Update(float deltaTime);
};