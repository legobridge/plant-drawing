#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <lsys.h>

using namespace std;

class Scene
{
public:

	// World dimensions
	int WORLD_W;
	int WORLD_H;

	// Pointer to a Shader object
	Shader* myShader;

	// The Vertex Array Object to draw from
	unsigned int VAO;

	// Background colors
	float backgroundColor[768][3];

	// Background vertices
	float background[768][1366 * 3];

	// LSys object pointer for the first tree
	LSys* LSys1;
	// LSys object pointer for the second tree
	LSys* LSys2;
	// LSys object pointer for the third tree
	LSys* LSys3;

	// Iteration to render on screen
	int iteration;

	// Constructor definition
	Scene()
	{
		// Set World size
		WORLD_W = 1366;
		WORLD_H = 768;

		// Build and compile shaders
		myShader = new Shader("./src/shader/vertex.shader", "./src/shader/fragment.shader");

		// Use the compiled shader program
		myShader -> use();

		// Generate and bind vertex array object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Initialize LSys Object with treeType = 1
		LSys1 = new LSys(1);
		// Initialize LSys Object with treeType = 2
		LSys2 = new LSys(2);
		// Initialize LSys Object with treeType = 3
		LSys3 = new LSys(3);

		// Start with 0th iteration (axiom)
		iteration = 0;
	}

	// Destructor definition
	~Scene()
	{
		// Deallocation of Resources
		delete myShader;
		delete LSys1;
		delete LSys2;
		delete LSys3;
		glDeleteVertexArrays(1, &VAO);
	}

	// Increase iteration counter, to a maximum of 4
	void iterUp()
	{
		iteration = min(iteration + 1, 4);
	}

	// Decrease iteration counter, to a minimum of 0
	void iterDown()
	{
		iteration = max(iteration - 1, 0);
	}

	// Compute the colors and vertices of the background
	void computeBackground()
	{
		// Assign all coordinates to the background vertices array
		for (int i = 0; i < WORLD_W; i++)
		{
			for (int j = 0; j < WORLD_H; j++)
			{
				background[j][i * 3] = (float)i;
				background[j][i * 3 + 1] = (float)j;
				background[j][i * 3 + 2] = 0.0f;
			}
		}

		// Define width and color of the soil
		int soilWidth = 100;
		float r = 0.074f;
		float g = 0.000f;
		float b = 0.200f;

		// Calculate respective changes in color values per scanline
		vector<float> delta = {r / (WORLD_H - soilWidth), g / (WORLD_H - soilWidth), b / (WORLD_H - soilWidth)};

		// Decrement color values by delta over time, and assign values to array
		for (int i = soilWidth; i < WORLD_H; i++)
		{
			backgroundColor[i][0] = r;
			backgroundColor[i][1] = g;
			backgroundColor[i][2] = b;
			r -= delta[0];
			g -= delta[1];
			b -= delta[2];
		}

		// Define color of the night sky
		r = 0.300f;
		g = 0.089f;
		b = 0.030f;

		// Calculate respective changes in color values per scanline
		delta = { r / soilWidth, g / soilWidth, b / soilWidth};

		// Decrement color values by delta over time, and assign values to array
		for (int i = 0; i < soilWidth; i++)
		{
			backgroundColor[i][0] = r;
			backgroundColor[i][1] = g;
			backgroundColor[i][2] = b;
			r -= delta[0];
			g -= delta[1];
			b -= delta[2];
		}
	}

	// Vertex calculations for the first tree
	void FirstTreeComputations()
	{
		// Expand instruction string
		LSys1 -> expand();

		// Fetch calculated vertices from LSys Object
		LSys1 -> setVertices();
	}

	// Vertex calculations for the second tree
	void SecondTreeComputations()
	{
		// Expand instruction string
		LSys2 -> expand();

		// Fetch calculated vertices from LSys Object
		LSys2 -> setVertices();
	}

	// Vertex calculations for the third tree
	void ThirdTreeComputations()
	{
		// Expand instruction string
		LSys3->expand();

		// Fetch calculated vertices from LSys Object
		LSys3->setVertices();
	}

	// Render the background using pre-calculated vertices and color values
	void drawBackground()
	{
		// Process the rendering vertically (since color is constant over a scanline)
		for (int i = 0; i < WORLD_H; i++)
		{
			// Setup transformation matrix
			glm::mat4 trans;
			trans = glm::scale(trans, glm::vec3(2 / (float)WORLD_W, 2 / (float)WORLD_H, 1.0f));
			trans = glm::translate(trans, glm::vec3(-683.0f, -384.0f, 0.0f));
			unsigned int transformLoc = glGetUniformLocation(myShader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			// Setup fragment color
			unsigned int myColorLoc = glGetUniformLocation(myShader->ID, "myColor");
			glUniform3f(myColorLoc, backgroundColor[i][0], backgroundColor[i][1], backgroundColor[i][2]);

			// Generate and bind vertex buffer object
			unsigned int VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, WORLD_W * 3 * sizeof(float), background[i], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Draw vertices
			glDrawArrays(GL_POINTS, 0, WORLD_W);

			// Delete the vertex buffer object from memory
			glDeleteBuffers(1, &VBO);
		}
	}

	// Draw an object from the vertices stored in vertexVector,
	// with its origin shifted to the coordinates in translationVector,
	// and its fragments shaded with the color defined by colorVector
	// (Note that the world resolution dimensions for objects is
	//  taken to be the double of the screen resolution dimensions
	//  for calculations with higher precision and better fidelity)
	void drawObject(vector<float> vertexVector, glm::vec3 translationVector, vector<float> colorVector)
	{
		// Setup transformation matrix
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		trans = glm::translate(trans, translationVector);
		unsigned int transformLoc = glGetUniformLocation(myShader -> ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Setup fragment color
		unsigned int myColorLoc = glGetUniformLocation(myShader -> ID, "myColor");
		glUniform3f(myColorLoc, colorVector[0], colorVector[1], colorVector[2]);

		// Convert vector to C-style array
		size_t n = vertexVector.size();
		float *vertices = new float[n];
		for (size_t i = 0; i < n; i++)
		{
			vertices[i] = vertexVector[i];
		}

		// Generate and bind vertex buffer object
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Draw vertices
		glDrawArrays(GL_POINTS, 0, n / 3);

		// Delete the vertex buffer object from memory
		glDeleteBuffers(1, &VBO);

		// Delete the vertex array from memory
		delete vertices;
	}

	// Call rendering functions for all the pre-computed objects
	void drawObjects()
	{
		// ******* Color definitions for the tree parts *******

		// (The trunk and branches change color with each iteration)
		vector<float> stalkColor = {0.054f, 0.516f, 0.046f};
		vector<float> trunkColor = {0.550f * (iteration + 1) / 5, 0.210f, 0.030f};
		vector<float> branchColor = {0.550f * (iteration + 1) / 5, 0.320f, 0.030f};
		vector<float> flowerColor1 = {0.700f, 0.501f, 0.070f};
		vector<float> flowerColor2 = {0.850f, 0.005f, 0.000f};


		// ******* Draw the computed objects on screen *******

		// Calls for the first tree
		drawObject((LSys1 -> v[iteration])[0], glm::vec3(800.0f, -630.0f, 0.0f), stalkColor);
		drawObject((LSys1 -> v[iteration])[1], glm::vec3(800.0f, -630.0f, 0.0f), trunkColor);
		drawObject((LSys1 -> v[iteration])[2], glm::vec3(800.0f, -630.0f, 0.0f), branchColor);
		drawObject((LSys1 -> v[iteration])[3], glm::vec3(800.0f, -630.0f, 0.0f), flowerColor1);
		drawObject((LSys1 -> v[iteration])[4], glm::vec3(800.0f, -630.0f, 0.0f), flowerColor2);

		// Calls for the second tree
		drawObject((LSys2 -> v[iteration])[0], glm::vec3(-100.0f, -680.0f, 0.0f), stalkColor);
		drawObject((LSys2 -> v[iteration])[1], glm::vec3(-100.0f, -680.0f, 0.0f), trunkColor);
		drawObject((LSys2 -> v[iteration])[2], glm::vec3(-100.0f, -680.0f, 0.0f), branchColor);
		drawObject((LSys2 -> v[iteration])[3], glm::vec3(-100.0f, -680.0f, 0.0f), flowerColor1);
		drawObject((LSys2 -> v[iteration])[4], glm::vec3(-100.0f, -680.0f, 0.0f), flowerColor2);

		// Calls for the third tree
		drawObject((LSys3 -> v[iteration])[0], glm::vec3(-650.0f, -630.0f, 0.0f), stalkColor);
		drawObject((LSys3 -> v[iteration])[1], glm::vec3(-650.0f, -630.0f, 0.0f), trunkColor);
		drawObject((LSys3 -> v[iteration])[2], glm::vec3(-650.0f, -630.0f, 0.0f), branchColor);
		drawObject((LSys3 -> v[iteration])[3], glm::vec3(-650.0f, -630.0f, 0.0f), flowerColor1);
		drawObject((LSys3 -> v[iteration])[4], glm::vec3(-650.0f, -630.0f, 0.0f), flowerColor2);
	}

	// Computation driver function
	void computeSceneVertices()
	{
		computeBackground();
		FirstTreeComputations();
		SecondTreeComputations();
		ThirdTreeComputations();
	}
};

#endif