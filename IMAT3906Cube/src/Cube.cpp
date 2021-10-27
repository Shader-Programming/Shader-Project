#include "Cube.h"

Cube::Cube() {

}

void Cube::RenderCube(Shader& shader) {
	shader.setVec3("objectcol", cubecolor);
	glBindVertexArray(cubeVAO);  // bind and draw cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -3));
	shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0, 0, 3));
	model = glm::rotate(model, (float)glfwGetTime() * 0.5f, glm::vec3(2, 2, 2));
	model = glm::scale(model, glm::vec3(2));
	shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Cube::CreateCube() {
	// cube data
	float cubeVertices[] = {
		//back
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 0
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   //1
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			//front
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   //4
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			//left
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  //8
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			//right
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   //12
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			//bottom
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  //16
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			//top	
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //20
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

	};

	unsigned int cubeIndices[] = {
		1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		23,22,21,
		23,21,20,

		16,17,18,
		16,18,19
	};


	// Create VAO
	// Cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);

	glBindVertexArray(cubeVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}