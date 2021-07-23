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

void FBO::GenenerateTexture(int width, int height, GLenum slot, GLenum texType, int count) {
	for (int i = 0; i < count; i++) {
		glGenTextures(1, &TexID[i]);
		glBindTexture(GL_TEXTURE_2D, TexID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, texType, width, height, 0, texType, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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