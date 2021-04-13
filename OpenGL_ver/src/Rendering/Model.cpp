#include "pch.h"

#include <Rendering/deprecated/Mesh.h>
#include <Rendering/Shader.h>
#include <Rendering/deprecated/Model.h>



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
    shader.SetMat4("transform", transform);
    Draw(shader);
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
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = meshPtr->mVertices[i].x;
        vector.y = meshPtr->mVertices[i].y;
        vector.z = meshPtr->mVertices[i].z;
        vertex.position = vector;
    // normals
        if (meshPtr->HasNormals())
        {
            vector.x = meshPtr->mNormals[i].x;
            vector.y = meshPtr->mNormals[i].y;
            vector.z = meshPtr->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (meshPtr->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = meshPtr->mTextureCoords[0][i].x;
            vec.y = meshPtr->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            // tangent
            vector.x = meshPtr->mTangents[i].x;
            vector.y = meshPtr->mTangents[i].y;
            vector.z = meshPtr->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = meshPtr->mBitangents[i].x;
            vector.y = meshPtr->mBitangents[i].y;
            vector.z = meshPtr->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

    vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < meshPtr->mNumFaces; i++)
    {
        aiFace face = meshPtr->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
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

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
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
