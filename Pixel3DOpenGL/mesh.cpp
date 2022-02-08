/*
#include "mesh.h"

SubMesh::SubMesh(Vertex verts[]) {
	for (int i = 0; i < sizeof(verts) / sizeof(Vertex); i++) {
		vertices.push_back(verts[i]);
	}
}

SubMesh::SubMesh(std::vector<Vertex> verts) {
	vertices = verts;
}

uint64_t SubMesh::GetSize() {
	if (indeces.size() != 0) return indeces.size();
	else return vertices.size();
}

Mesh::Mesh(Vertex verts[]) {
	subMeshes.clear();
	subMeshes.push_back(SubMesh(verts));
}

Mesh::Mesh(std::vector<Vertex> verts) {
	subMeshes.clear();
	subMeshes.push_back(SubMesh(verts));
}

void Mesh::AddSubMesh(SubMesh sub) {
	subMeshes.push_back(sub);
}

SubMesh* Mesh::GetSubMesh(uint16_t index) {
	if (subMeshes.size() > index) {
		return &subMeshes[index];
	}
	else {
		throw new _exception();
	}
}

Mesh::operator SubMesh* () {
	return GetSubMesh(0);
}

uint64_t Mesh::GetSize() {
	//if (indeces.size() != 0) return indeces.size();
	//else return vertices.size();
	return 0;
}
*/