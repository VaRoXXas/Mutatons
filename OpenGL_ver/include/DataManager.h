#pragma once



class DataManager
{
public:

    DataManager();
    ~DataManager();
    unsigned int* LoadAllTextures();
    unsigned int LoadTexture(char const* pathPtr);
    void LoadPlayBackgroundMusic(char const* pathPtr, sf::Music &music);
    sf::Sound* LoadSound(char const* pathPtr);
    void LoadAllModels();
    unsigned int LoadCubemap(std::vector<std::string> faces);
};
