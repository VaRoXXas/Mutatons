#pragma once

#include "pch.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Rendering/deprecated/Mesh.h>
#include <Rendering/Shader.h>

static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);



class Model
{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false);
    void Draw(Shader& shader);
    void CustomRender(Shader& shader, glm::mat4 transform);

private:

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
