#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
	GLuint ID;

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum type) {
		int width, height, chCount;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* bytes = stbi_load(image, &width, &height, &chCount, 0);

		if (!bytes) {
			std::cout << "Couldn't open texture: " << image << std::endl;
		}

		glGenTextures(1, &ID);
		glActiveTexture(slot);
		glBindTexture(texType, ID);

		glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(texType, 0, format, width, height, 0, format, type, bytes);
		glGenerateMipmap(texType);

		stbi_image_free(bytes);
	}

	Texture(GLuint id) {
		ID = id;
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void Unbind() {
		glBindTexture(ID, 0);
	}

	void Delete() {
		glDeleteTextures(1, &ID);
	}
};

#endif
