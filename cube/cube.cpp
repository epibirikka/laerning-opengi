#include "cube.hpp"
#include "program.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube(glm::mat4 projection)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	angle = 0.0f;

	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.open("shaders/cube_vertex.glsl", std::ifstream::in);
	fragmentFile.open("shaders/cube_frag.glsl", std::ifstream::in);

	shaderProgramId = createProgram(vertexFile, fragmentFile);

	vertexFile.close();
	fragmentFile.close();

	const GLfloat vertices[] = 
	{
		// front
		-1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 	1.0f, 1.0f,

		// right side
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

		// back side
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

		// left side
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		// bottom side
		-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f,

		// top side
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
	};

	const GLuint indexes[] =
	{
		0, 1, 2,
		0, 1, 3,

		4, 5, 6,
		4, 5, 7,

		8, 9, 10,
		8, 9, 11,

		12, 13, 14,
		12, 13, 15,

		16, 17, 18,
		16, 17, 19,

		20, 21, 22,
		20, 21, 23,
	};

	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	glGenBuffers(1, &shortcutId);

	glGenVertexArrays(1, &vertexId);
	glBindVertexArray(vertexId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortcutId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	GLuint layoutLocation = glGetAttribLocation(shaderProgramId, "pos");
	GLuint textureLayoutLocation = glGetAttribLocation(shaderProgramId, "inpTextcoord");

	glUseProgram(shaderProgramId);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);

	transformLocation = glGetUniformLocation(shaderProgramId, "transform");

	// position
	glVertexAttribPointer(layoutLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(layoutLocation);

	// texture
	glVertexAttribPointer(textureLayoutLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(textureLayoutLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::render()
{
	glm::mat4 renTrans = glm::mat4(1.0f);
	
	renTrans = glm::scale(renTrans, glm::vec3(0.25, 0.25, 0.25));
	renTrans = glm::translate(renTrans, position);
	renTrans = glm::rotate(renTrans, angle, rotation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortcutId);

	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(renTrans));

	glUseProgram(shaderProgramId);
	glBindVertexArray(vertexId);
	glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, 0);
}
