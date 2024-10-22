#include <ShaderProgram.h>

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

int ShaderProgram::ReadShaderFile(const std::string& VertexPath, const std::string& FragmentPath)
{
	std::cout << "Reading Vertex ShaderFile < " << VertexPath << " >" << std::endl;
	std::cout << "Reading Fragment ShaderFile < " << FragmentPath << " >" << std::endl;

	std::fstream ShaderVertexFile, ShaderFragmentFile;
	ShaderVertexFile.open(VertexPath, std::ios::in);
	ShaderFragmentFile.open(FragmentPath, std::ios::in);

	std::stringstream VertexStream;
	if (ShaderVertexFile.is_open())
	{
		VertexStream << ShaderVertexFile.rdbuf();
		ShaderVertexFile.close();
		vertexCode = VertexStream.str();
	}
	else
	{
		std::cout << "Could not open file"  << std::endl;
		return 0;
	}
	std::stringstream FragmentStream;

	if (ShaderFragmentFile.is_open())
	{
		FragmentStream << ShaderFragmentFile.rdbuf();
		ShaderFragmentFile.close();
		fragmentCode = FragmentStream.str();
	}
	else
	{
		std::cout << "Could not open file" << std::endl;
		return 0;
	}

	return 1;
}

int ShaderProgram::CompileShaders()
{
	int  success;
	char infoLog[512];


	// VERTEX SHADER;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char* vertexShaderSource = vertexCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// FRAGMENT SHADER;
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragmentShaderSource = fragmentCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return 1;
}

unsigned ShaderProgram::CreateProgram()
{
	int  success;
	char infoLog[512];


	
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return 1;
}

void ShaderProgram::UseProgram()
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::SetUniform3f(const std::string& name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniformMat4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));

}
