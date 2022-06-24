#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;

uniform mat4 lightSpaceMat;
uniform mat4 model;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(gl_FragCoord.z);
	//gl_FragDepth = gl_FragCoord.z;
}