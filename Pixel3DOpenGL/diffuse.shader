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
uniform mat4 lightSpaceMat;

out vec3 vertPos;
out vec3 normal;
out vec2 texCoord;
out vec3 lightSpacePos;

void main()
{
	vertPos = (model * vec4(aPos, 1)).xyz;
	normal = mat3(transpose(inverse(model))) * aNormal;//(model * vec4(aNormal, 1)).xyz;
	lightSpacePos = (lightSpaceMat * vec4(vertPos, 1)).xyz;
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
in vec3 lightSpacePos;

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
uniform mat4 lightSpaceMat;

uniform sampler2D shadowMap;

float calc_shadow(float lightDotNorm) {
	vec3 pos = lightSpacePos * 0.5 + 0.5;
	
	float depth = texture(shadowMap, pos.xy).r;
	float bias = max(0.05 * (1.0 - lightDotNorm), 0.005);
	float shadow = (depth + bias) < pos.z ? 0.0 : 1.0;
	/*vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, pos.xy + vec2(x, y) * texelSize).r;
			shadow += (depth + bias) < pcfDepth ? 0.0 : 1.0;
		}
	}
	shadow /= 9.0;*/
	//if (pos.z > 1.0) shadow = 0;
	return shadow;
}

void main()
{
	vec2 uv = (texCoord - vec2(0.5)) * 2;
	vec3 lightDir = normalize(lightPos);//normalize(lightPos - vertPos);
	vec3 norm = normalize(normal);
	float lightDist = length(lightDir);
	float lightDotNorm = dot(norm, lightDir);
	float dif = lightIntensity * 1 / pow(lightDist, 2) * max(lightDotNorm, 0);
	vec3 diffuse = dif * lightColor;

	vec3 ambient = ambientColor * ambientIntensity;

	vec3 viewDir = normalize(viewPos - vertPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = 1 * spec * lightColor;

	float shadow = calc_shadow(lightDotNorm);
	vec3 finalCol = (shadow * (diffuse + specular) + ambient) * objectColor;

	FragColor = vec4(finalCol, 1);
}