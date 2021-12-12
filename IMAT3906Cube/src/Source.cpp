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



//// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void SetFBOColour();




// camera
Camera camera(glm::vec3(0,0,9));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int map = 0;

unsigned int myFBO;
unsigned int colourattachment;

void SetUniform(Shader& shader, Shader& shader2, Shader& shader3) {
	shader.use();
	//Cube
	//dir light
	glm::vec3 lightdirection = glm::vec3(0, -1, 0);
	glm::vec3 lightcolor = glm::vec3(1.0, 1.0, 1.0);

	shader.setVec3("lightcol", lightcolor);
	shader.setVec3("lightdir", lightdirection);

	//spot light
	glm::vec3 plightpos = glm::vec3(0.0, 3.0, -4.0);
	glm::vec3 plightcol = glm::vec3(5.0, 5.0, 5.0);
	float kc = 1.0f;
	float kl = 0.22f;
	float ke = 0.2f;
	shader.setVec3("plight.pos", plightpos);
	shader.setVec3("plight.col", plightcol);
	shader.setFloat("plight.kc", kc);
	shader.setFloat("plight.kl", kl);
	shader.setFloat("plight.ke", ke);

	shader.setVec3("slight.col", glm::vec3(1.0f, 1.0f, 1.0f));
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

	//spot light
	shader2.setVec3("plight.pos", plightpos);
	shader2.setVec3("plight.col", plightcol);
	shader2.setFloat("plight.kc", kc);
	shader2.setFloat("plight.kl", kl);
	shader2.setFloat("plight.ke", ke);

	shader2.setVec3("slight.col", glm::vec3(1.0f, 1.0f, 1.0f));
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
	Shader quadshader("..\\shaders\\PP.vs", "..\\shaders\\PP.fs");
	SetUniform(cubeshader,floorshader,quadshader);
	SetFBOColour();
	while (!glfwWindowShouldClose(window))
	{
		cubeshader.use();
		cubeshader.setVec3("slight.pos", camera.Position);
		cubeshader.setVec3("slight.direction", (camera.Front));
		cubeshader.setVec3("viewpos", (camera.Position));
		floorshader.use();
		floorshader.setInt("IsNM", map);
		floorshader.setVec3("slight.pos", camera.Position);
		floorshader.setVec3("slight.direction", (camera.Front));
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//First Pass to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, myFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glEnable(GL_DEPTH_TEST);
		renderer.RenderScene(cubeshader,floorshader, camera);

		//Second Pass to Screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		renderer.quad1.RenderQuad(quadshader,colourattachment);

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

void SetFBOColour() {
	glGenFramebuffers(1, &myFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, myFBO);
	glGenTextures(1, &colourattachment);
	glBindTexture(GL_TEXTURE_2D, colourattachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourattachment, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not compelte!" << std::endl;
	}
}