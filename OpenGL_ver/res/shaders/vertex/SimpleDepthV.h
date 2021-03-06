#pragma once

static const char* s_simpleDepthVertexPtr = R"(
	#version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 lightSpaceMatrix;
    uniform mat4 model;
	uniform mat4 transform;

    void main()
    {
        gl_Position = lightSpaceMatrix * model * transform * vec4(aPos, 1.0);
    }  
)";
