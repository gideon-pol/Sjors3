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
#include "FBO.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"

class Renderer {
public:
	static glm::vec2 resolution;
	static Camera* activeCamera;

	static void Init();
	static void Clear();
	static void Draw(VAO VAO1, EBO EBO1);
	static void Draw(VAO VAO1, size_t count);
	static void DrawQuad(Ref<Shader> shader);
	static void DrawScene();
	static void DrawSceneShadowMaps(Ref<Shader> shader);
	static void DrawSkybox(Ref<Skybox> skybox);

	template<typename T>
	static void AddRenderComponent(Ref<T> component) {
		renderComponents.push_back(component);
	}

	template<typename T>
	static void RemoveRenderComponent(Ref<T> component) {
		for (int i = 0; i < renderComponents.size(); i++) {
			if (renderComponents[i] == component) {
				renderComponents.erase(std::next(renderComponents.begin(), i));
				return;
			}
		}
	}

	static void AddLight(Ref<Light> light) {
		lights.push_back(light);
	}

	static void RemoveLight(Ref<Light> light) {
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i] == light) {
				lights.erase(std::next(lights.begin(), i));
			}
		}
	}

	static std::vector<Ref<Light>> lights;

private:
	static std::vector<Ref<RenderComponent>> renderComponents;
};

#endif
