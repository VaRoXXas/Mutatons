#include "pch.h"
#include "DataManager.h"
#include "FileNames.h"
#include "Rendering/Model.h"


Model* modelPtr;
unsigned int textures[2];
std::string directory = "res/textures";
unsigned int textureId;
sf::Sound* soundPtr;
sf::SoundBuffer* bufferPtr;
std::vector<Model*> vecModel;
std::vector<Model*> vecAnimModel;

DataManager::DataManager()
{
	
}

DataManager::~DataManager()
{
	delete soundPtr;
	delete bufferPtr;
	delete modelPtr;
}

unsigned int DataManager::LoadTexture(char const* pathPtr)
{
	textureId = modelPtr->TextureFromFile(pathPtr, directory);
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

void InitSound()
{
	soundPtr = new sf::Sound();
	bufferPtr = new sf::SoundBuffer();

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
	//0
	modelPtr = new Model(CANAL_STRAIGHT);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(CANAL_CORNER_IN);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(CANAL_STRAIGHT_W_PIPE);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(CANAL_NOWALL);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_PATH_STRAIGHT);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_PATH_CORNER);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_PATH_TJUNCTION);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_WALL_STRAIGHT);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_WALL_CORNER_OUT);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_WALL_CORNER_IN);
	vecModel.push_back(modelPtr);
	//10
	modelPtr = new Model(LAB_TERMINALS);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_TANK1);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_TANK2);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_PLATFORM);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_FLOOR_PROPS);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_FLOOR_BASIC);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_CABIN);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_PATH_CROSSING);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(LAB_GENERATOR);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_ROCK);
	vecModel.push_back(modelPtr);
	//20
	modelPtr = new Model(NATURE_TREES1);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_TREES2);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_TREES3);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_BRUSH1);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_BRUSH2);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_FLOWER);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_GROUND1);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_GROUND2);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_PATH_STRAIGHT);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(NATURE_PATH_TJUNCTION);
	vecModel.push_back(modelPtr);
	//30
	modelPtr = new Model(DIRECTION_SWITCH);
	vecModel.push_back(modelPtr);
	modelPtr = new Model(CREATURE);
	vecModel.push_back(modelPtr);
	//Animated
	modelPtr = new Model(ANIM);
	vecAnimModel.push_back(modelPtr);
}

unsigned int DataManager::LoadCubemap()
{
	std::vector<std::string> faces
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
	};

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
