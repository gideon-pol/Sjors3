#pragma once
#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "EBO.h"
#include "shader.h"
#include "mesh.h"
#include "component.h"

class MeshRenderer : public Component {
private:
	Mesh _mesh;
	VAO _VAO;
	EBO _EBO;

	void _Draw(Shader shader);

public:
	Shader shader;

	MeshRenderer();
	~MeshRenderer();

	void SetMesh(Mesh mesh);
	const Mesh& GetMesh() { return _mesh; }
	void Draw();
	void Draw(Shader shader);
};

#endif