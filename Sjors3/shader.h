#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <map>
#include <typeindex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::string get_file_contents(const char* filename);

enum class ShaderType { PROGRAM = -1, VERTEX = 0, FRAGMENT = 1};

class Shader { 
public:
	GLuint ID = 0;

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

	std::map<std::string, GLenum> GetUniforms();

private:
	void _createShader(const char* vertexSource, const char* fragSource);
	bool _checkCompile(ShaderType shaderType);
	std::map<std::string, std::pair<GLenum, GLint>> _uniforms;
};

#endif