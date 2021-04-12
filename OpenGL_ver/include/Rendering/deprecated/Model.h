#pragma once

#include <Rendering/deprecated/Mesh.h>

static unsigned int TextureFromFile(const char* pathPtr, const std::string& directory, bool gamma = false);



class Model
{
public:
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const& path, bool gamma = false);
    void Draw(Shader& shader);
    void CustomRender(Shader& shader, glm::mat4 transform);

private:

    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* nodePtr, const aiScene* scenePtr);
    Mesh ProcessMesh(aiMesh* meshPtr, const aiScene* scenePtr);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* matPtr, aiTextureType type, std::string typeName);
};
