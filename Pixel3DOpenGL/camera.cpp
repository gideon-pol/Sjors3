#include "camera.h"

glm::mat4 VP;

Camera::Camera(float fov, int width, int height) {
	Width = width;
	Height = height;
	Fov = fov;
}

glm::mat4 Camera::GetViewMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::rotate(view, glm::radians<float>(rotation.y), glm::vec3(0, 0, 1.0f));
	view = glm::rotate(view, glm::radians<float>(rotation.x), glm::vec3(1.0f, 0, 0));
	view = glm::rotate(view, glm::radians<float>(rotation.z), glm::vec3(0, 1.0f, 0));
	view = glm::translate(view, -position);
	return view;
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(Fov), (float)(Width / Height), 0.1f, 1000.0f);
}

glm::mat4 Camera::CalculateVPMatrix() {
	VP = GetProjectionMatrix() * GetViewMatrix();
	return VP;
}

glm::mat4 Camera::GetVPMatrix() {
	return VP;
}

glm::vec3 Camera::GetForward() {
	return -glm::vec3(glm::inverse(GetViewMatrix())[2]);//normalize(glm::vec3(GetViewMatrix()[2]))* glm::vec3(1, 1, -1);
}

glm::vec3 Camera::GetUp() {
	return -glm::vec3(glm::inverse(GetViewMatrix())[1]);
}

glm::vec3 Camera::GetLeft() {
	return -glm::vec3(glm::inverse(GetViewMatrix())[0]);
}

void Camera::SetShaderMatrices(Shader &shader) {
	glm::mat4 view = GetViewMatrix();
	glm::mat4 proj = GetProjectionMatrix();

	shader.SetMat4Parameter("view", glm::value_ptr(view));
	shader.SetMat4Parameter("proj", glm::value_ptr(proj));
}