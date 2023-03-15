#pragma once

#include <iostream>
#include <fstream>

#include <glad/glad.h>

GLuint createProgram(std::ifstream& vertexStream, std::ifstream& fragmentStream);
