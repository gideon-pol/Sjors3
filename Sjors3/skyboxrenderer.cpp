#include "skyboxrenderer.h"
#include "OBJloader.h"

SkyboxRenderer::SkyboxRenderer() {
	type = typeid(SkyboxRenderer);
}


SkyboxRenderer::~SkyboxRenderer() {
	_VAO.Delete();
}

void SkyboxRenderer::Init() {
	Renderer::AddRenderComponent(this->shared_from_this());
	_mesh = loadObj("assets/models/cube.obj");

	_VAO = VAO();
	_VAO.Init();

	VBO VBO1 = VBO(_mesh->GetSubMesh(0));

	_VAO.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 14 * sizeof(float), 0);
	_VAO.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	_VAO.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	_VAO.LinkAttribute(VBO1, 3, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(8 * sizeof(float)));
	_VAO.LinkAttribute(VBO1, 4, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(11 * sizeof(float)));
	_VAO.Unbind();
	VBO1.Unbind();
	_VAO.Unbind();
}

void Destroy(Ref<SkyboxRenderer> srenderer) {
	Renderer::RemoveRenderComponent(srenderer);
	srenderer->object->RemoveComponent<SkyboxRenderer>(srenderer);
}

void SkyboxRenderer::Draw() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, object->position);
	model = glm::rotate(model, glm::radians<float>(object->rotation.y), glm::vec3(0, 0, 1.0f));
	model = glm::rotate(model, glm::radians<float>(object->rotation.x), glm::vec3(1.0f, 0, 0));
	model = glm::rotate(model, glm::radians<float>(object->rotation.z), glm::vec3(0, 1.0f, 0));
	glm::mat4 VP = Renderer::activeCamera->GetVPMatrix();
	glm::mat4 V = Renderer::activeCamera->GetViewMatrix();
	glm::mat4 P = Renderer::activeCamera->GetProjectionMatrix();
	glm::mat4 MVP = VP * model;
	Ref<Shader> shader = materials[i]->GetParameterizedShader();
	shader->Activate();
	shader->SetMat4Parameter("MVP", glm::value_ptr(MVP));
	shader->SetMat4Parameter("model", glm::value_ptr(model));
	//shader->SetMat4Parameter("view", glm::value_ptr(V));
	//shader->SetMat4Parameter("proj", glm::value_ptr(P));
	Renderer::Draw(_VAOS[i], _mesh->GetSubMesh(i)->GetSize());
}