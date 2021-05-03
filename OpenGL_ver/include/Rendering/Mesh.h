#pragma once

#include "Rendering/Shader.h"

#define MAX_BONE_INFLUENCE 4

// This class is designed to store mesh informations


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    //bone indexes which will influence this vertex
    int mBoneIDs[MAX_BONE_INFLUENCE];

    //weights from each bone
    float mWeights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};



class Mesh {
public:

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);

private:

    unsigned int VBO, EBO;

    void SetupMesh();

};
