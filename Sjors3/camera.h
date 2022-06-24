#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "object.h"
#include "shader.h"

class Camera : public Object {
public:
	int Width = 1000;
	int Height = 1000;
	float Fov = 45;
	Camera(float fov, int width, int height);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 CalculateVPMatrix();
	glm::mat4 GetVPMatrix();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetLeft();
	void SetShaderMatrices(Shader &shader);
};

#endif