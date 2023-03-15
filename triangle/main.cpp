#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "triangle.hpp"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow;

	if ((mainWindow = glfwCreateWindow(800, 600, "sample window", NULL, NULL)) == NULL)
	{
		std::cerr << "can't make window!!\n";
		glfwTerminate();
		std::exit(1);
	}
	
	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "can't load glad wtf\n";
		std::exit(1);
	}

	int winWidth, winHeight;
	glfwGetWindowSize(mainWindow, &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);

	TriangleObject triangle = TriangleObject(true);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.15, 0.15, 0.15, 1);

		triangle.render();

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
