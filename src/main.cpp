// Main Source File for Line Segment Drawing in OpenGL
// Authors -  Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 15/09/2017

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
	// Computations
	/*Calc myCalc(a, b);
	float *vertices = myCalc.getVertexArray();
	size_t n = myCalc.n;*/

	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

	// Build and compile shaders
	Shader myShader("./src/shader/vertex.shader", "./src/shader/fragment.shader");

	//// Generate and bind vertex array object
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//// Generate and bind vertex buffer object
	//unsigned int VBO;
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	// Wireframe Mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Input
		processInput(window);


		// Rendering Commands
		// Background Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*myShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, n / 3);
		glBindVertexArray(0);*/

		// Check and call events, then swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	//// Deallocation of Resources
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}