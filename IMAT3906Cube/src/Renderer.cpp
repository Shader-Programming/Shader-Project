#include "../include/Renderer.h"
Renderer::Renderer(unsigned int scrw,unsigned int scrh) {
	// settings
	const unsigned int SCR_WIDTH = 1200;
	const unsigned int SCR_HEIGHT = 900;
	screenH = scrh;
	screenW = scrw;
}
void Renderer::RenderScene(Shader& shader,Camera camera) {
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenW / (float)screenH, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setVec3("viewpos", camera.Position);
	//Render
}