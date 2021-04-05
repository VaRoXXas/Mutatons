#pragma once

#ifdef _DEBUG
#include <glad/gladDEBUG.h>
#else
#include <glad/glad.h>
#endif

extern glm::mat4* transformPtr;
extern glm::mat4* modelPtr;
extern glm::mat4* viewPtr;
extern glm::mat4* projectionPtr;


class Shader
{
public:
    unsigned int ID;
	
    Shader(const char* vertexShaderName, const char* fragmentShaderName);
    Shader(const char* vertexShaderName, const char* geometryShaderName, const char* fragmentShaderName);
    ~Shader();
    void Use() const;
    void SetBool(const char* name, bool value) const;
    void SetInt(const char* name, int value) const;
    void SetFloat(const char* name, float value) const;
    void SetMat4(const char* name, glm::mat4 value) const;
    void SetVecf3(const char* name, const float* value) const;
    void SetVecf3(const char* name, glm::vec3 value) const;
    void SetVecf4(const char* name, const float* value) const;
    void SetVecf4(const char* name, glm::vec4 value) const;
    void ApplyMVPT() const;
};
