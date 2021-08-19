#pragma once
#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "component.h"

class MeshRenderer : public RenderComponent, public std::enable_shared_from_this<MeshRenderer> {
public:
	Ref<Material> materials[20];

	MeshRenderer();
	~MeshRenderer();

	void Init();

	void SetMesh(Ref<Mesh> mesh);
	const Ref<Mesh> GetMesh() { return _mesh; }
	void Draw();
	void Draw(Ref<Shader> shader);
private:
	Ref<Mesh> _mesh;
	VAO _VAOS[20];
	EBO _EBO;

	void _draw(Ref<Shader> shader);
};

#endif