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
uniform sampler2D tex1;
uniform float threshold;

uniform int mode;

uniform bool horizontal;
uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	switch(mode){
	case 0:
		vec3 texCol = texture(tex0, TexCoord).xyz;

		if (length(texCol) > threshold) {
			FragColor.xyz = texCol;
		}
        else {
            FragColor = vec4(0);
        }
        break;
	case 1:
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
        FragColor = vec4(result, 1.0);
        break;
    case 2:
        FragColor = texture(tex0, TexCoord) + texture(tex1, TexCoord);
        break;
	}
}