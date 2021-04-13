#include "pch.h"
#include "Rendering/Shader.h"

extern glm::mat4* transformMatrixPtr;
extern glm::mat4* modelMatrixPtr;
extern glm::mat4* viewMatrixPtr;
extern glm::mat4* projectionMatrixPtr;



Shader::Shader(const char* vertexShaderNamePtr, const char* fragmentShaderNamePtr)
{
    const auto* vertexCodePtr = vertexShaderNamePtr;
    const auto* fragmentCodePtr = fragmentShaderNamePtr;

    int success;
    char infoLog[512];

    // vertex shader
    const auto vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCodePtr, nullptr);
    glCompileShader(vertex);  
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader  
    const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodePtr, nullptr);
    glCompileShader(fragment); 
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // program object  
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
	
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char* vertexShaderNamePtr, const char* geometryShaderNamePtr, const char* fragmentShaderNamePtr)
{
    const auto* vertexCodePtr = vertexShaderNamePtr;
    const auto* geometryCodePtr = geometryShaderNamePtr;
    const auto* fragmentCodePtr = fragmentShaderNamePtr;

    int success;
    char infoLog[512];

    // vertex shader  
    const auto vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCodePtr, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // geometry shader  
    const auto geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometryCodePtr, nullptr);
    glCompileShader(geometry);
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader  
    const auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodePtr, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Program Object  
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, geometry);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Use() const
{
    glUseProgram(id);
}

void Shader::ApplyMvptMatrices() const
{
    Use();
    SetMat4("projection", (*projectionMatrixPtr));
    SetMat4("view", (*viewMatrixPtr));
    SetMat4("model", (*modelMatrixPtr));
    SetMat4("transform", (*transformMatrixPtr));
}

void Shader::SetBool(const char* namePtr, const bool& value) const
{
    glUniform1i(glGetUniformLocation(id, namePtr), static_cast<int>(value));
}
void Shader::SetInt(const char* namePtr, const int& value) const
{
    glUniform1i(glGetUniformLocation(id, namePtr), value);
}
void Shader::SetFloat(const char* namePtr, const float& value) const
{
    glUniform1f(glGetUniformLocation(id, namePtr), value);
}

void Shader::SetMat4(const char* namePtr, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, namePtr), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVecf3(const char* namePtr, const float* valuePtr) const
{
    glUniform3fv(glGetUniformLocation(id, namePtr), 1, valuePtr);
}

void Shader::SetVecf3(const char* namePtr, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(id, namePtr), value.x, value.y, value.z);
}

void Shader::SetVecf4(const char* namePtr, const float* valuePtr) const
{
    glUniform4fv(glGetUniformLocation(id, namePtr), 1, valuePtr);
}

void Shader::SetVecf4(const char* namePtr, const glm::vec4& value) const
{
    glUniform4f(glGetUniformLocation(id, namePtr), value.x, value.y, value.z, value.w);
}

unsigned Shader::GetId() const
{
	return id;
}

void Shader::SetId(const unsigned newId)
{
	id = newId;
}
