#include "Light.h"

Light::Light() {

}

void Light::RenderLight(Shader& shader) {
	shader.use();
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, lightpos);
	shader.setMat4("model", model);
	shader.setVec3("objectcol", lightcolor);
	shader.setVec3("lightcol", lightcolor);
	glBindVertexArray(lightVAO);  // bind and draw light
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Light::CreateLight() {
	// cube data
	float cubeVertices[] = {
		//back
		-0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
		0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
		0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,
		-0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,

		//front
		-0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
		0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
		0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,
		-0.1f, 0.1f,  0.1f,  0.0f,  0.0f,  1.0f,

		//left
		-0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,
		-0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
		-0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,
		-0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,

		//right
		0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,
		0.1f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
		0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,
		0.1f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,

		//bottom
		-0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,
		0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,
		0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,
		-0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,

		//top	
		-0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,
		0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,
		0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,
		-0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,

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

		//19,18,17,
		//19,17,16,

		//23,22,21,
		//23,21,20
	};

	// Create VAO
	// Floor
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &lightEBO);

	glBindVertexArray(lightVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeIndices, GL_STATIC_DRAW);
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeVertices), cubeIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}