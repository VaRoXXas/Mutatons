#include "pch.h"
#include "Rendering/Shader.h"
#include "Camera.h"

extern Camera mainCamera;
extern glm::mat4* transformMatrixPtr;
extern glm::mat4* modelMatrixPtr;
extern glm::mat4* viewMatrixPtr;
extern glm::mat4* projectionMatrixPtr;

// Lit shaders' variables.
bool directionalLightEnabled = true;
bool pointLightEnabled = false;
bool spotLight1Enabled = false;
bool spotLight2Enabled = false;
bool lightsPositionsDirectionsShown = false;
float lightsDirectionVectorAngleOffset = 0.0f;
float* directionalLightColorPtr = new float[4];
float* spotLight1ColorPtr = new float[4];
float* spotLight2ColorPtr = new float[4];
float* pointLightColorPtr = new float[4];
glm::vec3 directionalLightsDirection;
glm::vec3 pointLightPos = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 spotLight1Pos = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 spotLight2Pos = glm::vec3(-1.0f, 0.5f, 0.5f);
static const float s_pointLightLinearVal = 0.35f; // The smaller the value, the greater the effect on further objects.
static const float s_pointLightQuadraticVal = 0.44f; // The smaller the value, the greater the effect on closer objects.
static const float s_spotLightCutOffAngle = 12.5f; // In degrees.
static const float s_darkness[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static glm::vec3 lightColor;
static glm::vec3 diffuseColor;
static glm::vec3 ambientColor;



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

void Lighting::InitLighting(Shader& shader)
{
    shader.Use();
    shader.SetInt("material.diffuse", 0);
    shader.SetInt("material.specular", 1);
    shader.SetFloat("material.shininess", 16.0f);
    shader.SetFloat("pointLight.constant", 1.0f);
    shader.SetFloat("pointLight.linear", s_pointLightLinearVal);
    shader.SetFloat("pointLight.constant", s_pointLightQuadraticVal);
    shader.SetFloat("spotLight1.cutOff", glm::cos(glm::radians(s_spotLightCutOffAngle)));
    shader.SetFloat("spotLight2.cutOff", glm::cos(glm::radians(s_spotLightCutOffAngle)));
}

void Lighting::UpdateLighting(Shader& shader)
{
    shader.Use();
    shader.SetVecf3("viewPos", mainCamera.GetPosition());
	
    if (directionalLightEnabled)
    {
        lightColor = glm::vec3(directionalLightColorPtr[0], directionalLightColorPtr[1], directionalLightColorPtr[2]);
        diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
        ambientColor = diffuseColor * glm::vec3(0.4f); // low influence
        shader.SetVecf3("dirLight.ambient", ambientColor);
        shader.SetVecf3("dirLight.diffuse", diffuseColor);
        shader.SetVecf3("dirLight.specular", glm::vec3(0.2f));

        directionalLightsDirection = glm::vec3(0.2f, -1.0f, 0.3f + lightsDirectionVectorAngleOffset);
        shader.SetVecf3("dirLight.direction", directionalLightsDirection);
    }
    else
    {
        shader.SetVecf3("dirLight.ambient", s_darkness);
        shader.SetVecf3("dirLight.diffuse", s_darkness);
        shader.SetVecf3("dirLight.specular", s_darkness);
    }
    if (pointLightEnabled)
    {
        lightColor = glm::vec3(pointLightColorPtr[0], pointLightColorPtr[1], pointLightColorPtr[2]);
        diffuseColor = lightColor * glm::vec3(0.6f); // decrease the influence
        ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        shader.SetVecf3("pointLight.ambient", ambientColor);
        shader.SetVecf3("pointLight.diffuse", diffuseColor);
        shader.SetVecf3("pointLight.specular", glm::vec3(0.6f));

        shader.SetVecf3("pointLight.position", pointLightPos);
    }
    else
    {
        shader.SetVecf3("pointLight.ambient", s_darkness);
        shader.SetVecf3("pointLight.diffuse", s_darkness);
        shader.SetVecf3("pointLight.specular", s_darkness);
    }
    if (spotLight1Enabled)
    {
        lightColor = glm::vec3(spotLight1ColorPtr[0], spotLight1ColorPtr[1], spotLight1ColorPtr[2]);
        diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
        ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
        shader.SetVecf3("spotLight1.ambient", ambientColor);
        shader.SetVecf3("spotLight1.diffuse", diffuseColor);
        shader.SetVecf3("spotLight1.specular", glm::vec3(1.0f));

        shader.SetVecf3("spotLight1.position", spotLight1Pos);
        shader.SetVecf3("spotLight1.direction", glm::vec3(-0.2f, -1.0f, -0.3f + lightsDirectionVectorAngleOffset));
    }
    else
    {
        shader.SetVecf3("spotLight1.ambient", s_darkness);
        shader.SetVecf3("spotLight1.diffuse", s_darkness);
        shader.SetVecf3("spotLight1.specular", s_darkness);
    }
    if (spotLight2Enabled)
    {
        lightColor = glm::vec3(spotLight2ColorPtr[0], spotLight2ColorPtr[1], spotLight2ColorPtr[2]);
        diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
        ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
        shader.SetVecf3("spotLight2.ambient", ambientColor);
        shader.SetVecf3("spotLight2.diffuse", diffuseColor);
        shader.SetVecf3("spotLight2.specular", glm::vec3(1.0f));

        shader.SetVecf3("spotLight2.position", spotLight2Pos);
        shader.SetVecf3("spotLight2.direction", glm::vec3(0.3f, -0.7f, 1.3f + lightsDirectionVectorAngleOffset));
    }
    else
    {
        shader.SetVecf3("spotLight2.ambient", s_darkness);
        shader.SetVecf3("spotLight2.diffuse", s_darkness);
        shader.SetVecf3("spotLight2.specular", s_darkness);
    }
}
