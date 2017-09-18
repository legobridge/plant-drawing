// Main Source File for L-System Based Plant Drawing in OpenGL
// Authors -  Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 18/09/2017

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <linecalc.h>
#include <circlecalc.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <scene.h>

using namespace std;


// Constants
const unsigned int SCR_W = 1024;
const unsigned int SCR_H = 576;
const unsigned int SCR_MIN_W = 640;
const unsigned int SCR_MIN_H = 360;
const unsigned int SCR_MAX_W = 1280;
const unsigned int SCR_MAX_H = 720;

// Pointer to the window
GLFWwindow* window;


// Callback Function for Framebuffer Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


// OpenGL Initialization
bool initializeOpenGL()
{
	// GLFW Initialization
	if (!glfwInit())
	{
		cout << "Initialization failed" << endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window (576p default) and Context Creation
	window = glfwCreateWindow(SCR_W, SCR_H, "Window", NULL, NULL);
	if (!window)
	{
		cout << "Window or context creation failed" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Register Callback Function for Window Resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Fix Aspect Ratio to 16:9
	glfwSetWindowAspectRatio(window, 16, 9);

	// Set Minimum Limit for Window Size to 360p
	glfwSetWindowSizeLimits(window, SCR_MIN_W, SCR_MIN_H, SCR_MAX_W, SCR_MAX_H);

	//  Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// Viewport Settings
	glViewport(0, 0, SCR_W, SCR_H);

	return true;
}


// ********************************
// ******* Input Processing *******
// - Pressing Esc closes the window
// ********************************

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


int main()
{
	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

	// Create instance of the Scene class
	Scene myScene;

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Input
		processInput(window);


		// Rendering Commands
		// Background Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Objects
		Scene myScene;
		myScene.drawScene();


		// Check and call events, then swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}