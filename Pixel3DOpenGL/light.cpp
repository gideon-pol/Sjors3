#include "light.h"
#include "renderer.h"

void Light::Init() {
	Renderer::AddLight(this->shared_from_this());
}

void Destroy(Ref<Light> light) {
	Renderer::RemoveLight(light);
	light->object->RemoveComponent<Light>(light);
}

glm::mat4 Light::GetProjectionMatrix() {
	if (type == LightType::Directional) {
		return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.01f, 100.0f);
	}
}

glm::mat4 Light::GetViewMatrix() {
	if (type == LightType::Directional) {
		std::cout << object->position.z << std::endl;
		return glm::lookAt(object->position, glm::vec3(0), glm::vec3(0, 1, 0));
	}
}

glm::mat4 Light::GetDirectionalLightMatrix() {
	return glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 100.0f) * glm::lookAt(object->position, glm::vec3(0), glm::vec3(0, 1, 0));
}