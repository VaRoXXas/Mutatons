#pragma once

static const char* RefractF = R"(
	#version 330 core
    out vec4 FragColor;

    in vec3 FragPos;
    in vec3 Normal;

    uniform vec3 viewPos;
    uniform samplerCube skybox;
	uniform bool refractMode;

    void main()
    {             
        vec3 I = normalize(FragPos - viewPos);
        vec3 R;
		if(refractMode)
		{
			float ratio = 1.00 / 1.52; // 1.52 jest wspolczynnikiem zalamania dla szkla
			R = refract(I, normalize(Normal), ratio);
		}
		else
		{
			R = reflect(I, normalize(Normal));
		}
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    }
)";
