#pragma once

static const char* LitTexturedInstancedV = R"(
    #version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;
	layout (location = 3) in vec3 offset;

	out vec2 TexCoords;
	out vec3 Normal;
	out vec3 FragPos;

	uniform mat4 transform;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
	    TexCoords = aTexCoords;    
	    gl_Position = projection * view * model * transform * vec4(aPos + offset, 1.0);
		Normal = mat3(transpose(inverse(model))) * aNormal; // Przeksztalcenie do macierzy normalnych usuwa problemy z nieprawidlowym oswietleniem przy nierownomiernym skalowaniu obiektu.
		FragPos = vec3(model * transform * vec4(aPos + offset, 1.0));
	}
)";
