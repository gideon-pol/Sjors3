#pragma once
#ifndef FBO_H
#define FBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FBO {
public:
	GLuint ID;
	GLuint TexID[10];

	FBO();
	void Bind();
	void Unbind();
	void Delete();

	void GenenerateTexture(int width, int height, GLenum slot, GLenum interalFormat, GLenum texType, GLenum format, GLenum wrapMode, int count);
	void BindTexture(int index);
	void DeleteTexture(int index);
};

#endif