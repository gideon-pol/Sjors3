#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <typeindex>
#include "Core.h"
#include "component.h"
#include "ECS.h"

class Object {
public:
	bool enabled = true;
	uint64_t id;

	//Object();
	~Object();

	template<typename T>
	Ref<T> AddComponent() {
		Ref<T> newComponent = std::make_shared<T>();
		newComponent->object = this;
		newComponent->Init();
		_components.push_back(newComponent);
		return newComponent;
	}

	template<typename T>
	void RemoveComponent(Ref<T> component) {
		for (int i = 0; i < _components.size(); i++) {
			if (_components[i] == component) {
				_components.erase(std::next(_components.begin(), i));
				return;
			}
		}
	}

	template<typename T>
	Ref<T> GetComponent() {
		for (int i = 0; i < _components.size(); i++) {
			if (_components[i]->type == typeid(T)) {
				return _components[i];
			}
		}
	}

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(0);
private:
	std::vector<Ref<Component>> _components;
};

#endif