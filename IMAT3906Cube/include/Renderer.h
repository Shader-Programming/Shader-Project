#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <../include/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Floor.h"
class Renderer {
public:

	Renderer(unsigned int scrw,unsigned int scrh);
	void RenderScene(Shader& shader,Camera cam);
private:
	unsigned int screenW, screenH;
	void CreateObjects();
	Cube cube1;
	Floor floor1;
};

#endif