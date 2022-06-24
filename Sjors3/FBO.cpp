#include "FBO.h"

FBO::FBO() {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Delete() {
	glDeleteFramebuffers(1, &ID);
}

void FBO::GenenerateTexture(int width, int height, GLenum slot, GLenum internalFormat, GLenum format, GLenum texType, GLenum wrapMode, int count) {
	for (int i = 0; i < count; i++) {
		glGenTextures(1, &TexID[i]);
		glBindTexture(GL_TEXTURE_2D, TexID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, texType, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		if (wrapMode == GL_CLAMP_TO_BORDER) {
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, slot + i, GL_TEXTURE_2D, TexID[i], 0
		);
	}
}

void FBO::BindTexture(int index) {
	glBindTexture(GL_TEXTURE_2D, TexID[index]);
}

void FBO::DeleteTexture(int index) {
	glBindTexture(GL_TEXTURE_2D, 0);
	if (index == -1) {
		for (int i = 0; i < 10; i++) {
			glDeleteTextures(1, &TexID[i]);
		}
	}
	else {
		glDeleteTextures(1, &TexID[index]);
	}
}