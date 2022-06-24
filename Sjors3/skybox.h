#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include "Core.h"
#include "texture.h"

class Skybox {
public:
	float exposure = 1;

	Skybox(const char* image) {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

		int width, height, chCount;
		for (int i = 0; i < 6; i++) {
			std::string fileName = image;
			fileName += '0'+i;
			fileName += ".png";

			stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &chCount, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);

			if (!data) {
				std::cout << "Couldn't open skybox texture: " << fileName << std::endl;
			}

			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}

	void Unbind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Delete() {
		glDeleteTextures(1, &ID);
	}
private:
	GLuint ID = 0;
};

#endif