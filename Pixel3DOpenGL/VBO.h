#pragma once
#ifndef VBO_H
#define VBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.h"

class VBO {

public:
	GLuint ID;

	VBO() {};
	VBO(Mesh mesh);
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif