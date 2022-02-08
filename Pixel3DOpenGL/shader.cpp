#include <fstream>
#include <string>
#include <sstream>
#include "shader.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		//std::cout << contents << std::endl;
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragCode = get_file_contents(fragFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragSource = fragCode.c_str();

	_createShader(vertexCode.c_str(), fragCode.c_str());
}

Shader::Shader(const char* shaderFile) {
	std::ifstream in(shaderFile, std::ios::binary);
		
	if (!in) {
		std::cout << "Couldn't open shader file: " << shaderFile << std::endl;
		return;
	}

	std::string line;
	std::stringstream streams[2];
	ShaderType type = ShaderType::PROGRAM;
	while (std::getline(in, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			streams[(int)type] << line << std::endl;

			if (line.substr(0, 18) == "uniform sampler2D ") {
				std::string varName;
				for (int i = 18; i < line.length(); i++) {
					if (line[i] == ';') break;
					varName += line[i];
				}

				streams[(int)type] << "uniform int " << varName << "Assigned = 0;" << std::endl;
			}
		}
	}

	_createShader(streams[0].str().c_str(), streams[1].str().c_str());
}

void Shader::_createShader(const char* vertexSource, const char* fragSource) {
	//Initialize and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	_checkCompile(ShaderType::VERTEX);

	//Initialize and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	_checkCompile(ShaderType::FRAGMENT);

	//Make shaders executable for the GPU
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	if (_checkCompile(ShaderType::PROGRAM)) {
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 32; // maximum parameter length
		GLchar parameter[bufSize]; // variable parameter in GLSL
		GLsizei length; // parameter length

		glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &count);
		//printf("\nActive Uniforms: %d\n", count);

		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(ID, (GLuint)i, bufSize, &length, &size, &type, parameter);
			std::string uniform = std::string(parameter, length);
			GLint uniformLocation = glGetUniformLocation(ID, uniform.c_str());
			_uniforms.emplace(uniform, std::make_pair(type, uniformLocation));
			//printf("Uniform #%d Type: %u Name: %s\n", i, type, parameter);
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::SetFloatParameter(std::string parameter, float value) {
	if (_uniforms.find(parameter) != _uniforms.end()) {
		glUniform1f(_uniforms[parameter].second, value);
	}
}

void Shader::SetIntParameter(std::string parameter, int value) {
	if (_uniforms.find(parameter) != _uniforms.end()) {
		glUniform1i(_uniforms[parameter].second, value);
	}
	else {
		std::cout << "No uniform of name " << parameter << std::endl;
	}
}

void Shader::SetMat4Parameter(std::string parameter, glm::f32* value) {
	if (_uniforms.find(parameter) != _uniforms.end()) {
		glUniformMatrix4fv(_uniforms[parameter].second, 1, GL_FALSE, value);
	}
}

void Shader::SetVec2Parameter(std::string parameter, glm::vec2 value) {
	if (_uniforms.find(parameter) != _uniforms.end()) {
		glUniform2fv(_uniforms[parameter].second, 1, (GLfloat*)&value);
	}
}

void Shader::SetVec3Parameter(std::string parameter, glm::vec3 value) {
	if (_uniforms.find(parameter) != _uniforms.end()) {
		glUniform3fv(_uniforms[parameter].second, 1, (GLfloat*)&value);
	}
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

bool Shader::_checkCompile(ShaderType shaderType) {
	GLint successfullyCompiled;
	char errMsg[1024];

	if(shaderType != ShaderType::PROGRAM) {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &successfullyCompiled);

		if (successfullyCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(ID, 1024, NULL, errMsg);
			std::cout << "SHADER_COMPILATION_ERROR for: " << (shaderType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "\n" << errMsg << std::endl;

			return false;
		}
		else 
		{
			std::cout << "Program successfully compiled" << std::endl;
		}
	}
	else {
		glGetProgramiv(ID, GL_LINK_STATUS, &successfullyCompiled);

		if (successfullyCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(ID, 1024, NULL, errMsg);
			std::cout << "SHADER_COMPILATION_ERROR for: PROGRAM" << "\n" << errMsg << std::endl;
			return false;
		}
	}

	return true;
}

std::map<std::string, GLenum> Shader::GetUniforms() {
	std::map<std::string, GLenum> m;

	std::map<std::string, std::pair<GLenum, GLint>>::iterator it;

	for (it = _uniforms.begin(); it != _uniforms.end(); it++) {
		
		if (!(it->first[0] == 's' && it->first[1] == '_')) {
			m.emplace(it->first, it->second.first);
		}
	}

	return m;
}