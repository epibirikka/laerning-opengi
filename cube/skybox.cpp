#include "skybox.hpp"
#include "program.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>

#include "stb_image.h"

Skybox::Skybox()
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.open("shaders/sky_vert.glsl", std::ifstream::in);
	fragmentFile.open("shaders/sky_frag.glsl", std::ifstream::in);

	shaderProgramId = createProgram(vertexFile, fragmentFile);

	vertexFile.close();
	fragmentFile.close();

	const GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	};

	const GLuint indexes[] = {
		0, 1, 2,
		0, 1, 3
	};

	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	glGenVertexArrays(1, &vertexId);
	glBindVertexArray(vertexId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &shortcutId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortcutId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	GLuint posLocation = glGetAttribLocation(shaderProgramId, "pos");
	GLuint texCoordsLocation = glGetAttribLocation(shaderProgramId, "inpTexCoords");

	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE,
			sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(posLocation);

	glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE,
			sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(texCoordsLocation);

	int textureWidth, textureHeight, _;

	unsigned char* skyTextureData = stbi_load("sky.jpg", &textureWidth, &textureHeight, &_, 0);

	glGenTextures(1, &mainTextureId);
	glBindTexture(GL_TEXTURE_2D, mainTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTextureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(shaderProgramId);
	glUniform1i(glGetUniformLocation(shaderProgramId, "texture0"), 0);
	glUseProgram(-1);

	stbi_image_free(skyTextureData);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Skybox::render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTextureId);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortcutId);

	glUseProgram(shaderProgramId);
	glBindVertexArray(vertexId);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
