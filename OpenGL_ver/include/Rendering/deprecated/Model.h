#pragma once

#include <Rendering/deprecated/Mesh.h>
#include <AnimData.h>

//static unsigned int TextureFromFile(const char* pathPtr, const std::string& directory, bool gamma = false);


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
    auto& GetOffsetMatMap() 
    {
        return m_OffsetMatMap;
    };
    int& GetBoneCount();
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

private:

    std::map<std::string, BoneInfo> m_OffsetMatMap;
    int m_BoneCount = 0;

    void LoadModel(std::string const& path);
    void ProcessNode(aiNode* nodePtr, const aiScene* scenePtr);
    Mesh ProcessMesh(aiMesh* meshPtr, const aiScene* scenePtr);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* matPtr, aiTextureType type, std::string typeName);
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    

};

/*unsigned int TextureFromFile(const char* pathPtr, const std::string& directory, bool gamma)
{
    std::string filename = std::string(pathPtr);
    filename = directory + '/' + filename;

    unsigned int textureId;
    glGenTextures(1, &textureId);

    int width, height, nrComponents;
    unsigned char* dataPtr = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (dataPtr)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, dataPtr);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(dataPtr);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << pathPtr << std::endl;
        stbi_image_free(dataPtr);
    }

    return textureId;
}*/
