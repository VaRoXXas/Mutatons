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
Shader* unlitTexturedAnimatedShaderPtr;
Shader* simpleDepthShaderPtr;
Shader* depthMapDebugShaderPtr;
Shader* hud1ShaderPtr;
GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, boxVAO, boxVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO, hud1VAO, hud1VBO;
GLuint houseBaseDiffuseTexture, roofDiffuseTexture, planeDiffuseTexture, houseBaseSpecularTexture, roofSpecularTexture, planeSpecularTexture, cubemapTexture;
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
	glBindTexture(GL_TEXTURE_2D, planeDiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planeSpecularTexture);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); // 3 vertices of the triangle * 2 triangles
	glBindVertexArray(0);
}

void CustomDrawing::DrawCube(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, houseBaseDiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseBaseSpecularTexture);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void CustomDrawing::DrawPyramid(const glm::mat4& transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roofDiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, roofSpecularTexture);
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
	refractShaderPtr->SetFloat("refractValue", 1.0f);
	glBindVertexArray(boxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void CustomDrawing::DrawQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

void CustomDrawing::DrawHud1()
{
	glUseProgram(hud1ShaderPtr->GetId());
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0,-0.5,0));
	hud1ShaderPtr->SetMat4("view", (*viewMatrixPtr));
	hud1ShaderPtr->SetMat4("transform", transform);
	hud1ShaderPtr->SetMat4("projection", (*projectionMatrixPtr));
	
	glBindVertexArray(hud1VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // no need to unbind it every time 

}
void CustomDrawing::DrawHud2()
{
	glUseProgram(hud1ShaderPtr->GetId());
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.5, 0.2, 0));
	transform = glm::rotate(transform, 1.57079633f,glm::vec3(0.0f, 0.0f, 1.0f));
	hud1ShaderPtr->SetMat4("transform", transform);
	hud1ShaderPtr->SetMat4("projection", (*projectionMatrixPtr));

	glBindVertexArray(hud1VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // no need to unbind it every time 

}
