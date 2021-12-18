#include "../include/Renderer.h"
Renderer::Renderer(unsigned int scrw,unsigned int scrh) {
	// settings
	const unsigned int SCR_WIDTH = 1200;
	const unsigned int SCR_HEIGHT = 900;
	screenH = scrh;
	screenW = scrw;
	LoadTextureFiles();
	CreateObjects();
}
void Renderer::RenderScene(Shader& shader, Shader& shader2, Camera camera) {
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenW / (float)screenH, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setVec3("viewpos", camera.Position);
	shader2.use();
	shader2.setMat4("projection", projection);
	shader2.setMat4("view", view);
	shader2.setMat4("model", model);
	shader2.setVec3("viewpos", camera.Position);
	//Render
	cube1.RenderCube(shader);
	floor1.RenderFloor(shader2);
}

void Renderer::RenderLights(Shader& shader, Camera camera) {
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenW / (float)screenH, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
}
void Renderer::CreateObjects() {
	cube1.CreateCube();
	floor1.CreateFloor();
	quad1.CreateQuad();
}
void Renderer::LoadTextureFiles() {
	cube1.cubediff = LoadTexture("..\\Textures\\metalPlate\\diffuse.jpg");
	cube1.cubenorm = LoadTexture("..\\Textures\\metalPlate\\normal.jpg");
	cube1.cubespec = LoadTexture("..\\Textures\\metalPlate\\specular.jpg");
	floor1.floordiff = LoadTexture("..\\Textures\\wood\\diffuse.jpg");
	floor1.floornorm = LoadTexture("..\\Textures\\wood\\normal.jpg");
	floor1.floorspec = LoadTexture("..\\Textures\\wood\\specular.jpg");
	floor1.floordisp = LoadTexture("..\\Textures\\wood\\displacement.jpg");
}

unsigned int Renderer::LoadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrcomps;
	unsigned char* data = stbi_load(path, &width, &height, &nrcomps, 0);

	if (data) {
		GLenum format;
		switch (nrcomps)
		{
		case(1):
			format = GL_RED;
			break;

		case(3):
			format = GL_RGB;
			break;

		case(4):
			format = GL_RGBA;
			break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		std::cout << "loaded at path: " << path << std::endl;
	}
	else {
		stbi_image_free(data);
		std::cout << "error" << std::endl;
	}
	return textureID;
}