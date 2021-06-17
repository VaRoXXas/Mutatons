#include "pch.h"
#include "AudioManager.h"

const bool MUSIC_ENABLED = true, SOUNDS_ENABLED = true;
std::list<sf::Sound*> soundsRunning;
sf::Music mainMenuMusic, gameplayMusic;
sf::SoundBuffer winningSoundBuffer, losingSoundBuffer, mutationSoundBuffer, controlPanelSoundBuffer, dyingFromLaserSoundBuffer;


void AudioManager::Initialize()
{
	LoadSoundFileToBuffer("winning_sound.ogg", winningSoundBuffer);
	LoadSoundFileToBuffer("mutation_sound.ogg", mutationSoundBuffer);
	LoadSoundFileToBuffer("losing_sound.ogg", losingSoundBuffer);
	LoadSoundFileToBuffer("dying_from_laser_sound.ogg", dyingFromLaserSoundBuffer);
	LoadSoundFileToBuffer("control_panel_sound.ogg", controlPanelSoundBuffer);

	if (MUSIC_ENABLED)
	{
		mainMenuMusic.openFromFile("res/audio/music/main_menu_music_looping.ogg");
		mainMenuMusic.setVolume(10.0f);
		mainMenuMusic.setLoop(true);
		mainMenuMusic.play();
	}
}

void AudioManager::ChangeMainMenuMusicToGameplayMusic()
{
	if (MUSIC_ENABLED)
	{
		mainMenuMusic.stop();
		gameplayMusic.openFromFile("res/audio/music/gameplay_music_looping.ogg");
		gameplayMusic.setVolume(10.0f);
		gameplayMusic.setLoop(true);
		gameplayMusic.play();
	}
}

void AudioManager::GameplayMusicToMainMenuMusic()
{
	if (MUSIC_ENABLED)
	{
		gameplayMusic.stop();
		mainMenuMusic.setVolume(10.0f);
		mainMenuMusic.setLoop(true);
		mainMenuMusic.play();
	}
}

void AudioManager::PlaySfSound(sf::SoundBuffer& buffer)
{
	if (SOUNDS_ENABLED)
	{
		auto* soundPtr = new sf::Sound();
		soundPtr->setBuffer(buffer);
		soundPtr->setVolume(10.0f);
		soundPtr->setLoop(false);
		soundPtr->play();
		soundsRunning.push_back(soundPtr);
	}
}

void AudioManager::TryClean()
{
	for (auto* sound : soundsRunning)
	{
		if (sound->Stopped)
		{
			soundsRunning.remove(sound);
			delete sound;
		}
	}
}

void AudioManager::LoadSoundFileToBuffer(const std::string& fileName, sf::SoundBuffer& buffer)
{
	buffer.loadFromFile(std::string("res/audio/sounds/") + fileName);
}
