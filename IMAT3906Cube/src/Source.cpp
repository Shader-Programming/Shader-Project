#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

#include<string>
#include <iostream>
#include <numeric>

#include "Cube.h"
#include "Floor.h"
#include "Light.h"



//// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

const unsigned int SH_WIDTH = 1024;
const unsigned int SH_HEIGHT = 1024;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void SetFBOColourAndDepth();
void SetFBOBlur();
void SetFBODoF();
void SetFBODepth();

glm::vec3 lightdirection = glm::vec3(0.5, -2.0f, -2.0f);


// camera
Camera camera(glm::vec3(0,0,9));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int map = 0;

unsigned int myFBO,MyFBODepth,myFBOColourAndDepth,myFBOBlur,myFBODoF;
unsigned int blurredtexture,doftexture, depthattachment;
unsigned int depthmap;
unsigned int FBOShadow;
unsigned int colourattachment[2];


void SetUniform(Shader& shader, Shader& shader2, Shader& shader3, Shader& shader4,Shader& shader5,Shader& shader6) {
	float bloombrightness = 0.85f;
	shader.use();
	//Cube
	//dir light
	glm::vec3 lightcolor = glm::vec3(1.0, 1.0, 1.0);
	//glm::vec3 plightpos = glm::vec3(0.0, 3.0, -4.0);
	//glm::vec3 plightpos2 = glm::vec3(0.0, 3.0, -4.0);

	glm::vec3 plightspos[] = {
		glm::vec3(0.0, 1.0, -4.0),
		glm::vec3(0.0, 1.0, 4.0)
	};

	glm::vec3 plightscol[] = {
		glm::vec3(3.0, 3.0, 3.0),
		glm::vec3(3.0, 3.0, 3.0)
	};

	float kc = 1.0f;
	float kl = 0.22f;
	float ke = 0.2f;

	shader.setVec3("lightcol", lightcolor);
	shader.setVec3("lightdir", lightdirection);
	shader.setFloat("bloombrightness", bloombrightness);

	shader.setVec3("plights[0].pos", plightspos[0]);
	shader.setVec3("plights[0].col", plightscol[0]);
	shader.setFloat("plights[0].kc", kc);
	shader.setFloat("plights[0].kl", kl);
	shader.setFloat("plights[0].ke", ke);

	shader.setVec3("plights[1].pos", plightspos[1]);
	shader.setVec3("plights[1].col", plightscol[1]);
	shader.setFloat("plights[1].kc", kc);
	shader.setFloat("plights[1].kl", kl);
	shader.setFloat("plights[1].ke", ke);

	shader.setVec3("slight.col", glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setFloat("slight.kc", 1.0f);
	shader.setFloat("slight.kl", 0.027f);
	shader.setFloat("slight.ke", 0.0028f);
	shader.setFloat("slight.innerrad", glm::cos(glm::radians(12.5f)));
	shader.setFloat("slight.outerrad", glm::cos(glm::radians(17.5f)));

	shader.setInt("diffusetexture", 0);
	shader.setInt("speculartexture", 1);
	shader.setInt("normalmap", 2);

	shader2.use();
	//Floor

	shader2.setVec3("plights[0].pos", plightspos[0]);
	shader2.setVec3("plights[0].col", plightscol[0]);
	shader2.setFloat("plights[0].kc", kc);
	shader2.setFloat("plights[0].kl", kl);
	shader2.setFloat("plights[0].ke", ke);

	shader2.setVec3("plights[1].pos", plightspos[1]);
	shader2.setVec3("plights[1].col", plightscol[1]);
	shader2.setFloat("plights[1].kc", kc);
	shader2.setFloat("plights[1].kl", kl);
	shader2.setFloat("plights[1].ke", ke);

	shader2.setFloat("bloombrightness", bloombrightness);

	shader2.setVec3("slight.col", glm::vec3(2.0f, 2.0f, 2.0f));
	shader2.setFloat("slight.kc", 1.0f);
	shader2.setFloat("slight.kl", 0.027f);
	shader2.setFloat("slight.ke", 0.0028f);
	shader2.setFloat("slight.innerrad", glm::cos(glm::radians(12.5f)));
	shader2.setFloat("slight.outerrad", glm::cos(glm::radians(17.5f)));

	shader2.setInt("diffusetexture", 0);
	shader2.setInt("speculartexture", 1);
	shader2.setInt("normalmap", 2);
	shader2.setInt("displacementmap", 3);
	shader2.setFloat("PXscale", 0.0175);
	shader2.setVec3("mat.ambient", glm::vec3(1, 1, 1));
	shader2.setFloat("mat.shine", 265);

	shader3.use();
	shader3.setInt("image", 0);

	shader4.use();
	shader4.setInt("image", 0);

	shader5.use();
	shader5.setInt("image", 0);

	shader6.use();
	shader6.setInt("image", 0);
	shader6.setInt("bloomblur", 1);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
	// simple vertex and fragment shader 
	Shader cubeshader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
	Shader floorshader("..\\shaders\\floorVert.vs", "..\\shaders\\floorFrag.fs");
	Shader postprocess("..\\shaders\\PP.vs", "..\\shaders\\PP.fs");
	Shader depthpostprocess("..\\shaders\\PP.vs", "..\\shaders\\DPP.fs");
	Shader blurshader("..\\shaders\\PP.vs", "..\\shaders\\Blur.fs");
	Shader bloomshader("..\\shaders\\PP.vs", "..\\shaders\\Bloom.fs");
	Shader lightshader("..\\shaders\\Light.vs", "..\\shaders\\Light.fs");
	Shader dofshader("..\\shaders\\PP.vs", "..\\shaders\\DoF.fs");
	Shader shadowmapshader("..\\shaders\\SM.vs", "..\\shaders\\SM.fs");
	SetUniform(cubeshader,floorshader, postprocess,depthpostprocess,blurshader,bloomshader);
	//SetFBOColourAndDepth();
	//SetFBOBlur();
	//SetFBODoF();
	SetFBODepth();
	float orthosize = 10;
	while (!glfwWindowShouldClose(window))
	{
		//cubeshader.use();
		//cubeshader.setVec3("slight.pos", camera.Position);
		//cubeshader.setVec3("slight.direction", (camera.Front));
		//cubeshader.setVec3("viewpos", (camera.Position));
		//floorshader.use();
		//floorshader.setInt("IsNM", map);
		//floorshader.setVec3("slight.pos", camera.Position);
		//floorshader.setVec3("slight.direction", (camera.Front));
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//First Pass to fill shadow map
		glBindFramebuffer(GL_FRAMEBUFFER, FBOShadow); //at location 0, bright parts at location 1
		glViewport(0, 0, SH_WIDTH, SH_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightprojection = glm::ortho(-orthosize, orthosize, -orthosize, orthosize, -orthosize, orthosize * 2);
		glm::mat4 lightview = glm::lookAt(lightdirection * glm::vec3(-1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightspacematrix = lightprojection * lightview;
		shadowmapshader.use();
		shadowmapshader.setMat4("lightspacematrix", lightspacematrix);

		renderer.cube1.CreateCube();
		renderer.floor1.CreateFloor();
		renderer.cube1.RenderCube(shadowmapshader);
		renderer.floor1.RenderFloor(shadowmapshader);

		//Render to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glDisable(GL_DEPTH_TEST);
		renderer.quad1.CreateQuad();
		renderer.quad1.RenderQuad(depthpostprocess, depthmap);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		if (map == 1) { map = 0; std::cout << "normal enabled" << std::endl; }
		else { map = 1; }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void SetFBOColourAndDepth() {
	glGenFramebuffers(1, &myFBOColourAndDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, myFBOColourAndDepth);
	glGenTextures(2, colourattachment);
	for (int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, colourattachment[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colourattachment[i], 0);
	}

	//depth
	glGenTextures(1, &depthattachment);
	glBindTexture(GL_TEXTURE_2D, depthattachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthattachment, 0);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SetFBOBlur() {
	glGenFramebuffers(1, &myFBOBlur);
	glBindFramebuffer(GL_FRAMEBUFFER, myFBOBlur);
	glGenTextures(1, &blurredtexture);

	glBindTexture(GL_TEXTURE_2D, blurredtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurredtexture, 0);
}

void SetFBODoF() {
	glGenFramebuffers(1, &myFBODoF);
	glBindFramebuffer(GL_FRAMEBUFFER, myFBODoF);
	glGenTextures(1, &doftexture);

	glBindTexture(GL_TEXTURE_2D, doftexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurredtexture, 0);
}
void SetFBODepth() {
	glGenFramebuffers(1, &FBOShadow);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOShadow);
	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SH_WIDTH, SH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}