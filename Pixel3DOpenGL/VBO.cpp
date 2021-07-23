#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	//Tell OpenGL ID should reference a vertex data buffer
	glGenBuffers(1, &ID);
	//Bind the ID to the vertex array
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//Load data into buffer
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO(Mesh mesh) {
	//Tell OpenGL ID should reference a vertex data buffer
	glGenBuffers(1, &ID);
	//Bind the ID to the vertex array
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//Load data into buffer
	std::cout << "Number of vertices: " << mesh.vertices.size() << std::endl;
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}