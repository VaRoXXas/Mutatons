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
Shader* textShaderPtr;
Shader* postProcessingShaderPtr;
Shader* mainmenuShaderPtr;
GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, boxVAO, boxVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO, hud1VAO, hud1VBO, textVAO, textVBO, mainmenuVAO, mainmenuVBO;
GLuint houseBaseDiffuseTexture, roofDiffuseTexture, planeDiffuseTexture, houseBaseSpecularTexture, roofSpecularTexture, planeSpecularTexture, cubemapTexture, mainmenuTexture, UITexture;
std::vector<GLuint*> customVAOs, customVBOs;
glm::vec3 lineShaderEndPointPos;
int geometryShaderPseudoMeshDetailLevel = 3;

struct TextCharacter {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};
std::map<GLchar, TextCharacter> TextCharacters;

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
	glUseProgram(mainmenuShaderPtr->GetId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, UITexture);
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
void CustomDrawing::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	textShaderPtr->Use();
	textShaderPtr->SetVecf3("textColor", glm::vec3(color.x, color.y, color.z));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		TextCharacter ch = TextCharacters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void CustomDrawing::DrawMainmenu()
{
	mainmenuShaderPtr->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainmenuTexture);
	glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, glm::vec3(0.3, 0.4, 0));


	glBindVertexArray(mainmenuVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0); // no need to unbind it every time 

}