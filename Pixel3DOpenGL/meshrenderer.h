#pragma once
#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "EBO.h"
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "component.h"

class MeshRenderer : public RenderComponent {
private:
	Mesh _mesh;
	VAO _VAO;
	EBO _EBO;

	void _Draw(Shader shader);

public:
	Material material;

	MeshRenderer();
	~MeshRenderer();

	void SetMesh(Mesh mesh);
	const Mesh& GetMesh() { return _mesh; }
	void Draw();
	void Draw(Shader shader);
};

#endif