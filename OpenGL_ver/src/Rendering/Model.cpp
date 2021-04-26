#include "pch.h"

#include <Rendering/deprecated/Mesh.h>
#include <Rendering/Shader.h>
#include <Rendering/deprecated/Model.h>
#include <AnimData.h>
#include <AssimpGLMHelpers.h>



Model::Model(std::string const& path, bool gamma) : gammaCorrection(gamma)
{
    LoadModel(path);
}

void Model::Draw(Shader& shader)
{
    shader.Use();
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::CustomRender(Shader& shader, glm::mat4 transform)
{
    shader.Use();
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetMat4("transform", transform);
    Draw(shader);
}

/*auto& Model::GetOffsetMatMap()
{
    return m_OffsetMatMap;
}*/

int& Model::GetBoneCount()
{
    return m_BoneCount;
}



void Model::LoadModel(std::string const& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* nodePtr, const aiScene* scenePtr)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < nodePtr->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scenePtr->mMeshes[nodePtr->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scenePtr));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < nodePtr->mNumChildren; i++)
    {
        ProcessNode(nodePtr->mChildren[i], scenePtr);
    }
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.mBoneIDs[i] = -1;
        vertex.mWeights[i] = 0.0f;
    }
}

Mesh Model::ProcessMesh(aiMesh* meshPtr, const aiScene* scenePtr)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < meshPtr->mNumVertices; i++)
    {
        Vertex vertex;
        SetVertexBoneDataToDefault(vertex);
        vertex.position = AssimpGLMHelpers::GetGLMVec(meshPtr->mVertices[i]);
        vertex.normal = AssimpGLMHelpers::GetGLMVec(meshPtr->mNormals[i]);

        if (meshPtr->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = meshPtr->mTextureCoords[0][i].x;
            vec.y = meshPtr->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < meshPtr->mNumFaces; i++)
    {
        aiFace face = meshPtr->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* materialPtr = scenePtr->mMaterials[meshPtr->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(materialPtr, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(materialPtr, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(materialPtr, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(materialPtr, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    ExtractBoneWeightForVertices(vertices, meshPtr, scenePtr);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.mBoneIDs[i] < 0)
        {
            vertex.mWeights[i] = weight;
            vertex.mBoneIDs[i] = boneID;
            break;
        }
    }
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* meshPtr, const aiScene* scenePtr)
{
    auto& boneInfoMap = m_OffsetMatMap;
    int& boneCount = m_BoneCount;

    for (int boneIndex = 0; boneIndex < meshPtr->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = meshPtr->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCount;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(meshPtr->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = meshPtr->mBones[boneIndex]->mWeights;
        int numWeights = meshPtr->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

unsigned int Model::TextureFromFile(const char* pathPtr, const std::string& directory, bool gamma)
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
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* matPtr, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < matPtr->GetTextureCount(type); i++)
    {
        aiString str;
        matPtr->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}
