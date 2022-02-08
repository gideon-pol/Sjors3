#include <glm/glm.hpp>
#include "renderer.h"
#include "mesh.h"
#include "OBJloader.h"

glm::vec2 Renderer::resolution = glm::vec2(1000);
Camera* Renderer::activeCamera = NULL;
std::vector<Ref<RenderComponent>> Renderer::renderComponents = {};
std::vector<Ref<Light>> Renderer::lights = {};

std::vector<Vertex> quad = {
	Vertex {glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0)}, // bottom left
	Vertex {glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)}, // bottom right
	Vertex {glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)}, // top left

	Vertex {glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)},
	Vertex {glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1)}, // top right
	Vertex {glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)}
};

Ref<Mesh> quadMesh;
VAO quadVAO;
VBO quadVBO;

Ref<Mesh> skyboxMesh;
Ref<Shader> skyboxShader;
VAO skyboxVAO;
VBO skyboxVBO;

void Renderer::Init() {
	quadMesh = Ref<Mesh>(new Mesh(quad));
	quadVAO = VAO();
	quadVAO.Init();
	quadVBO = VBO(quadMesh->GetSubMesh(0));
	quadVAO.LinkAttribute(quadVBO, 0, 3, GL_FLOAT, 14 * sizeof(float), 0);
	quadVAO.LinkAttribute(quadVBO, 1, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	quadVAO.LinkAttribute(quadVBO, 2, 2, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	quadVAO.LinkAttribute(quadVBO, 3, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(8 * sizeof(float)));
	quadVAO.LinkAttribute(quadVBO, 4, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(11 * sizeof(float)));
	quadVAO.Unbind();
	quadVBO.Unbind();

	skyboxMesh = loadObj("assets/models/cube.obj");
	skyboxVAO = VAO();
	skyboxVAO.Init();
	skyboxVBO = VBO(skyboxMesh->GetSubMesh(0));
	skyboxVAO.LinkAttribute(skyboxVBO, 0, 3, GL_FLOAT, 14 * sizeof(float), 0);
	skyboxVAO.LinkAttribute(skyboxVBO, 1, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	skyboxVAO.LinkAttribute(skyboxVBO, 2, 2, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	skyboxVAO.LinkAttribute(skyboxVBO, 3, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(8 * sizeof(float)));
	skyboxVAO.LinkAttribute(skyboxVBO, 4, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(11 * sizeof(float)));
	skyboxVAO.Unbind();
	skyboxVBO.Unbind();

	skyboxShader = Ref<Shader>(new Shader("assets/shaders/skybox.shader"));
}

void Renderer::Clear() {
	glClearColor(0.3, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(VAO VAO1, EBO EBO1) {
	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, EBO1.Size, GL_UNSIGNED_INT, 0);
}

void Renderer::Draw(VAO VAO1, size_t count) {
	VAO1.Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
	VAO1.Unbind();
}

void Renderer::DrawQuad(Ref<Shader> shader) {
	Draw(quadVAO, 6);
}

void Renderer::DrawScene() {
	activeCamera->CalculateVPMatrix();

	for (int i = 0; i < renderComponents.size(); i++) {
		if (renderComponents[i]->enabled) {
			renderComponents[i]->Draw();
		}
	}
}

void Renderer::DrawSceneShadowMaps(Ref<Shader> depthShader) {
	for (Ref<Light> light : lights) {
		if (!light->castShadows) continue;
		if (light->type == LightType::Directional) {
			activeCamera->CalculateVPMatrix();

			glm::mat4 VP = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -100.0f, 100.0f) * glm::mat4(glm::mat3(Renderer::activeCamera->GetViewMatrix()));
			glm::mat4 lightSpaceMat = light->GetDirectionalLightMatrix();
			depthShader->Activate();

			for (int i = 0; i < renderComponents.size(); i++) {
				if (renderComponents[i]->enabled && renderComponents[i]->castShadows) {
					glm::mat4 MVP = lightSpaceMat * renderComponents[i]->GetModelMatrix();
					depthShader->SetMat4Parameter("MVP", glm::value_ptr(MVP));

					(renderComponents[i])->Draw(depthShader);
				}
			}
		}

		return;
	}
}

void Renderer::DrawSkybox(Ref<Skybox> skybox) {
	skyboxShader->Activate();
	glActiveTexture(GL_TEXTURE0);
	skybox->Bind();

	glm::mat4 VP = Renderer::activeCamera->GetProjectionMatrix() * glm::mat4(glm::mat3(Renderer::activeCamera->GetViewMatrix()));
	skyboxShader->SetMat4Parameter("VP", glm::value_ptr(VP));
	skyboxShader->SetFloatParameter("exposure", skybox->exposure);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	Draw(skyboxVAO, skyboxMesh->GetSubMesh(0)->GetSize());
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

}