#pragma once
#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Core.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class SubMesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indeces;

	SubMesh() { };

	SubMesh(std::vector<Vertex> verts) {
		vertices = verts;
	}

	uint64_t GetSize() {
		if (indeces.size() != 0) return indeces.size();
		else return vertices.size();
	}
};

class Mesh {
public:
	std::vector<SubMesh> subMeshes = {};

	Mesh() { };

	Mesh(std::vector<Vertex> verts) {
		subMeshes.clear();
		subMeshes.push_back(SubMesh(verts));
	}

	void AddSubMesh(SubMesh sub) {
		subMeshes.push_back(sub);
	}

	SubMesh* GetSubMesh(uint16_t index) {
		if (subMeshes.size() > index) {
			return &subMeshes[index];
		}
		else {
			throw new _exception();
		}
	}

	operator SubMesh*() {
		return GetSubMesh(0);
	}

	uint64_t GetSize() {
		//if (indeces.size() != 0) return indeces.size();
		//else return vertices.size();
		return 0;
	}
};



#endif