#pragma once
#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H

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

#include "renderer.h"

class SkyboxRenderer : public RenderComponent, public std::enable_shared_from_this<SkyboxRenderer> {
public:
	SkyboxRenderer();
	~SkyboxRenderer();

	void Init();

	void Draw();
private:
	Ref<Mesh> _mesh;
	VAO _VAO;
};

#endif

#endif