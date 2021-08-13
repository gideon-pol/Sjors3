#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size) {
	//Tell OpenGL ID should reference an index buffer
	glGenBuffers(1, &ID);
	//Bind the ID to the vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

	Size = size;
	//Load data into buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::EBO(SubMesh sub) {
	//Tell OpenGL ID should reference an index buffer
	glGenBuffers(1, &ID);
	//Bind the ID to the vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

	//Load data into buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sub.indeces.size() * sizeof(int), sub.indeces.data(), GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}