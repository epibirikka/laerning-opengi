#pragma once

#include <glad/glad.h>

class Skybox
{
	public:
		Skybox();
		void render();
		GLuint shaderProgramId;
	private:
		GLuint bufferId;
		GLuint mainTextureId;
		GLuint vertexId;
		GLuint shortcutId;
};
