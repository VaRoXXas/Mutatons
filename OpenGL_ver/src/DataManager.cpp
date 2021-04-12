#include "pch.h"
#include "DataManager.h"
#include "FileNames.h"
#include "Rendering/deprecated/Model.h"

//Model* modelPtr;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{
}

unsigned int DataManager::LoadTexture(char const* path)
{
	//if you haven't done so already
	stbi_set_flip_vertically_on_load(true);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int* DataManager::LoadAllTextures()
{

	DataManager zmienna;
	unsigned int texture1[2];

	texture1[0] = zmienna.LoadTexture(CONTAINER_DIFFUSE);
	texture1[1] = zmienna.LoadTexture(GRASS_DIFFUSE);


	return texture1;
}

void DataManager::LoadPlayBackgroundMusic(char const* path, sf::Music &music)
{
	
	
	if (!music.openFromFile(path))
	{
		std::cout << "Music failed to load at path: " << path << std::endl;
	}
	music.setVolume(1.f);
	music.play();
}

void DataManager::LoadSound(char const* path, sf::Sound& sound, sf::SoundBuffer& buffer)
{
	if (!buffer.loadFromFile(path))
	{
		std::cout << "Music failed to load at path: " << path << std::endl;
	}
	
	sound.setBuffer(buffer);
	
}

void DataManager::LoadAllModels()
{
	//modelPtr = new Model(TANK1);
}
