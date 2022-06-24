#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;

out vec3 TexCoord;

uniform mat4 VP;

void main()
{
	TexCoord = aPos;
	gl_Position = (VP * vec4(aPos, 1)).xyww;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform float exposure;
uniform samplerCube skybox;

void main()
{
	vec4 col = texture(skybox, TexCoord);
	FragColor = col * 0.8;
}