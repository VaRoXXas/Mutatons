#pragma once



class DataManager
{
public:

    DataManager();
    ~DataManager();
    unsigned int* LoadAllTextures();
    unsigned int LoadTexture(char const* path);
    void LoadPlayBackgroundMusic(char const* path, sf::Music &music);
    void LoadSound(char const* path, sf::Sound &sound, sf::SoundBuffer &buffer);
    void LoadAllModels();
};
