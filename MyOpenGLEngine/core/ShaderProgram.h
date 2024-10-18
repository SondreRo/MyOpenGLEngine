#pragma once
#include <string>
#include "glm/glm.hpp"
class ShaderProgram 
{
public:
	int ReadShaderFile(const std::string& VertexPath, const std::string& FragmentPath);
	int CompileShaders();
	unsigned int CreateProgram();
	void UseProgram();

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, glm::vec3 value);
	void SetUniformMat4(const std::string& name, glm::mat4 value);


	std::string vertexCode;
	std::string fragmentCode;
	unsigned int fragmentShader;
	unsigned int vertexShader;

	unsigned int shaderProgram;
};