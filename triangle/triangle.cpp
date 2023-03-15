#include "triangle.hpp"

#include <iostream>
#include <fstream>
#include <string.h>

#include "glad/glad.h"

const GLfloat constVertices[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
};

// for now
void printSuccess(GLuint shaderId)
{
	int success;
	char logbuffer[2048];

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!!success)
	{
		std::cout << "shaderid " << shaderId << " worked!\n";
		return;
	}

	glGetShaderInfoLog(shaderId, 2048, NULL, logbuffer);
	std::cerr << "how did " << shaderId << " not work!!: " << logbuffer << "\n";
}

TriangleObject::TriangleObject(bool setIsRainbow)
{
	std::ifstream vertexFs("trivert.glsl", std::fstream::in);
	std::ifstream pixelFs("tripixel.glsl", std::fstream::in);

	// vertex shading

	std::string vertexFiledata = "";

	if (vertexFs.is_open())
	{
		std::string currentline;

		while (std::getline(vertexFs, currentline))
		{
			vertexFiledata += currentline + "\n";
		}

		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

		const char* cStrSource = vertexFiledata.c_str();

		glShaderSource(vertexShaderId, 1, &cStrSource, NULL);
		glCompileShader(vertexShaderId);

		printSuccess(vertexShaderId);
	}
	else {
		// lol
		std::cerr << "failed to open file for vertex shading!!!\n";
	}

	// pixel shading
	std::string pixelFiledata = "";

	if (pixelFs.is_open())
	{
		std::string currentline;

		while (std::getline(pixelFs, currentline))
		{
			pixelFiledata += currentline + "\n";
		}

		pixelShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		const char* cStrSource = pixelFiledata.c_str();

		glShaderSource(pixelShaderId, 1, &cStrSource, NULL);
		glCompileShader(pixelShaderId);

		printSuccess(pixelShaderId);
	}
	else {
		std::cerr << "failed to open file for pixel shading!!!\n";
	}

	glUniform1f(glGetUniformLocation(programId, "makeItRainbow"), setIsRainbow?1.0f:0.0f);

	// bleh if i wanted to go check for errors i should probably create a shader class/wrapper for that
	
	pixelFs.close();
	vertexFs.close();

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, pixelShaderId);
	glLinkProgram(programId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(pixelShaderId);

	// wtf is (void*) used for????
	glGenVertexArrays(1, &vertexId);
	glBindVertexArray(vertexId);

	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(constVertices), constVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void TriangleObject::render()
{
	// this should set the external shaders
	glUseProgram(programId);

	glBindVertexArray(vertexId);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
