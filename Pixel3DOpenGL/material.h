#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <typeindex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "utils.h"

class Material {
public:
	Material() {};

	~Material() {
		std::map<std::string, std::pair<void*, GLenum>>::iterator it;

		for (it = _parameters.begin(); it != _parameters.end(); it++) {
			delete it->second.first;
		}
	}

	Material(const Material& mat) {
		std::map<std::string, std::pair<void*, GLenum>>::iterator it;
		for (it = _parameters.begin(); it != _parameters.end(); it++) {
			delete it->second.first;
		}
		_parameters.clear();

		std::map<std::string, std::pair<void*, GLenum>>::const_iterator it2;
		for (it2 = mat._parameters.begin(); it2 != mat._parameters.end(); it2++) {
			if (it2->second.second == GL_FLOAT) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new float(*(float*)it2->second.first), GL_FLOAT)));
			}
			else if (it2->second.second == GL_FLOAT_MAT4) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new glm::mat4(*(glm::mat4*)it2->second.first), GL_FLOAT_MAT4)));
			}
			else if (it2->second.second == GL_FLOAT_VEC3) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new glm::vec3(*(glm::vec3*)it2->second.first), GL_FLOAT_VEC3)));
			}
		}

		shader = mat.shader;
	}

	void SetShader(Shader& s) {
		shader = s;

		std::map<std::string, std::pair<void*, GLenum>>::iterator it;

		for (it = _parameters.begin(); it != _parameters.end(); it++) {
			delete it->second.first;
		}
		_parameters.clear();

		std::map<std::string, GLenum> uniforms = s.GetUniforms();
		std::map<std::string, GLenum>::iterator it2;

		for (it2 = uniforms.begin(); it2 != uniforms.end(); it2++) {
			std::cout << it2->first << std::endl;
			if (it2->second == GL_FLOAT) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new float(0), GL_FLOAT)));
			}
			else if (it2->second == GL_FLOAT_MAT4) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new glm::mat4(0), GL_FLOAT_MAT4)));
			}
			else if (it2->second == GL_FLOAT_VEC3) {
				_parameters.insert(std::make_pair(it2->first, std::make_pair<void*, GLenum>(new glm::vec3(0), GL_FLOAT_VEC3)));
			}
		}
	}

	void SetFloat(std::string parameter, float value) {
		_parameters[parameter] = std::make_pair<void*, GLenum>(new float(value), GL_FLOAT);
	}

	void SetMat4(std::string parameter, glm::mat4 value) {
		_parameters[parameter] = std::make_pair<void*, GLenum>(new glm::mat4(value), GL_FLOAT_MAT4);
	}

	void SetVec3(std::string parameter, glm::vec3 value) {
		_parameters[parameter] = std::make_pair<void*, GLenum>(new glm::vec3(value), GL_FLOAT_VEC3);
	}

	Shader& GetParameterizedShader() {
		std::map<std::string, std::pair<void*, GLenum>>::iterator it;
		
		for (it = _parameters.begin(); it != _parameters.end(); it++) {
			if (it->second.second == GL_FLOAT_VEC3) {
				shader.SetVec3Parameter(it->first, *(glm::vec3*)it->second.first);
			}
			else if (it->second.second == GL_FLOAT) {
				shader.SetFloatParameter(it->first, *(float*)it->second.first);
				
			}
			else if (it->second.second == GL_FLOAT_MAT4) {
				shader.SetMat4Parameter(it->first, (glm::f32*)it->second.first);
			}
		}
		
		return shader;
	}

private:
	std::map<std::string, std::pair<void*, GLenum>> _parameters;
	Shader shader;
};

#endif