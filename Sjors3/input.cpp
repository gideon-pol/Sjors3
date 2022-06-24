#include "input.h"

glm::vec2 Input::_mousePos = glm::vec2(0);
glm::vec2 Input::_mousePosDelta = glm::vec2(0);
glm::vec2 Input::_mouseScrollDelta = glm::vec2(0);
std::pair<bool, bool> Input::_mouseButtons[8] = {};

void Input::Init() {
	glfwSetMouseButtonCallback(WindowManager::GetWindow(), _mouseButtonCallback);
	glfwSetCursorPosCallback(WindowManager::GetWindow(), _mousePosCallback);
	glfwSetScrollCallback(WindowManager::GetWindow(), _mouseScrollCallback);
}

bool Input::GetKey(GLenum key) {
	return glfwGetKey(WindowManager::GetWindow(), key);
}

void Input::Poll() {
	if (WindowManager::IsFocused()) {
		_mousePosDelta = _mousePos - WindowManager::GetSize() / glm::vec2(2);
		glfwSetCursorPos(WindowManager::GetWindow(), WindowManager::GetSize().x / 2, WindowManager::GetSize().y / 2);
	}
	else {
		_mousePosDelta = glm::vec2(0);
	}

	_mouseScrollDelta = glm::vec2(0);

	for (int i = 0; i < sizeof(_mouseButtons) / sizeof(_mouseButtons[0]); i++) {
		_mouseButtons[i].second = false;
	}
}

bool Input::GetMouseButton(int button) {
	return _mouseButtons[button].first;
}

bool Input::GetMouseButtonDown(int button) {
	return _mouseButtons[button].second;
}

glm::vec2 Input::GetMousePosition() {
	return _mousePos;
}

glm::vec2 Input::GetMouseDelta() {
	return _mousePosDelta;
}

glm::vec2 Input::GetMouseScrollDelta() {
	return _mouseScrollDelta;
}

void Input::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		_mouseButtons[button].first = true;
		_mouseButtons[button].second = true;
	}
	else {
		_mouseButtons[button].first = false;
		_mouseButtons[button].second = false;
	}
}

void Input::_mousePosCallback(GLFWwindow* window, double x, double y) {
	_mousePos = glm::vec2(x, y);
}

void Input::_mouseScrollCallback(GLFWwindow* window, double x, double y) {
	_mouseScrollDelta = glm::vec2(x, y);
}