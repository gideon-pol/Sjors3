#include "light.h"
#include "renderer.h"

Light::Light() {
	Renderer::AddLight(this);
}

Light::~Light() {
	Renderer::RemoveLight(this);
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
	return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.01f, 100.0f) * glm::lookAt(object->position, glm::vec3(0), glm::vec3(0, 1, 0));
}