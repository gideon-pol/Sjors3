#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "EBO.h"
#include "shader.h"
#include "ECS.h"
#include "camera.h"

class Renderer {
public:
	static glm::vec2 resolution;
	static Camera* activeCamera;

	static void Init();
	static void Clear();
	static void Draw(VAO VAO1, EBO EBO1, Shader shader);
	static void Draw(VAO VAO1, size_t count, Shader shader);
	static void DrawQuad(Shader shader);
	static void DrawScene();
	static void DrawSceneShadowMap(Shader shader);

	template<typename T>
	static void AddRenderComponent(T* component) {
		renderComponents.push_back((Component*)component);
	}

	template<typename T>
	static void RemoveRenderComponent(T* component) {
		for (int i = 0; i < renderComponents.size(); i++) {
			if (renderComponents[i] == (Component*)component) {
				renderComponents.erase(std::next(renderComponents.begin(), i));
				return;
			}
		}
	}

private:
	static std::vector<Component*> renderComponents;
};

#endif#pragma once
