#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
class Renderer {
public:

	Renderer(unsigned int scrw,unsigned int scrh);
	void RenderScene(Shader& shader,Camera cam);
private:
	void RenderCubes(Shader& shader);
	void RenderFloor(Shader& shader);
	void CreateCube();
	void CreateFloor();

	unsigned int floorVBO, cubeVBO, floorEBO, cubeEBO, cubeVAO, floorVAO;

	float floorsize = 5.0f;
	float floorlevel = -2.0f;
	const glm::vec3 cubecolor = glm::vec3(1, 0.4, 0.4);
	const glm::vec3 floorcolor = glm::vec3(0.1, 0.3, 0.3);
	unsigned int screenW, screenH;
};

#endif