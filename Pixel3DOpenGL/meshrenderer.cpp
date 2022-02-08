#include <glm/glm.hpp>
#include "meshrenderer.h"
#include "renderer.h"

MeshRenderer::MeshRenderer() {
	type = typeid(MeshRenderer);
}

MeshRenderer::~MeshRenderer() {
	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
		_VAOS[i].Delete();
	}
	
	_EBO.Delete();
}

void MeshRenderer::Init() {
	Renderer::AddRenderComponent(this->shared_from_this());
}

void Destroy(Ref<MeshRenderer> mrenderer) {
	Renderer::RemoveRenderComponent(mrenderer);
	mrenderer->object->RemoveComponent<MeshRenderer>(mrenderer);
}

glm::mat4 MeshRenderer::GetModelMatrix() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, object->position);
	model = glm::rotate(model, glm::radians<float>(object->rotation.y), glm::vec3(0, 0, 1.0f));
	model = glm::rotate(model, glm::radians<float>(object->rotation.x), glm::vec3(1.0f, 0, 0));
	model = glm::rotate(model, glm::radians<float>(object->rotation.z), glm::vec3(0, 1.0f, 0));
	return model;
}

void MeshRenderer::SetMesh(Ref<Mesh> mesh) {
	_mesh = mesh;

	for (int i = 0; i < _mesh->subMeshes.size(); i++) {
		_VAOS[i] = VAO();
		_VAOS[i].Init();

		VBO VBO1 = VBO(_mesh->GetSubMesh(i));

		if (_mesh->subMeshes[0].indeces.size() == 0) _EBO.Unbind();
		//else _EBO = EBO(mesh);

		_VAOS[i].LinkAttribute(VBO1, 0, 3, GL_FLOAT, 14 * sizeof(float), 0);
		_VAOS[i].LinkAttribute(VBO1, 1, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		_VAOS[i].LinkAttribute(VBO1, 2, 2, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		_VAOS[i].LinkAttribute(VBO1, 3, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(8 * sizeof(float)));
		_VAOS[i].LinkAttribute(VBO1, 4, 3, GL_FLOAT, 14 * sizeof(float), (GLvoid*)(11 * sizeof(float)));
		_VAOS[i].Unbind();
		VBO1.Unbind();
		_VAOS[i].Unbind();
	}
}

void MeshRenderer::Draw(Ref<Shader> shader) {
	_draw(shader);
}

void MeshRenderer::_draw(Ref<Shader> shader) {
	glm::mat4 model = GetModelMatrix();
	glm::mat4 VP = Renderer::activeCamera->GetVPMatrix();
	glm::mat4 MVP = VP * model;

	//shader->SetMat4Parameter("MVP", glm::value_ptr(MVP));
	shader->SetMat4Parameter("model", glm::value_ptr(model));

	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
		Renderer::Draw(_VAOS[i], _mesh->GetSubMesh(i)->GetSize());
	}
}

void MeshRenderer::Draw() {
	glm::mat4 model = GetModelMatrix();
	glm::mat4 VP = Renderer::activeCamera->GetVPMatrix();
	glm::mat4 MVP = VP * model;

	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
		Ref<Shader> shader = materials[i]->GetParameterizedShader();
		shader->SetMat4Parameter("MVP", glm::value_ptr(MVP));
		shader->SetMat4Parameter("model", glm::value_ptr(model));
		Renderer::Draw(_VAOS[i], _mesh->GetSubMesh(i)->GetSize());
	}
}