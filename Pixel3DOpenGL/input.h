#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "windowmanager.h"

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

class Input {
public:
	static void Init();

	static bool GetKey(GLenum key);

	static void Poll();

	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);

	static glm::vec2 GetMousePosition();

	static glm::vec2 GetMouseDelta();

	static glm::vec2 GetMouseScrollDelta();

private:
	static glm::vec2 _mousePos;
	static glm::vec2 _mousePosDelta;
	static glm::vec2 _mouseScrollDelta;
	static std::pair<bool, bool> _mouseButtons[8];

	static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void _mousePosCallback(GLFWwindow* window, double x, double y);

	static void _mouseScrollCallback(GLFWwindow* window, double x, double y);
};

#endif