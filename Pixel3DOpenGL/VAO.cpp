#include "VAO.h"

VAO::VAO(){}

//Not very elegant
void VAO::Init() {
	//Tell OpenGL ID should reference a vertex array
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}

void VAO::LinkAttribute(VBO VBO1, GLuint layout, GLuint numcomponents, GLenum type, GLsizei stride, const GLvoid* pointer) {
	VBO1.Bind();
	glVertexAttribPointer(layout, numcomponents, type, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(layout);
	VBO1.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}