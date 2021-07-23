#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;
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
	normal = mat3(transpose(inverse(model))) * aNormal;//(model * vec4(aNormal, 1)).xyz;
	texCoord = aCoord;
	gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;

uniform float time;
uniform float deltaTime;

in vec3 vertPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform float lightIntensity;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform vec3 ambientColor;

uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D tex0;

void main()
{
	vec3 realLightPos = vec3(2, 2, 0);
	vec2 uv = (texCoord - vec2(0.5)) * 2;
	vec3 lightDir = normalize(realLightPos);//normalize(realLightPos - vertPos);
	vec3 norm = normalize(normal);
	float lightDist = length(lightDir);
	float dif = lightIntensity * 1 / pow(lightDist, 2) * max(dot(norm, lightDir), 0);
	vec3 diffuse = dif * lightColor;

	vec3 ambient = ambientColor * ambientIntensity;

	vec3 viewDir = normalize(viewPos - vertPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = 1 * spec * lightColor;

	vec3 finalCol = (diffuse + ambient + specular) * objectColor;
	FragColor = vec4(finalCol, 1);
}