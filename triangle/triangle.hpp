#pragma once

#include "glad/glad.h"

class TriangleObject
{
	public:
		TriangleObject(bool setIsRainbow);
		void render();
	private:
		GLuint bufferId;
		GLuint vertexId;

		GLuint vertexShaderId;
		GLuint pixelShaderId;

		GLuint programId;
};
