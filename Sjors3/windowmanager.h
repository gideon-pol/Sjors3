#pragma once
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <GLFW/glfw3.h>
#include "renderer.h"

class WindowManager {
public:
	static bool CreateWindow(const char* windowName, glm::vec2 size);

	static GLFWwindow* GetWindow();

	static glm::vec2 GetSize();

	static void Display();

	static bool Closing();

	static bool IsFocused();

	static void Destroy();


private:
	static GLFWwindow* _window;
	static glm::vec2 _size;
	static bool _inFocus;

	static void _windowFocusCallback(GLFWwindow* window, int focused);
};

#endif