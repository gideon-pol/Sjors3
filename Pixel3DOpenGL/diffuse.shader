#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;
uniform float s_Time;

struct DLight {
	vec3 Direction;
	vec3 Color;
	float Intensity;
	mat4 lightSpaceMat;
	sampler2D shadowMap;
};

uniform DLight s_Lights[5];

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 MVP;
uniform mat4 s_LightSpaceMat;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 lightSpacePos[5];

void main()
{
	FragPos = vec3(model * vec4(aPos, 1));
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);//(model * vec4(aNormal, 1)).xyz;
	//lightSpacePos[0] = s_LightSpaceMat * vec4(FragPos, 1);
	for (int i = 0; i < 5; i++) {
		lightSpacePos[i] = s_Lights[i].lightSpaceMat * vec4(FragPos, 1);
	}
	TexCoord = aCoord;
	gl_Position = MVP * vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct DLight {
	vec3 Direction;
	vec3 Color;
	float Intensity;
	mat4 lightSpaceMat;
	sampler2D shadowMap;
};

uniform DLight s_Lights[5];
uniform int s_LightCount;

in vec4 gl_FragCoord;

uniform float s_Time;
uniform float s_DeltaTime;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 lightSpacePos[5];

uniform vec3 objectColor;
uniform float specularIntensity;

uniform vec3 s_ViewPos;
uniform float s_AmbientIntensity;
uniform vec3 s_AmbientColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 s_LightSpaceMat;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

float calc_shadow(float lightDotNorm) {
	vec3 pos = lightSpacePos[0].xyz / lightSpacePos[0].w;
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
	vec3 viewDir = normalize(s_ViewPos - FragPos);

	for (int i = 0; i < s_LightCount; i++) {
		vec3 lightDir = normalize(s_Lights[i].Direction);// -FragPos);

		float lightDist = length(lightDir);
		float lightDotNorm = dot(Normal, lightDir);
		float dif = s_Lights[i].Intensity * /*(1/pow(lightDist, 2)) */ max(lightDotNorm, 0);
		vec3 diffuse = dif * s_Lights[i].Color;

		vec3 ambient = s_AmbientColor * s_AmbientIntensity;
		
		vec3 reflectDir = reflect(-lightDir, Normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularIntensity;
		vec3 specular = spec * s_Lights[i].Color;

		float shadow = 1;//calc_shadow(lightDotNorm);
		vec3 finalCol = (shadow * (diffuse + specular) + ambient) * s_Lights[i].Color * objectColor;

		vec3 texCol = texture(diffuseMap, TexCoord).xyz;

		float brightness = shadow * (spec + dif) + s_AmbientIntensity;

		if (brightness <= 0.2) {
			FragColor += vec4(s_Lights[i].Color * objectColor * 0.4, 1);
		}
		else if (brightness <= 0.95) {
			FragColor += vec4(s_Lights[i].Color * objectColor * 0.6, 1);
		}
		else {
			FragColor += vec4(s_Lights[i].Color * objectColor * 0.8, 1);
		}

		if (spec * shadow >= 0.1) {
			FragColor.xyz += s_Lights[i].Color;
		}
	}
}