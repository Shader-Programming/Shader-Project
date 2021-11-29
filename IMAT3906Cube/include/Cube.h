#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "normalmapper.h"

class Cube {
public:
	Cube();
	void CreateCube();
	void RenderCube(Shader& shader);
	unsigned int cubediff, cubenorm, cubespec;
private:
	unsigned int cubeVBO, cubeEBO, cubeVAO;
	glm::vec3 cubecolor = glm::vec3(1, 0.4, 0.4);
};