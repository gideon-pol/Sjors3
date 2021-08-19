#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aCoord;
	gl_Position = vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	FragColor = texture(tex0, TexCoord) + texture(tex1, TexCoord);
}