#pragma once

namespace CustomDrawing
{
	// Used multiple times by the graph nodes.
	void DrawOrbit(const float& radius, const float* color, const glm::mat4& transform);
	void DrawSphere(const float& radius, const float* color, const glm::mat4& transform);
	void DrawPlane(const glm::mat4& transform);
	void DrawCube(const glm::mat4& transform);
	void DrawPyramid(const glm::mat4& transform);
	void DrawLine(const glm::mat4& transform);
	void DrawReflected(const glm::mat4& transform);
	void DrawRefracted(const glm::mat4& transform);
	// for debugging depth map (used in shadow mapping)
	void DrawQuad();

	// Used once in main.
	void DrawSkybox();
	void DrawHud1();
	void DrawHud2();
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void DrawMainmenu();
	void DrawLose();
	void DrawVictory();
	void DrawTutorial();
}
