#pragma once
#ifndef VAO_H
#define VAO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VBO.h"

class VAO {
public:
	GLuint ID;

	VAO();
	void Init();

	void LinkAttribute(VBO VBO1, GLuint layout, GLuint numcomponents, GLenum type, GLsizei stride, const GLvoid* pointer);
	void Bind();
	void Unbind();
	void Delete();
};

#endif