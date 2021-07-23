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
		std::cout << contents << std::endl;
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragCode = get_file_contents(fragFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragSource = fragCode.c_str();

	CreateShader(vertexCode.c_str(), fragCode.c_str());
}

Shader::Shader(const char* shaderFile) {
	std::ifstream in(shaderFile, std::ios::binary);
	if (!in) {
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
		}
	}

	CreateShader(streams[0].str().c_str(), streams[1].str().c_str());
}

void Shader::CreateShader(const char* vertexSource, const char* fragSource) {
	//Initialize and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CheckCompile(ShaderType::VERTEX);

	//Initialize and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	CheckCompile(ShaderType::FRAGMENT);

	//Make shaders executable for the GPU
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);
	CheckCompile(ShaderType::PROGRAM);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::SetFloatParameter(std::string name, float value) {
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1f(uniformLocation, value);
}

void Shader::SetIntParameter(std::string name, int value) {
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());
	glUniform1i(uniformLocation, value);
}

void Shader::SetMat4Parameter(std::string name, glm::f32* mat) {
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat);
}

void Shader::SetVec2Parameter(std::string name, glm::vec2 vec) {
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());
	glUniform2fv(uniformLocation, 1, (GLfloat*)&vec);
}

void Shader::SetVec3Parameter(std::string name, glm::vec3 vec) {
	GLint uniformLocation = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(uniformLocation, 1, (GLfloat*)&vec);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::CheckCompile(ShaderType shaderType) {
	return;
	GLint successfullyCompiled;
	char errMsg[1024];

	if(shaderType == ShaderType::PROGRAM) {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &successfullyCompiled);

		if (successfullyCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(ID, 1024, NULL, errMsg);
			std::cout << "SHADER_COMPILATION_ERROR for: PROGRAM" << "\n" << errMsg << std::endl;
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
			std::cout << "SHADER_COMPILATION_ERROR for: " << (shaderType == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT") << "\n" << errMsg << std::endl;
		}
	}
}