#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;
uniform float time;

in vec2 texCoord;
uniform sampler2D tex0;

uniform vec2 resolution = vec2(1000);

void main()
{
	FragColor = texture(tex0, floor(gl_FragCoord.xy/2)/(resolution/2));
}