#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

#include "shader.h"

using namespace Engine;

Shader::Shader(const char* vPath, const char* fPath)
{
	std::ifstream vFile(vPath);
	std::ifstream fFile(fPath);

	if (!vFile.is_open())
	{
		std::cout << "ERROR::CANNOT_OPEN_VERTEX_SHADER_SRC" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!fFile.is_open())
	{
		std::cout << "ERROR::CANNOT_OPEN_SHADER_SRC" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::stringstream vStream;
	std::stringstream fStream;

	vStream << vFile .rdbuf();
	fStream << fFile.rdbuf();

	vFile.close();
	fFile.close();

	std::string vStr = vStream.str();
	std::string fStr = fStream.str();

	vStream.clear();
	fStream.clear();

	const char* vCode = vStr.c_str();
	const char* fCode = fStr.c_str();

	unsigned int vertexID, fragmentID;
	int success;
	char infoLog[512];

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vCode, nullptr);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED:" << std::endl;
		std::cout << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fCode, nullptr);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED:" << std::endl;
		std::cout << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);

	glGetShaderiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cout << "ERROR::SHADER_LINK::LINK_FAILED:" << std::endl;
		std::cout << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}
}

Shader::~Shader()
{

}

void Shader::use() const
{
	glUseProgram(ID);
}
