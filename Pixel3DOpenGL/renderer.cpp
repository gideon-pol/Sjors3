#include "renderer.h"

glm::vec2 Renderer::resolution = glm::vec2(1000);
Camera* Renderer::activeCamera = NULL;
std::vector<RenderComponent*> Renderer::renderComponents = {};

std::vector<Vertex> quad = {
	Vertex {glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0)}, // bottom left
	Vertex {glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)}, // bottom right
	Vertex {glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)}, // top left

	Vertex {glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)},
	Vertex {glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1)}, // top right
	Vertex {glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)}
};

Mesh quadMesh = Mesh(quad);
VAO quadVAO;
VBO quadVBO;

void Renderer::Init() {
	quadVAO = VAO();
	quadVAO.Init();
	quadVBO = VBO(quadMesh);
	quadVAO.LinkAttribute(quadVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
	quadVAO.LinkAttribute(quadVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	quadVAO.LinkAttribute(quadVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	quadVAO.Unbind();
	quadVBO.Unbind();
}

void Renderer::Clear() {
	glClearColor(0.1,0.1,0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(VAO VAO1, EBO EBO1, Shader shader) {
	shader.Activate();
	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, EBO1.Size, GL_UNSIGNED_INT, 0);
}

void Renderer::Draw(VAO VAO1, size_t count, Shader shader) {
	shader.Activate();
	VAO1.Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
	VAO1.Unbind();
}

void Renderer::DrawQuad(Shader shader) {
	Draw(quadVAO, 6, shader);
}

void Renderer::DrawScene() {
	activeCamera->CalculateVPMatrix();
	for (int i = 0; i < renderComponents.size(); i++) {
		if (renderComponents[i]->enabled) {
			((MeshRenderer*)renderComponents[i])->Draw();
		}
	}
}

void Renderer::DrawSceneShadowMap(Shader depthShader) {
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_CW);
	for (int i = 0; i < renderComponents.size(); i++) {
		if (renderComponents[i]->enabled && renderComponents[i]->castShadows) {
			((MeshRenderer*)renderComponents[i])->Draw(depthShader);
	 	}
	}
	//glCullFace(GL_BACK);
}