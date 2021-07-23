#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;

uniform float time;
uniform float deltaTime;

in vec3 vertPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPos;
uniform float lightIntensity;
uniform vec3 lightColor;
uniform vec3 shadowColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D tex0;

void main()
{
	vec3 col = vec3(1);
	vec3 realLightPos = vec3(1, 1.5, 0);
	vec2 uv = (texCoord - vec2(0.5))*2;
	vec3 lightDir = normalize(realLightPos - vertPos);
	vec3 norm = normalize(normal);
	float lightDist = length(lightDir);
	float lightStrength = lightIntensity * 1/pow(lightDist, 2) * max(dot(norm, lightDir), 0);
	if(lightStrength > 0.5){
		FragColor = vec4(lightColor, 1);
	} else {
		FragColor = vec4(shadowColor, 1);
	}
	FragColor = vec4(lightColor * lightStrength, 1) + vec4(lightColor/5, 1);
}