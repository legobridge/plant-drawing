#ifndef SCENE_H
#define SCENE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <lsys.h>
#include <vector>

using namespace std;

class Scene
{
public:

	// World dimensions
	unsigned int WORLD_W;
	unsigned int WORLD_H;

	// Pointer to a Shader Object
	Shader* myShader;

	// The Vertex Array Object to draw from
	unsigned int VAO;

	// ****************************************************
	// Pairs of float vectors to store vertices of each
	// of many trees:
	// - The first vector in each pair stores positions
	//   of the green colored points of the tree
	// - The second vector in each pair stores positions
	//   of the points that denote the flowers of the tree
	// ****************************************************
	pair<vector<float>, vector<float> > vertexVectors1;
	pair<vector<float>, vector<float> > vertexVectors2;
	pair<vector<float>, vector<float> > vertexVectors3;

	// The iteration counter
	int iteration;

	// Constructor definition
	Scene()
	{
		// Set World size
		WORLD_W = 1366;
		WORLD_H = 768;

		// Build and compile shaders
		myShader = new Shader("./src/shader/vertex.shader", "./src/shader/fragment.shader");
		myShader -> use();

		// Generate and bind vertex array object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Initialize iteration counter with 0
		iteration = 0;
	}

	// Destructor definition
	~Scene()
	{
		// Deallocation of Resources
		delete myShader;
		glDeleteVertexArrays(1, &VAO);
	}

	// Vertex calculations for the first tree
	void FirstTreeComputations()
	{
		// Create LSys Object with treeType = 1
		LSys myLSys(1);

		// Expand string according to iteration counter
		for (int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}

		// Fetch calculated vertices from LSys Object
		pair<vector<float>, vector<float> > p = myLSys.getVertices();

		// Branch Vertex Computations
		vertexVectors1.first = p.first;


		// Flower Vertex Computations
		vertexVectors1.second = p.second;
	}

	// Vertex calculations for the second tree
	void SecondTreeComputations()
	{
		// Create LSys Object with treeType = 2
		LSys myLSys(2);

		// Expand string according to iteration counter
		for (int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}

		// Fetch calculated vertices from LSys Object
		pair<vector<float>, vector<float> > p = myLSys.getVertices();

		// Branch Vertex Computations
		vertexVectors2.first = p.first;


		// Flower Vertex Computations
		vertexVectors2.second = p.second;
	}

	//// Vertex calculations for the third tree
	//void ThirdTreeComputations()
	//{
	//	// Create LSys Object with treeType = 3
	//	LSys myLSys(3);

	//	// Expand string according to iteration counter
	//	for (int i = 1; i <= iteration; i++)
	//	{
	//		myLSys.expand();
	//	}

	//	// Fetch calculated vertices from LSys Object
	//	pair<vector<float>, vector<float> > p = myLSys.getVertices();

	//	// Branch Vertex Computations
	//	vertexVectors3.first = p.first;


	//	// Flower Vertex Computations
	//	vertexVectors3.second = p.second;
	//}

	// Draw an object from the vertices stored in vertexVector,
	// with its origin shifted to the coordinates in translationVector,
	// and its fragments shaded with the color defined by colorVector
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
	}

	void drawObjects()
	{
		// Color definitions for the tree stalks and flowers
		vector<float> stalkColor = {0.0f, 1.0f, 0.0f};
		vector<float> flowerColor = { 1.0f, 0.0f, 0.0f };

		// Draw the computed objects on screen
		drawObject(vertexVectors1.first, glm::vec3(800.0f, -668.0f, 0.0f), stalkColor);
		drawObject(vertexVectors1.second, glm::vec3(800.0f, -668.0f, 0.0f), flowerColor);
		drawObject(vertexVectors2.first, glm::vec3(0.0f, -668.0f, 0.0f), stalkColor);
		drawObject(vertexVectors2.second, glm::vec3(0.0f, -668.0f, 0.0f), flowerColor);
		//drawObject(vertexVectors3.first, glm::vec3(-800.0f, -668.0f, 0.0f), stalkColor);
		//drawObject(vertexVectors3.second, glm::vec3(-800.0f, -668.0f, 0.0f), flowerColor);
	}

	// Computation driver function
	void computeSceneVertices()
	{
		FirstTreeComputations();
		SecondTreeComputations();
		// ThirdTreeComputations();
	}
};

#endif