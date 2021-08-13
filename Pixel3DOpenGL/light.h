#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "component.h"

enum class LightType { Directional=0, Point=1, Spot=2 };

class Light : public Component,  public std::enable_shared_from_this<Light>{
public:
	LightType type = LightType::Directional;

	glm::vec3 color = glm::vec3(1);
	float intensity = 1;

	Light() {};
	~Light();

	void Init();

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetDirectionalLightMatrix();
};
#endif