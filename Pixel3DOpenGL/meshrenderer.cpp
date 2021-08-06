#include <glm/glm.hpp>
#include "meshrenderer.h"
#include "renderer.h"

MeshRenderer::MeshRenderer() {
	Renderer::AddRenderComponent(this);
	type = typeid(MeshRenderer);
}

MeshRenderer::~MeshRenderer() {
	_VAO.Delete();
	_EBO.Delete();
	Renderer::RemoveRenderComponent(this);
}

void MeshRenderer::SetMesh(Mesh mesh) {
	_mesh = mesh;

	_VAO = VAO();
	_VAO.Init();
	VBO VBO1 = VBO(mesh);

	if (mesh.indeces.size() == 0) _EBO.Unbind();
	else _EBO = EBO(mesh);

	_VAO.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
	_VAO.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	_VAO.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	_VAO.Unbind();
	VBO1.Unbind();
	_EBO.Unbind();
}

void MeshRenderer::Draw() {
	_Draw(material.GetParameterizedShader());
}

void MeshRenderer::Draw(Shader shader) {
	_Draw(shader);
}

void MeshRenderer::_Draw(Shader shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, object->position);
	model = glm::rotate(model, glm::radians<float>(object->rotation.y), glm::vec3(0, 0, 1.0f));
	model = glm::rotate(model, glm::radians<float>(object->rotation.x), glm::vec3(1.0f, 0, 0));
	model = glm::rotate(model, glm::radians<float>(object->rotation.z), glm::vec3(0, 1.0f, 0));
	glm::mat4 VP = Renderer::activeCamera->GetVPMatrix();
	glm::mat4 V = Renderer::activeCamera->GetViewMatrix();
	glm::mat4 P = Renderer::activeCamera->GetProjectionMatrix();
	glm::mat4 MVP = VP * model;
	shader.Activate();
	shader.SetMat4Parameter("MVP", glm::value_ptr(MVP));
	shader.SetMat4Parameter("model", glm::value_ptr(model));
	shader.SetMat4Parameter("view", glm::value_ptr(V));
	shader.SetMat4Parameter("proj", glm::value_ptr(P));

	Renderer::Draw(_VAO, _mesh.GetSize(), shader);
}