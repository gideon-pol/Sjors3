#pragma once
#ifndef EBO_H
#define EBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh.h"

class EBO {

public:
	GLuint ID;
	size_t Size;

	EBO(GLuint* indices, GLsizeiptr size);
	EBO(Mesh mesh);
	EBO() {};

	void Bind();
	void Unbind();
	void Delete();
};

#endif