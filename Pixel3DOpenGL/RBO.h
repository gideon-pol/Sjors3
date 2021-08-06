#pragma once
#ifndef RBO_H
#define RBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RBO {
public:
	GLuint ID;

	RBO(int width, int height);
	void Bind();
	void BindToFBO();
	void Unbind();
	void Delete();
};

#endif