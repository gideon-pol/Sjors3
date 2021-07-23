#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::string get_file_contents(const char* filename);

enum class ShaderType { PROGRAM=-1, VERTEX=0, FRAGMENT=1};

class Shader { 
public:
	GLuint ID;

	Shader() {};
	Shader(const char* vertexFile, const char* fragFile);
	Shader(const char* shaderFile);

	void Activate();
	void Delete();
	void SetFloatParameter(std::string name, float value);
	void SetIntParameter(std::string name, int value);
	void SetMat4Parameter(std::string name, glm::f32* mat);
	void SetVec2Parameter(std::string name, glm::vec2 vec);
	void SetVec3Parameter(std::string name, glm::vec3 vec);

private:
	void CreateShader(const char* vertexSource, const char* fragSource);
	void CheckCompile(ShaderType shaderType);
};

#endif