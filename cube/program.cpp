#include <iostream>
#include <fstream>

#include <sstream>
#include <string.h>

#include "program.hpp"

#include <glad/glad.h>


GLuint createProgram(std::ifstream& vertexStream, std::ifstream& fragmentStream)
{
	if (!vertexStream.is_open() || !fragmentStream.is_open())
	{
		std::cerr << "one of the shader files are non-existent or can't be opened to read\n";
		return -1;
	}

	// source from the files
	std::stringstream vertexbuffer;
	std::stringstream fragmentbuffer;

	vertexbuffer << vertexStream.rdbuf();
	fragmentbuffer << fragmentStream.rdbuf();

	std::string vertexbufferString = vertexbuffer.str();
	std::string fragmentbufferString = fragmentbuffer.str();

	const char* vertexbufferPass = vertexbufferString.c_str();
	const char* fragmentbufferPass = fragmentbufferString.c_str();

	// attach to program
	
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderId, 1, &vertexbufferPass, NULL);
	glCompileShader(vertexShaderId);

	int vertexSuccess;
	char vertexInfoLog[10000];

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &vertexSuccess);

	if (!vertexSuccess)
	{
		glGetShaderInfoLog(vertexShaderId, 10000, NULL, vertexInfoLog);
		std::cerr << "vertex shader failed: " << vertexInfoLog << "\n";
		return  -1;
	}

	glShaderSource(fragmentShaderId, 1, &fragmentbufferPass, NULL);
	glCompileShader(fragmentShaderId);

	int fragmentSuccess;
	char fragmentInfoLog[10000];

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &fragmentSuccess);

	if (!fragmentSuccess)
	{
		glGetShaderInfoLog(fragmentShaderId, 10000, NULL, fragmentInfoLog);
		std::cerr << "fragment shader failed: " << fragmentInfoLog << "\n";
		return  -1;
	}

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	int linkSuccess;
	char linkInfoLog[1000];

	glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess)
	{
		glGetProgramInfoLog(programId, 1000, NULL, linkInfoLog);
		std::cerr << "linking failed: " << linkInfoLog << "\n";
		return -1;
	}

	std::cout << "program linking success\n";

	return programId;
};
