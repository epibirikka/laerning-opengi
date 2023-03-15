#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"

class Cube
{
	public:
		Cube(glm::mat4 projection);
		void render();
		GLuint shaderProgramId;

		glm::vec3 position;
		glm::vec3 rotation;
		
		float angle;
	private:
		GLuint vertexId;
		GLuint bufferId;
		GLuint shortcutId;

		GLuint transformLocation;
};
