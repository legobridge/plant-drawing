#ifndef SCENE_H
#define SCENE_H

//#include 
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <lsys.h>
#include <vector>
#include <time.h>

using namespace std;

class Scene
{
public:
	unsigned int WORLD_W;
	unsigned int WORLD_H;
	Shader* myShader;
	unsigned int VAO;
	pair<vector<float>, vector<float> > vertexVectors1;
	pair<vector<float>, vector<float> > vertexVectors2;
	pair<vector<float>, vector<float> > vertexVectors3;
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

		iteration = 1;
	}

	// Destructor definition
	~Scene()
	{
		// Deallocation of Resources
		delete myShader;
		glDeleteVertexArrays(1, &VAO);
	}

	void FirstTreeComputations()
	{
		LSys myLSys(1, "[T][+T][-T]");
		for (int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}

		pair<vector<float>, vector<float> > p = myLSys.getVertices();

		// ********* Branch Computations *********
		vertexVectors1.first = p.first;


		// ********* Flower Computations *********
		vertexVectors1.second = p.second;
	}

	//void SecondTree()
	//{
	//}

	//void ThirdTree()
	//{
	//}

	// Draw an object from the vertices stored in vertexVector
	void drawObject(vector<float> vertexVector, glm::vec3 translationVector, vector<float> rgb)
	{
		// Setup transformation matrix
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		trans = glm::translate(trans, translationVector);
		unsigned int transformLoc = glGetUniformLocation(myShader -> ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Setup fragment color
		unsigned int myColorLoc = glGetUniformLocation(myShader -> ID, "myColor");
		glUniform3f(myColorLoc, rgb[0], rgb[1], rgb[2]);

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
		vector<float> g = {0.0f, 1.0f, 0.0f};
		vector<float> r = {1.0f, 0.0f, 0.0f};

		drawObject(vertexVectors1.first, glm::vec3(0.0f, 0.0f, 0.0f), g);
		drawObject(vertexVectors1.second, glm::vec3(0.0f, 0.0f, 0.0f), r);
		/*drawObject(vertexVectors2.first);
		drawObject(vertexVectors2.second);
		drawObject(vertexVectors3.first);
		drawObject(vertexVectors3.second);*/
	}

	// Computation driver function
	void computeSceneVertices()
	{
		FirstTreeComputations();
		/*SecondTreeComputations();
		ThirdTreeComputations();*/
	}
};

#endif