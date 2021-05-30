#pragma once

#include "Shader.h"


// PostProcessor hosts all PostProcessing effects.
// It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or 
// Shake boolean. 
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
    // State
    Shader postProcessingShader;
    GLuint texture;
    GLuint width, height;
    float shakeTime = 0.0f;
	
    float GetShakeTime() const;
    void SetShakeTime(const float shakeTime);
    // Options
    GLboolean confuse, chaos, shake;
    // Constructor
    PostProcessor(Shader shader, GLuint width, GLuint height);
    // Prepares the postprocessor's framebuffer operations before rendering the game.
    void BeginRender() const;
    // Should be called after rendering the game, so it stores all the rendered data into a texture object.
    void EndRender() const;
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void RenderQuad(GLfloat time) const;
private:
    // Render state
    GLuint MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    GLuint RBO; // RBO is used for multisampled color buffer
    GLuint VAO;
    // Initialize quad for rendering postprocessing texture
    void InitRenderData();
};
