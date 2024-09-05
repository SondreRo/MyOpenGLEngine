#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
struct Transform
{

	// Getters
	glm::mat4 GetMatrix();

	glm::vec3 GetLocation();
	glm::vec3 GetScale();
	glm::vec3 GetRotation();

	// Setters
	void SetLocation(glm::vec3 inLocation);
	void SetScale(glm::vec3 inScale);
	void SetRotation(glm::vec3 inRotation);

	void SetLocationX(float inX);
	void SetLocationY(float inY);
	void SetLocationZ(float inZ);


	glm::vec3 GetUpVector();
	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	// Adders
	void AddLocation(glm::vec3 inLocation);
	void AddScale(glm::vec3 inScale);
	void AddRotation(glm::vec3 inRotation);
private:

	glm::vec3 Location = glm::vec3(0);
	glm::vec3 Scale = glm::vec3(1);

	float Pitch = 0;
	float Yaw = 0;
	float Roll = 0;

	glm::quat orientation = glm::quat(0, 0, 0, 0);
};