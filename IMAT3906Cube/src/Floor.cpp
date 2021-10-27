#include "Floor.h"

Floor::Floor() {
	
}

void Floor::RenderFloor(Shader& shader) {
	shader.setVec3("objectcol", floorcolor);
	glm::mat4 model = glm::mat4(1.0);
	shader.setMat4("model", model);
	shader.setVec3("objectcol", floorcolor);
	glBindVertexArray(floorVAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Floor::CreateFloor() {
	float floorSize = 5.0f;
	float floorLevel = -2.0f;
	float floorVertices[] = {
			 -floorSize, floorLevel,  -floorSize, 0.0, 1.0, 0.0,
			 floorSize, floorLevel,   -floorSize, 0.0, 1.0, 0.0,
			 floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,
			-floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,
	};

	unsigned int floorIndices[] = {
		3,2,1,
		3,1,0
	};

	// Create VAO
	// Floor
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glGenBuffers(1, &floorEBO);

	glBindVertexArray(floorVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}