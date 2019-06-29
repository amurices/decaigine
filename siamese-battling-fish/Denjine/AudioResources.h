#pragma once
#include "Headerz.h"
#include "IOManager.h"

namespace Denjine{

enum class PlaybackType {SOUND, STREAM};

// As with TextureResources, this is a class responsible for containing the audio resources that will be
// loaded into the game. They will remain in the map here so that ResourceManager has an easier time unloading everything.
class AudioResources
{
public:
	AudioResources();
	~AudioResources();

	// Get result of an attempt to load an audio file; if successful, &sound will be the address of the audio
	void getAudio(std::string audPath, FMOD::Sound** audio, FMOD::System* system, PlaybackType t);
	void unloadAudio(FMOD::System* system);
private:
	// Store pointers to where the sound objects are in memory
	std::map<std::string, FMOD::Sound**> _audioContainer;
};


}
