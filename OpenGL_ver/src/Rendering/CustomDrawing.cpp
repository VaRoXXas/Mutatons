#include "pch.h"
#include "Rendering/CustomDrawing.h"
#include "Rendering/Shader.h"
#include "Camera.h"

extern Camera mainCamera;
extern glm::mat4* viewMatrixPtr;
extern glm::mat4* projectionMatrixPtr;

Shader* litTexturedShaderPtr;
Shader* litTexturedInstancedShaderPtr;
Shader* orbitShaderPtr;
Shader* sphereShaderPtr;
Shader* lineShaderPtr;
Shader* refractShaderPtr;
Shader* skyboxShaderPtr;
GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO;
GLuint houseBase_diffuse, roof_diffuse, plane_diffuse, houseBase_specular, roof_specular, plane_specular, cubemapTexture;
std::vector<GLuint*> customVAOs, customVBOs;
glm::vec3 lineShaderEndPointPos;
int geometryShaderPseudoMeshDetailLevel = 3;



void CustomDrawing::DrawOrbit(const float& radius, const float* color, const glm::mat4& transform)
{
	orbitShaderPtr->Use();
	orbitShaderPtr->SetFloat("radius", radius);
	orbitShaderPtr->SetVecf4("color", color);
	orbitShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(orbitVAO);
	glDrawArrays(GL_POINTS, 0, 1); // Our orbit is created from a single point.
	glBindVertexArray(0);
}

void CustomDrawing::DrawSphere(const float& radius, const float* color, const glm::mat4& transform)
{
	sphereShaderPtr->Use();
	sphereShaderPtr->SetFloat("radius", radius);
	sphereShaderPtr->SetVecf4("color", color);
	sphereShaderPtr->SetMat4("transform", transform);
	sphereShaderPtr->SetInt("meshDetailLevel", geometryShaderPseudoMeshDetailLevel);
	glBindVertexArray(sphereVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 8); // 3 vertices of the triangle * 8 triangles
	glBindVertexArray(0);
}

void CustomDrawing::DrawPlane(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plane_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, plane_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); // 3 vertices of the triangle * 2 triangles
	glBindVertexArray(0);
}

void CustomDrawing::DrawCube(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, houseBase_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseBase_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void CustomDrawing::DrawPyramid(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roof_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, roof_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(pyramidVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * (2 + 4)); // 3 vertices of the triangle * (2 triangles of base + 4 triangles of sides)
	glBindVertexArray(0);
}

void CustomDrawing::DrawLine(const glm::mat4& transform)
{
	lineShaderPtr->Use();
	lineShaderPtr->SetMat4("transform", transform);
	lineShaderPtr->SetVecf3("endPointPos", lineShaderEndPointPos);
	glBindVertexArray(orbitVAO); // We use previously used VAO, because the line is being from a single point too.
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void CustomDrawing::DrawReflected(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	refractShaderPtr->Use();
	refractShaderPtr->SetVecf3("viewPos", mainCamera.GetPosition());
	refractShaderPtr->SetBool("refractMode", false);
	refractShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void CustomDrawing::DrawRefracted(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	refractShaderPtr->Use();
	refractShaderPtr->SetVecf3("viewPos", mainCamera.GetPosition());
	refractShaderPtr->SetBool("refractMode", true);
	refractShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void CustomDrawing::DrawSkybox()
{
	glDepthFunc(GL_LEQUAL);
	skyboxShaderPtr->Use();
	(*viewMatrixPtr) = glm::mat4(glm::mat3(mainCamera.GetViewMatrix())); // remove translation from the view matrix
	skyboxShaderPtr->SetMat4("view", (*viewMatrixPtr));
	skyboxShaderPtr->SetMat4("projection", (*projectionMatrixPtr));

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}
