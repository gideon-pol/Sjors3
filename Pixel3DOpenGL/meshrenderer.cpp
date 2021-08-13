#include <glm/glm.hpp>
#include "meshrenderer.h"
#include "renderer.h"

MeshRenderer::MeshRenderer() {
	std::cout << "Meshrenderer constructor called" << std::endl;
	type = typeid(MeshRenderer);
}

MeshRenderer::~MeshRenderer() {
	std::cout << "Meshrenderer destructor called" << std::endl;
	
	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
		_VAOS[i].Delete();
	}
	
	_EBO.Delete();
	std::cout << "Meshrenderer destructor finished" << std::endl;
}

void MeshRenderer::Init() {
	Renderer::AddRenderComponent(this->shared_from_this());
}

void Destroy(Ref<MeshRenderer> mrenderer) {
	Renderer::RemoveRenderComponent(mrenderer);
	mrenderer->object->RemoveComponent<MeshRenderer>(mrenderer);
}

void MeshRenderer::SetMesh(Ref<Mesh> mesh) {
	_mesh = mesh;

	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {

		_VAOS[i] = VAO();
		_VAOS[i].Init();

		VBO VBO1 = VBO(_mesh->GetSubMesh(i));

		if ( _mesh->subMeshes[0].indeces.size() == 0) _EBO.Unbind();
		//else _EBO = EBO(mesh);

		_VAOS[i].LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
		_VAOS[i].LinkAttribute(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		_VAOS[i].LinkAttribute(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		_VAOS[i].Unbind();
		VBO1.Unbind();
		_VAOS[i].Unbind();
	}
}

void MeshRenderer::Draw(Ref<Shader> shader) {
	_Draw(shader);
}

void MeshRenderer::_Draw(Ref<Shader> shader) {
	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, object->position);
		model = glm::rotate(model, glm::radians<float>(object->rotation.y), glm::vec3(0, 0, 1.0f));
		model = glm::rotate(model, glm::radians<float>(object->rotation.x), glm::vec3(1.0f, 0, 0));
		model = glm::rotate(model, glm::radians<float>(object->rotation.z), glm::vec3(0, 1.0f, 0));
		glm::mat4 VP = Renderer::activeCamera->GetVPMatrix();
		glm::mat4 V = Renderer::activeCamera->GetViewMatrix();
		glm::mat4 P = Renderer::activeCamera->GetProjectionMatrix();
		glm::mat4 MVP = VP * model;
		shader->Activate();
		shader->SetMat4Parameter("MVP", glm::value_ptr(MVP));
		shader->SetMat4Parameter("model", glm::value_ptr(model));
		shader->SetMat4Parameter("view", glm::value_ptr(V));
		shader->SetMat4Parameter("proj", glm::value_ptr(P));
		Renderer::Draw(_VAOS[i],  _mesh->GetSubMesh(0)->GetSize());
	}
}

void MeshRenderer::Draw() {
	for (int i = 0; i <  _mesh->subMeshes.size(); i++) {
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
		Renderer::Draw(_VAOS[i],  _mesh->GetSubMesh(i)->GetSize());
	}
}