#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

class Light {
public:
	Light();
	void CreateLight();
	void RenderLight(Shader& shader);
	unsigned int lightnorm;
	glm::vec3 lightpos;
	glm::vec3 lightcolor = glm::vec3(0.1, 0.3, 0.3);
private:
	unsigned int lightVBO, lightEBO, lightVAO;
};