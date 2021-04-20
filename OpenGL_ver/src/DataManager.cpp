#include "pch.h"
#include "DataManager.h"
#include "FileNames.h"
#include "Rendering/deprecated/Model.h"

Model* modelPtr;
unsigned int textures[2];
std::string directory = "res/textures";
unsigned int textureId;



DataManager::DataManager()
{
	//this->soundPtr = new sf::Sound;
	//this->bufferPtr = new sf::SoundBuffer;
}

DataManager::~DataManager()
{
	delete this->soundPtr;
	delete this->bufferPtr;
	delete modelPtr;
}

unsigned int DataManager::LoadTexture(char const* pathPtr)
{
	textureId = TextureFromFile(pathPtr, directory);
	return textureId;
	//if you haven't done so already
	/*stbi_set_flip_vertically_on_load(true);

	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(pathPtr, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
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
		std::cout << "Texture failed to load at path: " << pathPtr << std::endl;
		stbi_image_free(data);
	}

	return textureId;
	*/
}

unsigned int* DataManager::LoadAllTextures()
{
	//return table with textures
	

	textures[0] = DataManager::LoadTexture(CONTAINER_DIFFUSE);
	textures[1] = DataManager::LoadTexture(GRASS_DIFFUSE);


	return textures;
}

void DataManager::LoadPlayBackgroundMusic(char const* pathPtr, sf::Music &music)
{
	
	
	if (!music.openFromFile(pathPtr))
	{
		std::cout << "Music failed to load at path: " << pathPtr << std::endl;
	}
	music.setVolume(1.f);
	music.play();
}

sf::Sound* DataManager::LoadSound(char const* pathPtr)
{
	
	if (!bufferPtr->loadFromFile(pathPtr))
	{
		std::cout << "Music failed to load at path: " << pathPtr << std::endl;
	}
	
	soundPtr->setBuffer(*bufferPtr);
	
	return soundPtr;
}

void DataManager::LoadAllModels()
{
	//modelPtr = new Model(CANAL_CORNER_IN);
	modelPtr = new Model(NATURE_TREES3);
}

unsigned int DataManager::LoadCubemap(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* dataPtr = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (dataPtr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataPtr
			);
			stbi_image_free(dataPtr);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(dataPtr);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureId;
}
