#include "windowmanager.h"

GLFWwindow* WindowManager::_window = nullptr;
glm::vec2 WindowManager::_size = glm::vec2(0);
bool WindowManager::_inFocus = false;

bool WindowManager::CreateWindow(const char* windowName, glm::vec2 size) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	_window = glfwCreateWindow(size.x, size.y, windowName, NULL, NULL);

	if (_window == NULL) {
		return false;
	}

	_size = size;
	glfwMakeContextCurrent(_window);
	glfwSetWindowFocusCallback(_window, _windowFocusCallback);
	glfwFocusWindow(_window);
	return true;
}

GLFWwindow* WindowManager::GetWindow() {
	return _window;
}

glm::vec2 WindowManager::GetSize() {
	return _size;
}

void WindowManager::Display() {
	glfwSwapBuffers(_window);
}

bool WindowManager::Closing() {
	return glfwWindowShouldClose(_window);
}

bool WindowManager::IsFocused() {
	return _inFocus;
}

void WindowManager::Destroy() {
	glfwDestroyWindow(_window);
}

void WindowManager::_windowFocusCallback(GLFWwindow* window, int focused) {
	_inFocus = focused;
}