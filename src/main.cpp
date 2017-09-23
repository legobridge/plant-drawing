// Main Source File for L-System Based Plant Drawing in OpenGL
// Authors -  Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 22/09/2017

#include <iostream>
#include <algorithm>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <scene.h>

using namespace std;


// Constants
const unsigned int SCR_W = 1366;
const unsigned int SCR_H = 768;
const unsigned int SCR_MIN_W = 640;
const unsigned int SCR_MIN_H = 360;
const unsigned int SCR_MAX_W = 1366;
const unsigned int SCR_MAX_H = 768;

// Pointer to a GLFWwindow object
GLFWwindow* window;

// Pointer to a Scene object
Scene* myScene;


// Callback Function for Framebuffer Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// **************************************
// ********** Input Processing **********
// - Esc closes the window
// - Up key displays next iteration
// - Down key displays previous iteration
// **************************************

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		myScene -> iterUp();
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		myScene -> iterDown();
	}
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

	// Window (default: 768p fullscreen) and Context Creation
	window = glfwCreateWindow(SCR_W, SCR_H, "Plant Drawing with L-Systems", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		cout << "Window or context creation failed" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Register Callback Function for Window Resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register Callback Function for keypresses
	glfwSetKeyCallback(window, key_callback);

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


int main()
{
	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

	// Create Scene object myScene and pre-process vertices
	myScene = new Scene();
	myScene -> computeSceneVertices();

	// **************** Render Loop ****************
	while (!glfwWindowShouldClose(window))
	{
		// ******** Rendering Commands ********

		// Draw Background
		myScene -> drawBackground();

		// Draw Objects
		myScene -> drawObjects();

		// ****** End Rendering Commands ******

		// Check for events, then swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Deallocation of Resources
	delete myScene;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}