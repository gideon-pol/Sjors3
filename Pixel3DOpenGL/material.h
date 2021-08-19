#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <typeindex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Core.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"

class Material {
public:
	Material() { std::cout << "Material constructor called" << std::endl; };

	Material(const Ref<Material> mat) {
		_parameters.clear();

		std::map<std::string, std::pair<Ref<void>, GLenum>>::const_iterator it2;
		for (it2 = mat->_parameters.begin(); it2 != mat->_parameters.end(); it2++) {
			std::string uniformName = it2->first;
			void* value = it2->second.first.get();
			GLenum type = it2->second.second;

			switch (type) {
				case GL_INT:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new int(*(int*)value)), GL_INT)));
					break;
				case GL_FLOAT:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new float(*(float*)value)), GL_FLOAT)));
					break;
				case GL_FLOAT_MAT4:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new glm::mat4(*(glm::mat4*)value)), GL_FLOAT_MAT4)));
					break;
				case GL_FLOAT_VEC3:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new glm::vec3(*(glm::vec3*)value)), GL_FLOAT_VEC3)));
					break;
				case GL_SAMPLER_2D:
					Ref<void> ref = it2->second.first;
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(ref), GL_SAMPLER_2D)));
					break;
			}
		}

		_shader = mat->_shader;
	}

	void SetShader(Ref<Shader> s) {
		_shader = s;
		_parameters.clear();

		std::map<std::string, GLenum> uniforms = _shader->GetUniforms();
		std::map<std::string, GLenum>::iterator it2;

		for (it2 = uniforms.begin(); it2 != uniforms.end(); it2++) {
			std::string uniformName = it2->first;
			GLenum type = it2->second;
			std::cout << "Actual parameter: " << uniformName << std::endl;
			switch (type) {
				case GL_INT:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new int(0)), GL_INT)));
					break;
				case GL_FLOAT:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new float(0)), GL_FLOAT)));
					break;
				case GL_FLOAT_MAT4:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new glm::mat4(0)), GL_FLOAT_MAT4)));
					break;
				case GL_FLOAT_VEC3:
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(Ref<void>(new glm::vec3(0)), GL_FLOAT_VEC3)));
					break;
				case GL_SAMPLER_2D:
					std::cout << "Got a sampler" << std::endl;
					_parameters.insert(std::make_pair(uniformName, std::make_pair<Ref<void>, GLenum>(NULL, GL_SAMPLER_2D)));
					break;
			}
		}
	}

	void SetFloat(std::string parameter, float value) {
		if (_parameters.find(parameter) != _parameters.end()) {
			*(float*)_parameters[parameter].first.get() = value;
		}
	}

	void SetMat4(std::string parameter, glm::mat4 value) {
		if (_parameters.find(parameter) != _parameters.end()) {
			*(glm::mat4*)_parameters[parameter].first.get() = value;
		}
	}

	void SetVec3(std::string parameter, glm::vec3 value) {
		if (_parameters.find(parameter) != _parameters.end()) {
			*(glm::vec3*)_parameters[parameter].first.get() = value;
		}
	}

	void SetTexture(std::string parameter, Ref<Texture> value) {
		if (_parameters.find(parameter) != _parameters.end()) {
			_parameters[parameter].first = value;
		}
		else {
			std::cout << "Couldnt find texture" << std::endl;
		}
	}

	Ref<Shader> GetParameterizedShader() {
		std::map<std::string, std::pair<Ref<void>, GLenum>>::iterator it;
		GLuint texIndex = 0;
		
		for (it = _parameters.begin(); it != _parameters.end(); it++) {
			std::string uniformName = it->first;
			void* value = it->second.first.get();

			switch (it->second.second) {
				case GL_INT:
					_shader->SetIntParameter(uniformName, *(int*)value);
					break;
				case GL_FLOAT:
					_shader->SetFloatParameter(uniformName, *(float*)value);
					break;
				case GL_FLOAT_MAT4:
					_shader->SetMat4Parameter(uniformName, (glm::f32*)value);
					break;
				case GL_FLOAT_VEC3:
					_shader->SetVec3Parameter(uniformName, *(glm::vec3*)value);
					break;
				case GL_SAMPLER_2D:
					if (value != NULL) {
						glActiveTexture(GL_TEXTURE0 + texIndex);
						((Texture*)value)->Bind();
						_shader->SetIntParameter(uniformName, texIndex);
						if (_parameters[uniformName + "Assigned"].first.get() != nullptr)
							*(int*)_parameters[uniformName + "Assigned"].first.get() = 1;
						texIndex++;
					}
					else {
						if (_parameters[uniformName + "Assigned"].first.get() != nullptr)
							*(int*)_parameters[uniformName + "Assigned"].first.get() = 0;
						glBindTexture(GL_TEXTURE_2D, 0);
					}
					break;
			}
		}

		return _shader;
	}

private:
	std::map<std::string, std::pair<Ref<void>, GLenum>> _parameters;
	Ref<Shader> _shader;
};

#endif