#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
uniform float s_Time;

struct Light {
	int Type;
	vec3 Position;
	vec3 Color;
	float Intensity;
	int CastShadows;
	mat4 LightSpaceMat[6];
};

uniform Light s_Lights[10];
uniform int s_LightCount;

out LightData{
	vec3 TangentPosition;
	vec4 FragPosLightSpace;
} LightDatas[10];

uniform vec3 s_ViewPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 MVP;
uniform mat4 s_LightSpaceMat;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	mat3 TBN;
	vec3 TangentNormal;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1));
	vs_out.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);//(model * vec4(aNormal, 1)).xyz;
	vs_out.TexCoord = aCoord;

	gl_Position = MVP * vec4(aPos, 1);

	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);

	
	for (int i = 0; i < 10; i++) {
		if (s_Lights[i].CastShadows == 1) {
			if (s_Lights[i].Type == 0) {
				vs_out.FragPosLightSpace = s_LightSpaceMat * vec4(vs_out.FragPos, 1.0);
			}
		}
	}
	
	/*
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TBN = TBN;
	vs_out.TangentNormal = TBN * vs_out.Normal;
	
	vs_out.TangentViewPos = TBN * s_ViewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;

	for (int i = 0; i < s_LightCount; i++) {
		//s_Lights[i].Position = TBN * s_Lights[i].Position;
		LightDatas[i].TangentPosition = TBN * s_Lights[i].Position;
	}
	*/
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct Light {
	int Type;
	vec3 Position;
	vec3 Color;
	float Intensity;
	int CastShadows;
	mat4 LightSpaceMat[6];
};

uniform Light s_Lights[10];
uniform int s_LightCount;

in LightData{
	vec3 TangentPosition;
	vec4 FragPosLightSpace;
} LightDatas[10];

in vec4 gl_FragCoord;

uniform float s_Time;
uniform float s_DeltaTime;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	mat3 TBN;
	vec3 TangentNormal;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec4 FragPosLightSpace;
} fs_in;

uniform vec3 objectColor;
uniform float specularIntensity;
uniform vec3 emission;

uniform vec3 s_ViewPos;
uniform float s_AmbientIntensity;
uniform vec3 s_AmbientColor;

uniform mat4 s_LightSpaceMat;

uniform sampler2D s_ShadowMap;


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform float normalMapStrength;


float calc_shadow(int light, float lightDotNorm) {
	vec3 pos = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
	
	pos = pos * 0.5 + 0.5;

	if (pos.z > 1.0) return 1;
	
	
	float bias = max(0.01 * (1.0 - lightDotNorm), 0.001);
	float shadow = 0;

	vec2 texelSize = 1.0 / textureSize(s_ShadowMap, 0);
	
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float depth = texture(s_ShadowMap, pos.xy + vec2(x, y) * texelSize).r;
			shadow += depth + bias > pos.z ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	return shadow;
}

void main()
{
	vec2 uv = (fs_in.TexCoord - vec2(0.5)) * 2;
	vec3 viewDir = normalize(s_ViewPos - fs_in.FragPos);
	FragColor = vec4(s_AmbientColor * s_AmbientIntensity * objectColor, 1) + vec4(emission, 1);
	vec4 texColor;
	if (diffuseMapAssigned == 0) {
		texColor = vec4(1);
	}
	else {
		texColor = texture(diffuseMap, fs_in.TexCoord);
	}

	vec3 normal;
	if (normalMapAssigned == 0 || isnan(fs_in.TBN[0][0])) {
		normal = fs_in.Normal;
	}
	else {
		normal = texture(normalMap, fs_in.TexCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(fs_in.TBN * normal * normalMapStrength + (1 - normalMapStrength) * fs_in.Normal);
	}

	
	for (int i = 0; i < s_LightCount; i++) {
		Light light = s_Lights[i];

		vec3 lightDir;
		if (s_Lights[i].Type == 0) {
			lightDir = normalize(light.Position);// -fs_in.FragPos);
		}
		else {
			lightDir = normalize(light.Position - fs_in.FragPos);
		}

		float lightDotNorm = dot(normal, lightDir);
		float dif = max(lightDotNorm, 0);
		float lightDist = length(light.Position - fs_in.FragPos);
		
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularIntensity;

		float shadow = 1;//calc_shadow(lightDotNorm);
		if (light.CastShadows == 1) {
			shadow = calc_shadow(i, lightDotNorm);
		}

		float brightness = shadow *(spec + dif)* light.Intensity;

		switch (light.Type) {
			case 1:
				brightness *= (1 / pow(lightDist, 2));
				if (brightness <= 0.005) {
					continue;
				}
		}

		//FragColor = vec4(brightness);
		//return;
		
		
		if (brightness <= 0.05) {
			FragColor.xyz += light.Color * objectColor * .1 * texColor.xyz;
		}
		else if (brightness <= 0.65) {
			
			FragColor.xyz += light.Color * objectColor * .4 * texColor.xyz;
		}
		else {
			FragColor.xyz += light.Color * objectColor * .8 * texColor.xyz;
		}

		if (spec * shadow * light.Intensity >= 0.01) {
			FragColor.xyz += light.Color * pow(specularIntensity, 2) + max(1 - specularIntensity, 0) * FragColor.xyz * .3;
		}

	}
}