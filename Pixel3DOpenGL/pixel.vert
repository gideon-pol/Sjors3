#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aCoord;
layout (location = 2) in float aTex;
uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 color;
out vec2 texCoord;
out float tex;

void main()
{
	texCoord = aCoord;
	tex = aTex;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);
}