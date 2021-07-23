#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

class Texture {
public:
	GLuint ID;

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum type);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
