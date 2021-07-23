#version 330 core
out vec4 FragColor;
uniform float time;

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = vec4(1);// texture(tex0, texCoord);
}