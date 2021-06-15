#pragma once

// Manages music and sounds.

class AudioManager
{
public:
	// Initializes all the necessary things.
	static void Initialize();
	// Ends main menu music and begins playing gameplay music.
	static void ChangeMainMenuMusicToGameplayMusic();
	// Plays a new sound from buffer.
	static void PlaySfSound(sf::SoundBuffer& buffer);
	// Cleans sounds list from sounds which stopped playing.
	static void TryClean();

private:
	static void LoadSoundFileToBuffer(const std::string& fileName, sf::SoundBuffer& buffer);
};

