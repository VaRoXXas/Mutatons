#include "pch.h"
#include "GeometryCreation.h"
#include "Rendering/CustomDrawing.h"

extern GLuint hud1VAO, hud1VBO;

void GeometryCreation::Hud1Creation()
{
    float hud1ver[] = {
        // Pierwszy trójk¹t  
         0.5f, -0.2f, 0.5f, // Prawy górny  
         0.5f, -0.5f, 0.5f, // Prawy dolny  
         -0.5f,-0.2f, 0.5f, // Lewy górny  
        // Drugi trójk¹t  
         0.5f, -0.5f, 0.5f, // Prawy dolny  
        -0.5f, -0.5f, 0.5f, // Lewy dolny  
        -0.5f, -0.2f, 0.5f // Lewy górny  
    };

    //float hud1ver[] = {
    //         1.0f,  1.0f, 0.0f,  // top right
    //         1.0f, -1.0f, 0.0f,  // bottom rigVAOHUD1ht
    //        -1.0f, -1.0f, 0.0f,  // bottom left
    //        -1.0f,  1.0f, 0.0f   // top left 
    //};
    //unsigned int hud1ind[] = {  // note that we start from 0!
    //    0, 1, 3,  // first Triangle
    //    1, 2, 3   // second Triangle
    //};
    //unsigned int hud1VBO, hud1VAO, hud1EBO;
    glGenVertexArrays(1, &hud1VAO);
    glBindVertexArray(hud1VAO);
    glGenBuffers(1, &hud1VBO);
    //glGenBuffers(1, &hud1EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glBindBuffer(GL_ARRAY_BUFFER, hud1VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hud1ver), hud1ver, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hud1EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hud1ind), hud1ind, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);
}