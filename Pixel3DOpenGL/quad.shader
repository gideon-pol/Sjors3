#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;

out vec2 texCoord;

void main()
{
	texCoord = aCoord;
	gl_Position = vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	float depth = texture(tex0, texCoord).r;
	FragColor = vec4(vec3(depth*0.5+0.5), 1);
}