#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <typeindex>
#include "component.h"
#include "ECS.h"

class Object {
public:
	bool enabled = true;
	uint64_t id;

	//Object();
	~Object();

	template<typename T>
	T* AddComponent() {
		T* newComponent = new T();
		newComponent->object = this;
		components.push_back((Component*)newComponent);
		return newComponent;
	}

	template<typename T>
	void RemoveComponent() {
		for (int i = 0; i < components.size(); i++) {
			std::cout << "Hi" << std::endl;
			if (components[i]->type == typeid(T)) {
				delete components[i];
				components.erase(std::next(components.begin(), i));
				return;
			}
		}
	}

	template<typename T>
	T* GetComponent() {
		for (int i = 0; i < components.size(); i++) {
			if (components[i]->type == typeid(T)) {
				return (T*)components[i];
			}
		}
	}

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(0);
private:
	std::vector<Component*> components;
	//std::vector<std::pair<Component*> components;
};

#endif