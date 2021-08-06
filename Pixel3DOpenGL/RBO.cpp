#include "RBO.h"

RBO::RBO(int width, int height) {
	glGenRenderbuffers(1, &ID);
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void RBO::Bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RBO::BindToFBO() {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ID);
}

void RBO::Unbind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::Delete() {
	GLuint rbos = { ID };
	glDeleteRenderbuffers(1, &rbos);
}