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

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 lightSpacePos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1));
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);//(model * vec4(aNormal, 1)).xyz;
	lightSpacePos = lightSpaceMat * vec4(FragPos, 1);
	TexCoord = aCoord;
	gl_Position = MVP * vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;

uniform float time;
uniform float deltaTime;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 lightSpacePos;

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

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

float calc_shadow(float lightDotNorm) {
	vec3 pos = lightSpacePos.xyz / lightSpacePos.w;
	pos = pos * 0.5 + 0.5;
	if (pos.z > 1.0) return 1;
	
	float bias = max(0.01 * (1.0 - lightDotNorm), 0.005);
	float shadow = 0;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float depth = texture(shadowMap, pos.xy + vec2(x, y) * texelSize).r;
			shadow += depth + bias > pos.z ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;
	
	return shadow;
}

void main()
{
	vec2 uv = (TexCoord - vec2(0.5)) * 2;
	vec3 lightDir = normalize(lightPos);// -FragPos);

	float lightDist = length(lightDir);
	float lightDotNorm = dot(Normal, lightDir);
	float dif = lightIntensity * /*(1/pow(lightDist, 2)) */ max(lightDotNorm, 0);
	vec3 diffuse = dif * lightColor;

	vec3 ambient = ambientColor * ambientIntensity;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = 1 * spec * lightColor;

	float shadow = calc_shadow(lightDotNorm);
	vec3 finalCol = (shadow * (diffuse + specular) + ambient) * texture(diffuseMap, TexCoord).xyz;

	vec3 texCol = texture(diffuseMap, TexCoord).xyz;

	float brightness = shadow * (spec + dif) + ambientIntensity;

	if (brightness <= 0.2) {
		FragColor = vec4(0.4, 0, 0.4, 1);
	}
	else if (brightness <= 0.95) {
		FragColor = vec4(0.6, 0, 0.6, 1);
	}
	else if(brightness <= 0.99) {
		FragColor = vec4(0.8, 0, 0.8, 1);
	}
	else {
		FragColor = vec4(1);
	}
}