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
uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 texelSize = 1.0 / textureSize(tex0, 0); // gets size of single texel
    vec3 result = texture(tex0, TexCoord).rgb * weight[0]; // current fragment's contribution

    if (horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(tex0, TexCoord + vec2(texelSize.x * i, 0.0)).rgb * weight[i];
            result += texture(tex0, TexCoord - vec2(texelSize.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(tex0, TexCoord + vec2(0.0, texelSize.y * i)).rgb * weight[i];
            result += texture(tex0, TexCoord - vec2(0.0, texelSize.y * i)).rgb * weight[i];
        }
    }

    /*
    vec4 centerTexel = texture(tex0, TexCoord);
    vec2 texelSize2 = texelSize * 2;
    vec4 centerRightTexel = texture(tex0, TexCoord + vec2(texelSize2, 0));
    vec4 centerLeftTexel = texture(tex0, TexCoord + vec2(-texelSize2, 0));
    vec4 centerTopTexel = texture(tex0, TexCoord + vec2(0, texelSize2));
    vec4 centerBottomTexel = texture(tex0, TexCoord + vec2(0, -texelSize2));

    vec4 A = texture(tex0, TexCoord + vec2(-texelSize)) + texture(tex0, TexCoord + vec2(texelSize)) + texture(tex0, TexCoord + vec2(-texelSize, texelSize)), texture(tex0, TexCoord + vec2(texelSize, -texelSize)) * 0.125;
    vec4 B = centerTexel + centerRightTexel + centerTopTexel + texture(tex0, TexCoord + vec2(texelSize2)) * 0.03125;
    vec4 C = centerTexel + centerRightTexel + centerBottomTexel + texture(tex0, TexCoord + vec2(texelSize2, -texelSize2)) * 0.03125;
    vec4 D = centerTexel + centerLeftTexel + centerBottomTexel + texture(tex0, TexCoord + vec2(-texelSize2)) * 0.03125;
    vec4 E = centerTexel + centerLeftTexel + centerTopTexel + texture(tex0, TexCoord + vec2(-texelSize2, texelSize2)) * 0.03125;
    */
    FragColor = vec4(result, 1.0);
}