#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoord;
uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 MVP;

out vec3 vertPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
	vertPos = (model * vec4(aPos, 1)).xyz;
	normal = (model * vec4(aNormal, 1)).xyz;;
	texCoord = aCoord;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1);
}