#pragma once
#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Core.h"
#include "mesh.h"

SubMesh getSubMesh(std::vector<glm::vec3>* vertsPos, std::vector<glm::vec3>* vertsNormals, std::vector<glm::vec2>* texCoords, std::vector<int>* faceIndeces, std::vector<int>* texIndeces, std::vector<int>* nrmIndeces);

Ref<Mesh> loadObj(const char* fileName);

#endif