#pragma once

static const char* s_mainmenuFragmentPtr = R"(
#version 330 core
    out vec4 FragColor;
	
	in vec3 ourColor;
	in vec2 TexCoord;

	// texture samplers
	uniform sampler2D mainmenuTexture;

    void main()
    {
       FragColor = texture(mainmenuTexture, TexCoord);  
    }
)";