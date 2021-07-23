#pragma once
#pragma once
#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indeces;

	Mesh() {};
	Mesh(Vertex verts[]) {
		//Inefficient better fix this
		for (int i = 0; i < sizeof(verts) / sizeof(Vertex); i++) {
			vertices.push_back(verts[i]);
		}
	}

	Mesh(std::vector<Vertex> verts) {
		vertices = verts;
	}

	uint64_t GetSize() {
		if (indeces.size() != 0) return indeces.size();
		else return vertices.size();
	}
};

#endif