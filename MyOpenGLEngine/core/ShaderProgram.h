#pragma once
#include <string>

class ShaderProgram 
{
public:
	int ReadShaderFile(const std::string& VertexPath, const std::string& FragmentPath);
	int CompileShaders();
	unsigned int CreateProgram();
	void UseProgram();

	std::string vertexCode;
	std::string fragmentCode;
	unsigned int fragmentShader;
	unsigned int vertexShader;

	unsigned int shaderProgram;
};