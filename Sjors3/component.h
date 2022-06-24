#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <typeindex>
#include "Core.h"
#include "shader.h"

class Object;

class Component {
public:
	bool enabled = true;
	Object* object;
	std::type_index type = typeid(Component);

	virtual ~Component() { };
	virtual void Init() {};
};

class RenderComponent : public Component {
public:
	bool castShadows = true;

	virtual void Draw() { };
	virtual void Draw(Ref<Shader>) { };
	virtual glm::mat4 GetModelMatrix() { return glm::mat4(); };
};

#endif