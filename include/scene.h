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

	// Increase iteration counter
	void iterUp()
	{
		iteration = min(iteration + 1, 5);
	}

	// Decrease iteration counter
	void iterDown()
	{
		iteration = max(iteration - 1, 0);
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
		drawObject((LSys1 -> v[iteration]).first, glm::vec3(800.0f, -668.0f, 0.0f), stalkColor);
		drawObject((LSys1 -> v[iteration]).second, glm::vec3(800.0f, -668.0f, 0.0f), flowerColor);
		drawObject((LSys2 -> v[iteration]).first, glm::vec3(0.0f, -668.0f, 0.0f), stalkColor);
		drawObject((LSys2 -> v[iteration]).second, glm::vec3(0.0f, -668.0f, 0.0f), flowerColor);
		//drawObject((LSys3 -> v[iteration]), glm::vec3(-800.0f, -668.0f, 0.0f), stalkColor);
		//drawObject((LSys3 -> v[iteration]), glm::vec3(-800.0f, -668.0f, 0.0f), flowerColor);
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