#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;

uniform float exposure = 1.0;

const float gamma = 2.2;

in vec2 texCoord;
uniform sampler2D tex0;

uniform vec2 resolution = vec2(1000);

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 saturate(vec3 rgb, float adjustment)
{
    // Algorithm from Chapter 16 of OpenGL Shading Language
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

const float Epsilon = 1e-10;

vec3 RGBtoHSV(in vec3 RGB)
{
    vec4  P   = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0, 2.0/3.0) : vec4(RGB.gb, 0.0, -1.0/3.0);
    vec4  Q   = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
    float C   = Q.x - min(Q.w, Q.y);
    float H   = abs((Q.w - Q.y) / (6.0 * C + Epsilon) + Q.z);
    vec3  HCV = vec3(H, C, Q.x);
    float S   = HCV.y / (HCV.z + Epsilon);
    return vec3(HCV.x, S, HCV.z);
}

vec3 HSVtoRGB(in vec3 HSV)
{
    float H   = HSV.x;
    float R   = abs(H * 6.0 - 3.0) - 1.0;
    float G   = 2.0 - abs(H * 6.0 - 2.0);
    float B   = 2.0 - abs(H * 6.0 - 4.0);
    vec3  RGB = clamp( vec3(R,G,B), 0.0, 1.0 );
    return ((RGB - 1.0) * HSV.y + 1.0) * HSV.z;
}

void main()
{
    vec3 hdrColor = texture(tex0, texCoord).xyz;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

	//vec3 mapped = filmicToneMapping(hdrColor);
	mapped = pow(mapped, vec3(1.0 / gamma));
	mapped -= vec3(0.3);
    mapped = clamp(mapped, vec3(0), vec3(10000));
	mapped *= 1.25;
    vec3 mappedHSV = RGBtoHSV(mapped);
    mappedHSV.y *= 1.5;
    mapped = HSVtoRGB(mappedHSV);
  
    FragColor = vec4(mapped, 1.0);
}