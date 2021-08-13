#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;
uniform float s_Time;

struct Light {
	int Type;
	vec3 Position;
	vec3 Color;
	float Intensity;
};

uniform Light s_Lights[5];

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
		//lightSpacePos[i] = s_Lights[i].lightSpaceMat * vec4(FragPos, 1);
	}
	TexCoord = aCoord;
	gl_Position = MVP * vec4(aPos, 1);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct Light {
	int Type;
	vec3 Position;
	vec3 Color;
	float Intensity;
};

uniform Light s_Lights[5];
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
	FragColor = vec4(s_AmbientColor * s_AmbientIntensity * objectColor, 1);
	vec4 texColor;

	if (diffuseMapAssigned == 0) {
		texColor = vec4(1);
	}
	else {
		texColor = texture(diffuseMap, TexCoord);
	}
	
	for (int i = 0; i < s_LightCount; i++) {
		vec3 lightDir;
		if (s_Lights[i].Type == 0) {
			lightDir = normalize(s_Lights[i].Position);// -FragPos);
		}
		else {
			lightDir = normalize(s_Lights[i].Position - FragPos);
		}
		
		float lightDotNorm = dot(Normal, lightDir);
		float dif = max(lightDotNorm, 0);
		float lightDist = length(s_Lights[i].Position - FragPos);
		
		vec3 reflectDir = reflect(-lightDir, Normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularIntensity;

		float shadow = 1;//calc_shadow(lightDotNorm);

		float brightness = shadow * (spec + dif) * s_Lights[i].Intensity;

		switch (s_Lights[i].Type) {
			case 0:
				if (s_Lights[i].Intensity <= 0.00005) {
					//continue;
				}
				break;
			case 1:
				brightness *= (1 / pow(lightDist, 2));
				if (brightness <= 0.005) {
					continue;
				}
		}

		if (brightness <= 0.05) {
			FragColor.xyz += s_Lights[i].Color * objectColor * texColor.xyz;
		}
		else if (brightness <= 0.65) {
			FragColor.xyz += s_Lights[i].Color * objectColor * 4 * texColor.xyz;
		}
		else {
			FragColor.xyz += s_Lights[i].Color * objectColor * 8 * texColor.xyz;
		}

		if (spec * shadow * s_Lights[i].Intensity >= 0.01) {
			FragColor.xyz = s_Lights[i].Color * 10 * pow(specularIntensity, 2) + max(1 - specularIntensity, 0) * FragColor.xyz * 3;
		}
	}
}