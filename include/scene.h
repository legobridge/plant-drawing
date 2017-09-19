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
	const unsigned int WORLD_W = 1366;
	const unsigned int WORLD_H = 768;
	Shader myShader;
	unsigned int VAO;
	vector<float> vertexVector;
	size_t n;
	unsigned int iteration;

	// Constructor definition
	Scene()
	{
		// Build and compile shaders
		myShader = Shader("./src/shader/vertex.shader", "./src/shader/fragment.shader");
		myShader.use();

		// Generate and bind vertex array object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}

	// Destructor definition
	~Scene()
	{
		// Deallocation of Resources
		myShader.~Shader();
		glDeleteVertexArrays(1, &VAO);
	}

	void FirstTree()
	{
		LSys myLSys(1, "[T][+T][-T]");
		for (unsigned int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}
		 pair<vector<float>, vector<float> > p = myLSys.getVertices();


		// ********* Branch Rendering *********
		vertexVector = p.first;
		n = vertexVector.size();

		// Setup transformation matrix
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		trans = glm::translate(trans, glm::vec3(1000.0f, -668.0f, 0.0f));
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Setup fragment color
		unsigned int myColorLoc = glGetUniformLocation(myShader.ID, "myColor");
		glUniform3f(myColorLoc, 1.0f, 0.0f, 0.0f);

		drawObject();


		//// ********* Flower Rendering *********
		//vertexVector = p.second;
		//n = vertexVector.size();

		//// Setup transformation matrix
		//trans = glm::mat4();
		//trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		//trans = glm::translate(trans, glm::vec3(1000.0f, -668.0f, 0.0f));
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		//// Setup fragment color
		//glUniform3f(myColorLoc, 1.0f, 0.0f, 0.0f);

		//drawObject();
	}

	void SecondTree()
	{
		LSys myLSys(1, "[T][+T][-T]");
		for (unsigned int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}
		pair<vector<float>, vector<float> > p = myLSys.getVertices();

		// ********* Branch Rendering *********
		vertexVector = p.first;
		n = vertexVector.size();

		// Setup transformation matrix
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		trans = glm::translate(trans, glm::vec3(300.0f, -668.0f, 0.0f));
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Setup fragment color
		unsigned int myColorLoc = glGetUniformLocation(myShader.ID, "myColor");
		glUniform3f(myColorLoc, 0.0f, 1.0f, 0.0f);

		drawObject();
	}

	void ThirdTree()
	{
		LSys myLSys(1, "[T][+T][-T]");
		for (unsigned int i = 1; i <= iteration; i++)
		{
			myLSys.expand();
		}
		pair<vector<float>, vector<float> > p = myLSys.getVertices();

		// ********* Branch Rendering *********
		vertexVector = p.first;
		n = vertexVector.size();
		
		// Setup transformation matrix
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(1 / (float)WORLD_W, 1 / (float)WORLD_H, 1.0f));
		trans = glm::translate(trans, glm::vec3(-400.0f, -668.0f, 0.0f));
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Setup fragment color
		unsigned int myColorLoc = glGetUniformLocation(myShader.ID, "myColor");
		glUniform3f(myColorLoc, 0.0f, 0.0f, 1.0f);

		drawObject();
	}

	// Draw an object from the vertices stored in vertexVector
	void drawObject()
	{
		// Convert vector to C-style array
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

	// Driver function
	void drawScene()
	{
		FirstTree();
		SecondTree();
		ThirdTree();
	}
};

#endif