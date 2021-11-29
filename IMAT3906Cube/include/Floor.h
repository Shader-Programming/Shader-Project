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

class Floor {
public:
	Floor();
	void CreateFloor();
	void RenderFloor(Shader& shader);
	unsigned int floordiff, floornorm, floorspec;
private:
	unsigned int floorVBO, floorEBO, floorVAO;
	const glm::vec3 floorcolor = glm::vec3(0.1, 0.3, 0.3);
};