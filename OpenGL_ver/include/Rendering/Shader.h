#pragma once



class Shader
{
public:
	
    Shader(const char* vertexShaderNamePtr, const char* fragmentShaderNamePtr);
    Shader(const char* vertexShaderNamePtr, const char* geometryShaderNamePtr, const char* fragmentShaderNamePtr);
    ~Shader();

	// Makes this shader the one currently used by OpenGL (and your graphics card).
    void Use() const;
	// Applies global matrices' data to the shader's uniforms.
    void ApplyMvptMatrices() const;

	// shader uniforms' setters
    void SetBool(const char* namePtr, const bool& value) const;
    void SetInt(const char* namePtr, const int& value) const;
    void SetFloat(const char* namePtr, const float& value) const;
    void SetMat4(const char* namePtr, const glm::mat4& value) const;
    void SetVecf3(const char* namePtr, const float* valuePtr) const;
    void SetVecf3(const char* namePtr, const glm::vec3& value) const;
    void SetVecf4(const char* namePtr, const float* valuePtr) const;
    void SetVecf4(const char* namePtr, const glm::vec4& value) const;

	// getters and setters
    unsigned GetId() const;
    void SetId(const unsigned newId);

private:

    unsigned int id;
};

namespace Lighting
{
    void InitLighting(Shader& shader);
    void UpdateLighting(Shader& shader);
}
