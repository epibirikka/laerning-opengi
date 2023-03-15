#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "cube.hpp"
#include "skybox.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void onWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;

	if ((window = glfwCreateWindow(600, 600, "sample window", NULL, NULL)) == NULL)
	{
		std::cerr << "cannot create glfw window\n";
		glfwTerminate();
		std::exit(1);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "cannot connect to opengl\n";
		std::exit(1);
	}

	glEnable(GL_DEPTH_TEST);

	const int windowWidth = 600;
	const int windowHeight = 600;

	glm::mat4 perspective = glm::perspective(glm::radians(45.f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

	Cube cube(perspective);
	Skybox skybox;

	cube.position = glm::vec3(0.f, 0.f, -10.f);
	cube.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
	
	// cube texture
	GLuint cubeTexture;
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	int textureWidth, textureHeight, _;
	unsigned char* textureImageData = stbi_load("crate.jpg", &textureWidth, &textureHeight, &_, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureImageData);

	// troll face?
	GLuint trollTexture;
	glGenTextures(1, &trollTexture);
	glBindTexture(GL_TEXTURE_2D, trollTexture);

	int trollWidth, trollHeight, t_;
	unsigned char* trollfaceImageData = stbi_load("awesome.png", &trollWidth, &trollHeight, &t_, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, trollWidth, trollHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, trollfaceImageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(trollfaceImageData);

	glfwSetFramebufferSizeCallback(window, onWindowResize);

	glUseProgram(cube.shaderProgramId);

	glUniform1i(glGetUniformLocation(cube.shaderProgramId, "texture1"), 0);
	glUniform1i(glGetUniformLocation(cube.shaderProgramId, "texture2"), 1);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		glUseProgram(skybox.shaderProgramId);
		glUniform1f(glGetUniformLocation(skybox.shaderProgramId, "timer"), (float)glfwGetTime());
		// skybox.render();

		cube.angle = glm::radians((float)glfwGetTime() * 90);
		cube.position = glm::vec3((xpos - (windowWidth * 0.5)) / windowWidth, ypos / windowHeight, -15.0f);

		glActiveTexture(GL_TEXTURE0); // could this be it?
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, trollTexture);

		glUseProgram(cube.shaderProgramId);
		glUniform1f(glGetUniformLocation(cube.shaderProgramId, "timer"), (float)glfwGetTime());

		cube.render();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
