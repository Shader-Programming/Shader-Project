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

class Quad {
public:
	Quad();
	void CreateQuad();
	void RenderQuad(Shader& shader, unsigned int& textureobj);
	void RenderQuad(Shader& shader, unsigned int& textureobj, unsigned int& textureobj2);
	unsigned int testtex;
private:
	unsigned int quadVBO, quadVAO;
};