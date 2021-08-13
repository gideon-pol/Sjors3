#pragma once
#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Core.h"
#include "mesh.h"

SubMesh getSubMesh(std::vector<glm::vec3>* vertsPos, std::vector<glm::vec3>* vertsNormals, std::vector<glm::vec2>* texCoords, std::vector<int>* faceIndeces, std::vector<int>* texIndeces, std::vector<int>* nrmIndeces) {
    SubMesh sub = SubMesh();

    for (unsigned int i = 0; i < faceIndeces->size(); i++)
    {
        glm::vec3 vertexPos = vertsPos->at(faceIndeces->at(i));
        glm::vec3 vertexNormal = vertsNormals->at(nrmIndeces->at(i));
        glm::vec2 texCoord = texCoords->at(texIndeces->at(i));

        Vertex vert = { vertexPos, vertexNormal, texCoord };
        sub.vertices.push_back(vert);
        //std::cout << /*"Vertex position: " <<*/ vertexPos.x << ", " << vertexPos.y << ", " << vertexPos.z << ", " /*" Vertex normal: "*/ << vertexNormal.x << ", " << vertexNormal.y << ", " << vertexNormal.z << ", "/*" UV: "*/ << texCoord.x << ", " << texCoord.y << std::endl;
    }

    faceIndeces->clear();
    nrmIndeces->clear();
    texIndeces->clear();

    return sub;
}

static Ref<Mesh> loadObj(const char* fileName) {
    std::vector<glm::vec3> vertsPos;
    std::vector<glm::vec3> vertsNormals;
    std::vector<glm::vec2> texCoords;

    std::vector<int> faceIndeces;
    std::vector<int> texIndeces;
    std::vector<int> nrmIndeces;

    Ref<Mesh> mesh = std::make_shared<Mesh>();
    mesh->subMeshes.clear();

	std::ifstream file(fileName, std::ios::in);

    if (!file)
    {
        std::cerr << "Cannot open " << fileName << std::endl;
    }

    std::string line;
    int matGroupsFound = 0;
    while (std::getline(file, line))
    {
        //std::cout << "Line: " << line.substr(0, 2) << std::endl;
        if (line.substr(0, 2) == "v ") {
            std::stringstream lineStream(line.substr(2));
            glm::vec3 vert;
            float x, y, z;
            lineStream >> x;
            lineStream >> y;
            lineStream >> z;
            //std::cout << x << ":X " << y << ":Y " << z << ":Z" << std::endl;
            vert = glm::vec3(x, y, z);
            vertsPos.push_back(vert);
        }
        else if (line.substr(0, 2) == "vt") {

            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            double U, V;
            v >> U;
            v >> V;
            tex = glm::vec2(U, V);
            texCoords.push_back(tex);
        }
        else if (line.substr(0, 2) == "vn") {
            float x, y, z;
            const char* chh = line.c_str();
            sscanf_s(chh, "vn %f %f %f", &x, &y, &z);

            glm::vec3 normal = glm::vec3(x, y, z);

            vertsNormals.push_back(normal);
        }
        else if (line.substr(0, 2) == "f ") {
            int faceInA, faceInB, faceInC;
            int texInA, texInB, texInC;
            int nrmInA, nrmInB, nrmInC;
            const char* chh = line.c_str();
            sscanf_s(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &faceInA, &texInA, &nrmInA, &faceInB, &texInB, &nrmInB, &faceInC, &texInC, &nrmInC);
            //std::cout << a << " " << b << " " << c << std::endl;
            faceInA--;faceInB--;faceInC--;
            texInA--;texInB--;texInC--;
            nrmInA--;nrmInB--;nrmInC--;

            faceIndeces.push_back(faceInA);texIndeces.push_back(texInA);nrmIndeces.push_back(nrmInA);
            faceIndeces.push_back(faceInB);texIndeces.push_back(texInB);nrmIndeces.push_back(nrmInB);
            faceIndeces.push_back(faceInC);texIndeces.push_back(texInC);nrmIndeces.push_back(nrmInC);
        }
        else if (line.substr(0, 7) == "usemtl ") {
            matGroupsFound++;
            if (matGroupsFound == 1) {
                continue;
            }

            mesh->AddSubMesh(getSubMesh(&vertsPos, &vertsNormals, &texCoords, &faceIndeces, &texIndeces, &nrmIndeces));

            std::cout << "Got a group! Size: " << mesh->subMeshes[mesh->subMeshes.size() - 1].GetSize() << std::endl;
        }
    }

    mesh->AddSubMesh(getSubMesh(&vertsPos, &vertsNormals, &texCoords, &faceIndeces, &texIndeces, &nrmIndeces));

    std::cout << "Found " << matGroupsFound << " material groups" << std::endl;

    return mesh;
}

#endif