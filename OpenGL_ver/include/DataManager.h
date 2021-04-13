#pragma once



class DataManager
{
public:

    DataManager();
    ~DataManager();
    unsigned int* LoadAllTextures();
    unsigned int LoadTexture(char const* pathPtr);
    void LoadPlayBackgroundMusic(char const* pathPtr, sf::Music &music);
    void LoadSound(char const* pathPtr, sf::Sound &sound, sf::SoundBuffer &buffer);
    void LoadAllModels();
};
