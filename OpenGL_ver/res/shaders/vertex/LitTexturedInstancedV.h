#pragma once

static const char* s_litTexturedInstancedVertexPtr = R"(
    #version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;
	layout (location = 3) in vec3 offset;

	
    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoords;
    out vec4 FragPosLightSpace;

	uniform mat4 transform;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 lightSpaceMatrix;

	void main()
	{
		FragPos = vec3(model * transform * vec4(aPos + offset, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal; // It removes the problems with invalid lighting when we scale an object unevenly.
	    TexCoords = aTexCoords;
		FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	    gl_Position = projection * view * model * transform * vec4(aPos + offset, 1.0);
	}
)";
